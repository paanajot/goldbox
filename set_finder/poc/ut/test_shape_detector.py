import cv2
import pytest

from set_finder.shape_detector import ShapeDetector
from set_finder.card_preprocessor import CardPreprocessor

from cards import Shape
from os import listdir

filePath1 = "/repo/data/images/shape_detector/test/diamond/"
expectedShape1 = Shape.DIAMOND

filePath2 = "/repo/data/images/shape_detector/test/squiggle/"
expectedShape2 = Shape.SQUIGGLE

filePath3 = "/repo/data/images/shape_detector/test/oval/"
expectedShape3 = Shape.OVAL


@pytest.mark.parametrize(
    "folderPath,expectedShape",
    [
        (filePath1, expectedShape1),
        (filePath2, expectedShape2),
        (filePath3, expectedShape3),
    ],
)
class TestShapeDetector:
    def test(self, folderPath, expectedShape):
        files = listdir(folderPath)

        for fileName in files:
            sourceImg = cv2.imread(folderPath + fileName)
            cardPreprocessor = CardPreprocessor(sourceImg)

            assert ShapeDetector(cardPreprocessor).getShape() == expectedShape, (
                folderPath + fileName
            )
