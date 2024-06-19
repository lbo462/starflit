import cv2
from torch_net import get_torch_net

from classes import classes

KLASS = "iPod"


def main():
    cap = cv2.VideoCapture(0, cv2.CAP_V4L2)
    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 224)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 224)
    cap.set(cv2.CAP_PROP_FPS, 36)

    with get_torch_net(KLASS, 0.1, classes) as t:
        while True:
            ret, image = cap.read()
            if not ret:
                raise RuntimeError("failed to read frame")

            if t.search(image):
                print(f"Found {KLASS} !")


if __name__ == "__main__":
    main()
