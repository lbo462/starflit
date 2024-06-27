## @addtogroup Python Python
## @{

from dataclasses import dataclass

DATA_DUPLICATION_FACTOR = 3
"""
How many times the sent bytes are duplicated.
Please, do not edit this without thinking. It
would have unexpected impacts, and should match
the Arduino side.

Most problems will arise on the Arduino side
when parsing the frame. Here, it would work
just as expected and duplicate each bytes the
expected amount of times.
"""


@dataclass
class OutGoingFrame:
    """
    Specifies the content of an outgoing frame.
    """

    initialized: bool
    """
    Tells is the RPI is actually initialized or not.
    """

    object_detected: bool
    """
    Tells if the RPI detected an object or not.
    """

    x_object_position: int = 0
    """
    Specifies the position of the recognized object on the x-axis.
    Default is 0 when no object is detected.
    """

    y_object_position: int = 0
    """
    Specifies the position of the recognized object on the y-axis.
    Default is 0 when no object is detected.
    """


## @}
