import cv2
import pytest

from set_finder.shading_detector import ShadingDetector
from set_finder.card_preprocessor import CardPreprocessor

from cards import Shading
from os import listdir

filePath1 = "/repo/data/images/shading_detector/test/outlined/"
expectedShading1 = Shading.OUTLINED

filePath2 = "/repo/data/images/shading_detector/test/solid/"
expectedShading2 = Shading.SOLID

filePath3 = "/repo/data/images/shading_detector/test/striped/"
expectedShading3 = Shading.STRIPED


@pytest.mark.parametrize(
    "folderPath,expectedShading",
    [
        (filePath1, expectedShading1),
        (filePath2, expectedShading2),
        (filePath3, expectedShading3),
    ],
)
class TestShadingDetector:
    def test(self, folderPath, expectedShading):
        files = listdir(folderPath)

        for fileName in files:
            sourceImg = cv2.imread(folderPath + fileName)
            cardPreprocessor = CardPreprocessor(sourceImg)

            assert ShadingDetector(cardPreprocessor).getShading() == expectedShading, (
                folderPath + fileName
            )
