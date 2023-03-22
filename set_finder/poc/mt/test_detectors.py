import cv2
import pytest

import set_finder

from cards import Color, Shape, Shading

from set_finder.number_detector import NumberDetector
from set_finder.deep_color_detector import DeepColorDetector
from set_finder.shape_detector import ShapeDetector
from set_finder.shading_detector import ShadingDetector
from set_finder.card_preprocessor import CardPreprocessor

filePath1 = "/repo/data/images/mt_data/detectors/20221018_155541.jpg"
expectedNumbers1 = [3, 1, 3, 2, 3, 1, 2, 3, 1, 2, 1, 2]
expectedColors1 = [
    Color.RED,
    Color.PURPLE,
    Color.GREEN,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
    Color.PURPLE,
    Color.RED,
    Color.GREEN,
]
expectedShapes1 = [
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.SQUIGGLE,
    Shape.OVAL,
]
expectedShading1 = [
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.STRIPED,
    Shading.SOLID,
]

filePath2 = "/repo/data/images/mt_data/detectors/20221018_155611.jpg"
expectedNumbers2 = [3, 1, 3, 2, 3, 1, 2, 3, 1, 2, 1, 2]
expectedColors2 = [
    Color.RED,
    Color.PURPLE,
    Color.GREEN,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
    Color.PURPLE,
    Color.RED,
    Color.GREEN,
]
expectedShapes2 = [
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.SQUIGGLE,
    Shape.OVAL,
]
expectedShading2 = [
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.STRIPED,
    Shading.SOLID,
]

filePath3 = "/repo/data/images/mt_data/detectors/20221018_155627.jpg"
expectedNumbers3 = [2, 1, 2, 1, 3, 2, 1, 3, 2, 3, 1, 3]
expectedColors3 = [
    Color.GREEN,
    Color.RED,
    Color.PURPLE,
    Color.PURPLE,
    Color.GREEN,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
    Color.RED,
]
expectedShapes3 = [
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.OVAL,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.DIAMOND,
]
expectedShading3 = [
    Shading.SOLID,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
]

filePath4 = "/repo/data/images/mt_data/detectors/20221018_155649.jpg"
expectedNumbers4 = [3, 2, 1, 1, 2, 2, 1, 3, 1, 3, 2, 3]
expectedColors4 = [
    Color.PURPLE,
    Color.GREEN,
    Color.GREEN,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.GREEN,
    Color.RED,
    Color.PURPLE,
    Color.RED,
]
expectedShapes4 = [
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.DIAMOND,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.SQUIGGLE,
]
expectedShading4 = [
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.STRIPED,
    Shading.STRIPED,
    Shading.STRIPED,
    Shading.SOLID,
]

filePath5 = "/repo/data/images/mt_data/detectors/20221018_155726.jpg"
expectedNumbers5 = [3, 3, 3, 1, 1, 3, 1, 2, 1, 2, 2, 3]
expectedColors5 = [
    Color.RED,
    Color.RED,
    Color.GREEN,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.RED,
    Color.GREEN,
    Color.GREEN,
]
expectedShapes5 = [
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.OVAL,
]
expectedShading5 = [
    Shading.SOLID,
    Shading.STRIPED,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.STRIPED,
    Shading.STRIPED,
]

filePath6 = "/repo/data/images/mt_data/detectors/20221029_130715.jpg"
expectedNumbers6 = [3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 3, 2]
expectedColors6 = [
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
    Color.PURPLE,
]
expectedShapes6 = [
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.DIAMOND,
    Shape.OVAL,
]
expectedShading6 = [
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.OUTLINED,
    Shading.SOLID,
]

filePath7 = "/repo/data/images/mt_data/detectors/20221101_173454.jpg"
expectedNumbers7 = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1]
expectedColors7 = [
    Color.PURPLE,
    Color.GREEN,
    Color.PURPLE,
    Color.GREEN,
    Color.RED,
    Color.GREEN,
    Color.RED,
    Color.GREEN,
    Color.GREEN,
    Color.RED,
    Color.GREEN,
    Color.PURPLE,
]
expectedShapes7 = [
    Shape.OVAL,
    Shape.DIAMOND,
    Shape.DIAMOND,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.DIAMOND,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.OVAL,
    Shape.SQUIGGLE,
    Shape.SQUIGGLE,
]
expectedShading7 = [
    Shading.STRIPED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.SOLID,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.OUTLINED,
    Shading.STRIPED,
    Shading.SOLID,
    Shading.OUTLINED,
    Shading.STRIPED,
]


@pytest.mark.parametrize(
    "filePath,expectedNumbers,expectedColors,expectedShapes,expectedShading",
    [
        (
            filePath1,
            expectedNumbers1,
            expectedColors1,
            expectedShapes1,
            expectedShading1,
        ),
        (
            filePath2,
            expectedNumbers2,
            expectedColors2,
            expectedShapes2,
            expectedShading2,
        ),
        (
            filePath3,
            expectedNumbers3,
            expectedColors3,
            expectedShapes3,
            expectedShading3,
        ),
        (
            filePath4,
            expectedNumbers4,
            expectedColors4,
            expectedShapes4,
            expectedShading4,
        ),
        (
            filePath5,
            expectedNumbers5,
            expectedColors5,
            expectedShapes5,
            expectedShading5,
        ),
        (
            filePath6,
            expectedNumbers6,
            expectedColors6,
            expectedShapes6,
            expectedShading6,
        ),
        (
            filePath7,
            expectedNumbers7,
            expectedColors7,
            expectedShapes7,
            expectedShading7,
        ),
    ],
)
class TestDetectors:
    _colorDetector = DeepColorDetector()

    def test(
        self, filePath, expectedNumbers, expectedColors, expectedShapes, expectedShading
    ):
        numbers, shapes, shadings, colors = [], [], [], []
        cards = self.getCards(filePath)
        for cardImg in cards:
            cardPreprocessor = CardPreprocessor(cardImg)
            numbers.append(NumberDetector(cardPreprocessor).getNumber())
            shapes.append(ShapeDetector(cardPreprocessor).getShape())
            shadings.append(ShadingDetector(cardPreprocessor).getShading())
            colors.append(self._colorDetector.getColor(cardImg))

        assert numbers == expectedNumbers, filePath
        assert shapes == expectedShapes, filePath
        assert shadings == expectedShading, filePath
        assert colors == expectedColors, filePath

    def getCards(self, filePath):
        sourceImg = cv2.imread(filePath)
        sourceImg = set_finder.resizer.getResizedImageKeepRatio(sourceImg, width=800)

        cardDetector = set_finder.CardDetector(sourceImg)
        assert cardDetector.getNumberOfCards() == 12

        return cardDetector.getCardImages()
