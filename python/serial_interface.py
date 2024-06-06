## @addtogroup Python Python
## @{

from typing import ContextManager
from contextlib import contextmanager
from serial import Serial

EOT = b"\x04"


class SerialInterface:
    """
    Interfaces with the Arduino card through serial communication.
    ! Do not call directly !
    Instead, use the `get_serial()` context manager:
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
        """
        Get a full frame from the serial connection.
        The frame ends with a EOT.
        This function blocks the code execution until EOT is received from the serial.
        :return: A bytes object containing the received data, with the EOT removed.
        """
        return self._serial.read_until(EOT)[:-1]

    def send(self, msg: bytes):
        """
        Send a message to the serial connection.
        The message is sent as an array of bytes.
        This function auto adds a EOT byte at the end of the sequence.
        """
        sequence = msg + EOT
        print(sequence)
        self._serial.write(sequence)


@contextmanager
def get_serial(port: str, baud: int) -> ContextManager[SerialInterface]:
    """
    Get a temporary serial interface.
    To use as follows:

    with get_serial(...) as ser:
        ser.send(b'Hello world')
    """
    ser = SerialInterface(port, baud)
    ser.open()
    yield ser
    ser.close()


## @}
