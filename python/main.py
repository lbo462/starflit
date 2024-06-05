from serial_interface import get_serial


def main():
    with get_serial("/dev/ttyUSB5", 115200) as ser:
        print(ser.recv())


if __name__ == "__main__":
    main()
