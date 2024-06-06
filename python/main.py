from serial_interface import get_serial
from time import sleep


def main():
    with get_serial("/dev/ttyUSB0", 115200) as ser:
        while True:
            ser.send(b"Hello world")
            print("sent!")
            sleep(2)


if __name__ == "__main__":
    main()
