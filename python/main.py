from serial_interface import get_serial


def main():
    with get_serial("/dev/ttyUSB2", 115200) as ser:
        while True:
            ser.send(b"Hello world")


if __name__ == "__main__":
    main()
