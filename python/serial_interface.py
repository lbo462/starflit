## @addtogroup Python Python
## @{

from typing import ContextManager
from contextlib import contextmanager
from serial import Serial

STX = b"\x02"
ETX = b"\x03"


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
        The frame ends with a ETX and start with STX.
        This function blocks the code execution until EOT is received from the serial.
        :return: A bytes object containing the received data, with the STX and ETX removed.
        """

        # Empty the buffer until an STX byte is received.
        while self._serial.read() != STX:
            ...

        return self._serial.read_until(ETX)[:-1]

    def send(self, msg: bytes):
        """
        Send a message to the serial connection.
        The message is sent as an array of bytes.
        This function auto adds a ETX byte at the end of the sequence
        and an STX at its beginning.
        """
        sequence = STX + msg + ETX
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
