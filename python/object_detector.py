## @addtogroup Python Python
## @{

import tflite_runtime.interpreter as tflite
import cv2
import numpy as np


class ObjectDetector:
    """
    Detects the object that is defined when the class is initiated.
    """

    def __init__(self, object: str, normalSize: int, lowresSize: int) -> None:
        self.object = object
        self.normalSize = normalSize
        self.lowresSize = lowresSize

    def calculate_box_center_and_normalize(
        self, xmin: float, xmax: float, ymin: float, ymax: float
    ):
        """
        First, calculates the center of the box returned by the model.
        Then, normalizes the x and y axis to be between -lowresSize/2 and lowresSize/2.
        """
        x_frame, y_frame = self.lowresSize
        center = [abs(xmax - xmin) / 2, abs(ymax - ymin) / 2]
        normalized = [
            int((center[0] - (x_frame / 2))),
            int((center[0] - (y_frame / 2))),
        ]
        return normalized

    def run_inference(self, model, labels, threads, image):
        """
        Runs the inference.
        """

        with open(labels, "r") as f:
            labels = {}
            lines = f.readlines()
            for line in lines:
                split = line.strip().split(maxsplit=1)
                labels[int(split[0])] = split[1]

        # Initializes the interpreter and allocates tensors (mandatory)
        interpreter = tflite.Interpreter(model_path=model, num_threads=threads)
        interpreter.allocate_tensors()

        # Gets input and output details to define image size and detected classes
        input_details = interpreter.get_input_details()
        output_details = interpreter.get_output_details()
        height = input_details[0]["shape"][1]
        width = input_details[0]["shape"][2]

        # Checks if the model expects floats ar input
        floating_model = False
        if input_details[0]["dtype"] == np.float32:
            floating_model = True

        # Converts the input image from grayscale to RGB
        rgb_image = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)
        initial_h, initial_w, _ = rgb_image.shape

        # Resizes input image to size the model expects
        resized_img = cv2.resize(rgb_image, (width, height))

        # Adds a column to the start of the array
        input_data = np.expand_dims(resized_img, axis=0)

        # Normalizes the input data to the range -1:1
        if floating_model:
            input_data = (np.float32(input_data) - 127.5) / 127.5

        # Starts the interpreter
        interpreter.set_tensor(input_details[0]["index"], input_data)
        interpreter.invoke()

        # Gets the classes and scores that the model outputs
        detected_boxes = interpreter.get_tensor(output_details[0]["index"])
        detected_classes = interpreter.get_tensor(output_details[1]["index"])
        detected_score = interpreter.get_tensor(output_details[2]["index"])
        num_boxes = interpreter.get_tensor(output_details[3]["index"])

        # Prints the output and return the coordinates of the boxes
        for i in range(int(num_boxes)):
            score = detected_score[0][i]
            class_id = int(detected_classes[0][i])
            if self.object == labels[class_id]:
                if score > 0.1:
                    top, left, bottom, right = detected_boxes[0][i]
                    xmin = left * initial_w
                    ymin = bottom * initial_h
                    xmax = right * initial_w
                    ymax = top * initial_h

                    print(labels[class_id], "score = ", score)
                    return self.calculate_box_center_and_normalize(
                        xmin, xmax, ymin, ymax
                    )
                else:
                    return False


## @}
