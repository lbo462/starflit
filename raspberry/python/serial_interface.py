## @addtogroup Python Python
## @{

from typing import ContextManager
from contextlib import contextmanager
from serial import Serial

from frame import OutGoingFrame, DATA_DUPLICATION_FACTOR

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
        """Opens the communication"""
        self._serial.open()

    def close(self):
        """Closes the communication"""
        self._serial.close()

    def send_frame(self, frame: OutGoingFrame):
        """
        Sends a frame to the serial port.
        The frame is transformed into a byte array and sent to the serial port
        via the `_send()` method.

        One converts bytes using `bytes((variable,))` since `bytes(variables)` does not output what we need.
        One converts signed int into two bytes, using big endian.

        The data is also sent duplicated `DATA_DUPLICATION_FACTOR` times.
        """

        msg_arr = [
            bytes((frame.initialized,)),
            bytes((frame.object_detected,)),
            frame.x_object_position.to_bytes(2, "big", signed=True)[:1],
            frame.x_object_position.to_bytes(2, "big", signed=True)[1:],
            frame.y_object_position.to_bytes(2, "big", signed=True)[:1],
            frame.y_object_position.to_bytes(2, "big", signed=True)[1:],
        ]

        duplicated_msg = [b for b in msg_arr for _ in range(DATA_DUPLICATION_FACTOR)]
        return self._send(b"".join(duplicated_msg))

    def _send(self, msg: bytes):
        """
        Send a message to the serial connection.
        The message is sent as an array of bytes.
        This function auto adds a ETX byte at the end of the sequence
        and an STX at its beginning.
        """
        self._serial.write(STX + msg + ETX)


@contextmanager
def get_serial(port: str, baud: int) -> ContextManager[SerialInterface]:
    """
    Get a temporary serial interface.
    To use as follows:

    ```python
    with get_serial(...) as ser:
        ser.send(b'Hello world')
    ```
    """
    ser = SerialInterface(port, baud)
    ser.open()
    yield ser
    ser.close()


## @}
