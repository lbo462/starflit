from typing import ContextManager
from contextlib import contextmanager
from serial import Serial


class SerialInterface:
    """
    Interfaces with the Arduino card through serial communication.
    ! Do not call directly !
    Instead, use the `get_serial()` context manager as such:

    ```python
    with get_serial(port, baud) as ser:
        ser.send("Hello world")
        # do many other things with ser ...
    ```
    """

    def __init__(self, port: str, baud: int):
        self._serial = Serial()
        self._serial.port = port
        self._serial.baudrate = baud

    def open(self):
        """Open the communication"""
        self._serial.open()

    def close(self):
        """Closes the communication"""
        self._serial.close()

    def recv(self) -> bytes:
        return self._serial.read()


@contextmanager
def get_serial(port: str, baud: int) -> ContextManager[SerialInterface]:
    ser = SerialInterface(port, baud)
    ser.open()
    yield ser
    ser.close()
