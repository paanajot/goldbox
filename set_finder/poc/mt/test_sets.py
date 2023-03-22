import pytest
import cv2


from set_finder.set_manager import SetManager


filePath1 = "/repo/data/images/mt_data/sets/20221018_155649.jpg"
expectedSets1 = [(0, 3, 4), (0, 6, 10), (1, 6, 9), (5, 6, 7), (8, 9, 10)]

filePath2 = "/repo/data/images/mt_data/sets/20221114-143822.jpg"
expectedSets2 = [(9, 10, 11)]


@pytest.mark.parametrize(
    "filePath,expectedSets", [(filePath1, expectedSets1), (filePath2, expectedSets2)]
)
class TestDetectors:
    def test(self, filePath, expectedSets):
        image = cv2.imread(filePath)

        setManager = SetManager()
        setManager.fullRun(image)
        sets = setManager.getSets()
        setIds = []
        for threeCards in sets:
            setIds.append(tuple([card.id for card in threeCards]))
        assert setIds == expectedSets, filePath

        setManager = SetManager()
        setManager.run(image, useThreads=False)

        sets = setManager.getSets()
        setIds = []
        for threeCards in sets:
            setIds.append(tuple([card.id for card in threeCards]))
        assert setIds == expectedSets, filePath
