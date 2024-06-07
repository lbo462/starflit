from serial_interface import get_serial
from time import sleep


def main():
    with get_serial("/dev/ttyUSB0", 115200) as ser:
        while True:
            x = 8289  # 20 61
            y = 8290  # 20 62

            m = [x.to_bytes(2, "big"), y.to_bytes(2, "big")]

            # ser.send(b"".join(m))
            ser.send(bytes(b"\x20\x61\x20\x62"))
            sleep(1)


if __name__ == "__main__":
    main()
