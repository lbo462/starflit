from dataclasses import dataclass


@dataclass
class OutGoingFrame:
    """
    Specifies the content of an outgoing frame.
    """

    x_object_position: int
    """
    Specifies the position of the recognized object on the x-axis.
    """

    y_object_position: int
    """
    Specifies the position of the recognized object on the y-axis.
    """
