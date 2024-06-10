import os
from dotenv import load_dotenv

from frame import OutGoingFrame
from serial_interface import get_serial

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
    outgoing_frame = OutGoingFrame(
        x_object_position=512,
        y_object_position=-128,
    )

    with get_serial(os.getenv("SERIAL_PORT"), os.getenv("SERIAL_BAUD")) as ser:
        ser.send_frame(outgoing_frame)


if __name__ == "__main__":
    main()
