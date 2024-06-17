## @addtogroup Python Python
## @{

from dataclasses import dataclass


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
