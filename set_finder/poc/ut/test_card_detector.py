import cv2
import pytest

import set_finder

from os import listdir

filePath1 = "/repo/data/images/card_detector/3/"
expected1 = 3

filePath2 = "/repo/data/images/card_detector/12/"
expected2 = 12

filePath3 = "/repo/data/images/card_detector/15/"
expected3 = 15


@pytest.mark.parametrize(
    "folderPath,expected",
    [
        (filePath1, expected1),
        (filePath2, expected2),
        (filePath3, expected3),
    ],
)
class TestCardDetector:
    def test(self, folderPath, expected):
        files = listdir(folderPath)

        fails = []
        for fileName in files:
            sourceImg = cv2.imread(folderPath + fileName)

            sourceImg = set_finder.resizer.getResizedImageKeepRatio(
                sourceImg, width=800
            )
            cardDetector = set_finder.CardDetector(sourceImg)
            if cardDetector.getNumberOfCards() != expected:
                fails.append((fileName, cardDetector.getNumberOfCards()))

        assert len(fails) == 0, fails
