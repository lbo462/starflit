import os
from dotenv import load_dotenv
import argparse

from picamera2 import Picamera2, Preview

from frame import OutGoingFrame
from serial_interface import get_serial
from image_recognition import run_inference, lowresSize



# Remember to read the doc!
try:
    with open(".env", "r") as _:
        ...
except FileNotFoundError:
    print(
        "Please, create a .env file in the same folder as this file. Check the docs / DevEnv.md"
    )
    exit(1)

load_dotenv()


def main():
    # Parses the arguments given in the command line
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', help='Path of the detection model.', required=True)
    parser.add_argument('--label', help='Path of the labels file.')
    parser.add_argument('--threads', help='Number of threads used to run the model.', type=int, required=True)
    args = parser.parse_args()

    # Creates an instance of the picamera and configures it
    picam2 = Picamera2()
    config = picam2.create_preview_configuration(lores={"size": lowresSize})
    picam2.configure(config)
    # picam2.start_preview(Preview.QT)

    # Gets the length of each row of the image in bytes
    stride = picam2.stream_configuration('lores')['stride']

    # Starts streaming
    picam2.start()

    # Converts buffer data to grey scale and runs inference
    while True:
        buf = picam2.capture_buffer('lores')
        greyscale_img = buf[:stride * lowresSize[1]].reshape((lowresSize[1], stride))
        coord = run_inference(args.model, args.label ,args.threads, greyscale_img)

        outgoing_frame = OutGoingFrame(
            x_object_position=coord[0],
            y_object_position=coord[1],
        )

        with get_serial(os.getenv("SERIAL_PORT"), os.getenv("SERIAL_BAUD")) as ser:
            ser.send_frame(outgoing_frame)


if __name__ == "__main__":
    main()
