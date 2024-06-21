import os
from dotenv import load_dotenv
import argparse

from picamera2 import Picamera2, Preview

from frame import OutGoingFrame
from serial_interface import get_serial
from object_detector import ObjectDetector


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

LOW_RES_SIZE = (320, 240)


def main():
    # First things first, open the serial monitor, as this can take quite some time ...
    with get_serial(os.getenv("SERIAL_PORT"), os.getenv("SERIAL_BAUD")) as ser:
        # Say we're not ready yet ...
        ser.send_frame(OutGoingFrame(initialized=False, object_detected=False))

        # Parses the arguments given in the command line
        parser = argparse.ArgumentParser()
        parser.add_argument(
            "-m", "--model", help="Path of the detection model.", required=True
        )
        parser.add_argument(
            "-l", "--label", help="Path of the labels file.", required=True
        )
        parser.add_argument(
            "--threads",
            help="Number of threads used to run the model.",
            type=int,
            default=3,
        )
        parser.add_argument(
            "-o", "--obj", help="The object to search for.", required=True
        )
        args = parser.parse_args()

        # Creates an instance of the picamera and configures it
        picam2 = Picamera2()
        config = picam2.create_preview_configuration(lores={"size": LOW_RES_SIZE})
        picam2.configure(config)

        # Gets the length of each row of the image in bytes
        stride = picam2.stream_configuration("lores")["stride"]

        # Starts streaming
        picam2.start()

        # Create an object detector instance
        object_detector = ObjectDetector(args.obj, args.model, args.label, LOW_RES_SIZE)

        # Tells that we're ready
        ser.send_frame(OutGoingFrame(initialized=True, object_detected=False))

        while True:
            # Converts buffer data to grey scale and runs inference
            buf = picam2.capture_buffer("lores")
            greyscale_img = buf[: stride * LOW_RES_SIZE[1]].reshape(
                (LOW_RES_SIZE[1], stride)
            )
            coord = object_detector.run_inference(greyscale_img, threads=args.threads)

            if coord:
                outgoing_frame = OutGoingFrame(
                    initialized=True,
                    object_detected=True,
                    x_object_position=coord[0],
                    y_object_position=coord[1],
                )
                print(f"Detected {args.obj} at ({coord[0]}, {coord[1]})")
            else:
                outgoing_frame = OutGoingFrame(initialized=True, object_detected=False)

            ser.send_frame(outgoing_frame)


if __name__ == "__main__":
    main()
