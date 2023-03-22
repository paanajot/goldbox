import pytest
import cv2

from set_finder.set_manager import SetManager
from os import listdir

folderPath0 = "/repo/data/images/mt_data/no_set/"
expectedSets0 = 0

folderPath1 = "/repo/data/images/mt_data/one_set/"
expectedSets1 = 1


@pytest.mark.parametrize(
    "folderPath,expectedSets",
    [(folderPath0, expectedSets0), (folderPath1, expectedSets1)],
)
class TestSetNumber:
    def test(self, folderPath, expectedSets):
        files = listdir(folderPath)

        for fileName in files:
            image = cv2.imread(folderPath + fileName)
            setManager = SetManager()
            setManager.fullRun(image)

            assert len(setManager.getSets()) == expectedSets, folderPath + fileName
