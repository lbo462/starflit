import tflite_runtime.interpreter as tflite
import cv2
import numpy as np

normalSize = (640, 480)
lowresSize = (320, 240)

def run_inference(model, labels, threads, image):
    """
    Runs the inference.
    """

    with open(labels, 'r') as f :
        labels = {}
        lines = f.readlines()
        for line in lines:
            split = line.strip().split()
            labels[int(split[0])] = split[1]
    
    # Initializes the interpreter and allocates tensors (mandatory)
    interpreter = tflite.Interpreter(model_path=model, num_threads=threads)
    interpreter.allocate_tensors()

    # Gets input and output details to define image size and detected classes
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    height = input_details[0]['shape'][1]
    width = input_details[0]['shape'][2]

    # Checks if the model expects floats ar input
    floating_model = False
    if input_details[0]['dtype'] == np.float32:
        floating_model = True

    # Converts the input image from grayscale to RGB
    rgb_image = cv2.cvtColor(image, cv2.COLOR_GRAY2RGB)

    # Resizes input image to size the model expects 
    resized_img = cv2.resize(rgb_image, (width, height))

    # Adds a column to the start of the array
    input_data = np.expand_dims(resized_img, axis=0)

    # Normalizes the input data to the range -1:1
    if floating_model:
        input_data = (np.float32(input_data) - 127.5) / 127.5

    # Starts the interpreter
    interpreter.set_tensor(input_details[0]['index'], input_data)
    interpreter.invoke()

    # Gets the classes and scores that the model outputs
    detected_classes = interpreter.get_tensor(output_details[1]['index'])
    detected_score = interpreter.get_tensor(output_details[2]['index'])

    for dclass in detected_classes[0]:
        # print(dclass)
        for dscore in detected_score[0]:
            # print(dscore)
            if dscore >= 0.8:
                print(labels[dclass], " @ ", dscore, "accuracy")
    return [512, -128]