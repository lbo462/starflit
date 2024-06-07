from serial_interface import get_serial
from time import sleep


def main():
    with get_serial("/dev/ttyUSB0", 115200) as ser:
        while True:

            for x in range(-32767, 32768, 100):
                y = -128
                print(f"\n{x}, {y}")
                m = [
                    x.to_bytes(2, "big", signed=True),
                    y.to_bytes(2, "big", signed=True),
                ]
                ser.send(b"".join(m))
                sleep(0.1)


if __name__ == "__main__":
    main()
