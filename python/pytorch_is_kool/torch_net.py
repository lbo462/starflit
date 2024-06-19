from contextlib import contextmanager
from typing import ContextManager, Dict

import torch
from torchvision import models, transforms

# torch.backends.quantized.engine = "qnnpack"


class TorchNet:
    def __init__(self, klass: str, confidence: float, klasses: Dict):
        self.klass = klass
        """
        Class name of the object we're looking for
        """

        self.confidence = confidence
        """
        Confidence required to detect the object.
        Between 0 and 1.
        """

        self.klasses = klasses
        """
        Set of classes for our model.
        Presented under the form of a dictionary like:
        `{0: 'class1', 1: 'class2'}`
        """

        self.preprocess = transforms.Compose(
            [
                transforms.ToTensor(),
                transforms.Normalize(
                    mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]
                ),
            ]
        )

        net = models.get_model("quantized_mobilenet_v3_large", weights="IMAGENET1K_V2")
        # jit model to take it from ~20fps to ~30fps
        self.net = torch.jit.script(net)

    def search(self, image) -> bool:
        """
        Search for the object if the given image
        :return: True if the object was found, False otherwise.
        """

        # convert opencv output from BGR to RGB
        image = image[:, :, [2, 1, 0]]

        # preprocess
        input_tensor = self.preprocess(image)

        # create a mini-batch as expected by the model
        input_batch = input_tensor.unsqueeze(0)

        # run model
        output = self.net(input_batch)
        top = list(enumerate(output[0].softmax(dim=0)))
        for idx, val in top[:10]:
            print(f"{val.item() * 100:.2f}% {self.klasses[idx]}")
            if self.klasses[idx] == self.klass and val.item() > self.confidence:
                return True

        return False


@contextmanager
def get_torch_net(
    klass: str, confidence: float, klasses: Dict
) -> ContextManager[TorchNet]:
    t = TorchNet(klass, confidence, klasses)
    with torch.no_grad():
        yield t
