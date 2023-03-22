import cv2
import pytest

from set_finder.deep_color_detector import DeepColorDetector
from cards import Color
from os import listdir

filePath1 = "/repo/data/images/color_detector/test/red/"
expectedColor1 = Color.RED

filePath2 = "/repo/data/images/color_detector/test/green/"
expectedColor2 = Color.GREEN

filePath3 = "/repo/data/images/color_detector/test/purple/"
expectedColor3 = Color.PURPLE

filePath4 = "/repo/data/images/color_detector/app_test/red/"
expectedColor4 = Color.RED

filePath5 = "/repo/data/images/color_detector/app_test/green/"
expectedColor5 = Color.GREEN

filePath6 = "/repo/data/images/color_detector/app_test/purple/"
expectedColor6 = Color.PURPLE

@pytest.mark.parametrize(
    "folderPath,expectedColor",
    [
        (filePath1, expectedColor1),
        (filePath2, expectedColor2),
        (filePath3, expectedColor3),
        (filePath4, expectedColor4),
        (filePath5, expectedColor5),
        (filePath6, expectedColor6),
    ],
)
class TestColorDetector:
    _colorDetector = DeepColorDetector()

    def test(self, folderPath, expectedColor):
        files = listdir(folderPath)

        for fileName in files:
            sourceImg = cv2.imread(folderPath + fileName)

            assert self._colorDetector.getColor(sourceImg) == expectedColor, (
                folderPath + fileName
            )
