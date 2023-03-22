import cv2
import pytest

from set_finder.number_detector import NumberDetector
from set_finder.card_preprocessor import CardPreprocessor

from os import listdir

filePath1 = "/repo/data/images/number_detector/test/one/"
expectedNumber1 = 1

filePath2 = "/repo/data/images/number_detector/test/two/"
expectedNumber2 = 2

filePath3 = "/repo/data/images/number_detector/test/three/"
expectedNumber3 = 3


@pytest.mark.parametrize(
    "folderPath,expectedNumber",
    [
        (filePath1, expectedNumber1),
        (filePath2, expectedNumber2),
        (filePath3, expectedNumber3),
    ],
)
class TestNumberDetector:
    def test(self, folderPath, expectedNumber):
        files = listdir(folderPath)

        for fileName in files:
            sourceImg = cv2.imread(folderPath + fileName)
            cardPreprocessor = CardPreprocessor(sourceImg)

            assert NumberDetector(cardPreprocessor).getNumber() == expectedNumber, (
                folderPath + fileName
            )
