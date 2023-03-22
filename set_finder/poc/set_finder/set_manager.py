import resizer
import cv2

from enum import Enum
from threading import Thread

from debugger import Debugger
from cards import Card, Cards
from deep_color_detector import DeepColorDetector
from card_detector import CardDetector, FilterOptimizer, CardContoursGenerator, Status
from number_detector import NumberDetector
from shape_detector import ShapeDetector
from shading_detector import ShadingDetector
from card_preprocessor import CardPreprocessor
from set_finder import SetFinder
from image_filter import ImageFilter


deepColorDetector = DeepColorDetector()

GREEN_COLOR = (0, 255, 0)
THICKNESS = 5
ALL_CONTOURS_MODE = -1


class Processor(Enum):
    OPTIMIZE = 1
    GENERATE_SET = 2
    UPDATE_CONTOURS = 3


class SetManager:
    def __init__(self):
        self._detectedCardsLength = 0
        self._lower, self._upper = None, None

        self._solutionReady = False
        self._isOptimizerRunning = False
        self._isGeneratorRunning = False
        self._processor = Processor.OPTIMIZE

        self._sets = []

    def run(self, image, useThreads=True):
        self._resizedImg = self._resizeImage(image)
        if not self._isOptimizerRunning and self._processor == Processor.OPTIMIZE:
            print("optimize filter")
            self._isOptimizerRunning = True

            if useThreads:
                Thread(target=self._adjustFilter, args=()).start()
            else:
                self._adjustFilter()

        if not self._isGeneratorRunning and self._processor == Processor.GENERATE_SET:
            print("generate set")
            self._isGeneratorRunning = True

            if useThreads:
                Thread(target=self._generateSets, args=()).start()
            else:
                self._generateSets()

        if self._processor == Processor.UPDATE_CONTOURS:
            print("update set")
            self._updateContours()

    def fullRun(self, image):
        self._resizedImg = self._resizeImage(image)
        self._generateSets(adaptiveFilter=True)
        self._solutionReady = True

    def _adjustFilter(self):
        imageFilter = ImageFilter(self._resizedImg)
        contoursGenerator = CardContoursGenerator()
        optimizer = FilterOptimizer(imageFilter)
        optimizerStatus = optimizer.run(contoursGenerator.runAlgorithm)
        if optimizerStatus != Status.INFEASIBLE_PROBLEM:
            self._lower, self._upper = optimizer.getSolution()
            self._processor = Processor.GENERATE_SET

        self._isOptimizerRunning = False

    def _generateSets(self, adaptiveFilter=False):
        localLower, localUpper = self._lower, self._upper

        cardDetector = CardDetector(
            self._resizedImg, localLower, localUpper, adaptiveFilter=adaptiveFilter
        )
        self._cards = Cards()
        self._cardImages = cardDetector.getCardImages()
        cardImagesWithContours = cardDetector.getCardImagesWithContours()
        self._detectedCardsLength = len(cardImagesWithContours)

        for id, (cardImg, contour) in enumerate(cardImagesWithContours):
            cardPreprocessor = CardPreprocessor(cardImg)
            if not cardPreprocessor.getContours():
                break
            numberDetector = NumberDetector(cardPreprocessor)
            shapeDetector = ShapeDetector(cardPreprocessor)
            shadingDetector = ShadingDetector(cardPreprocessor)
            color = deepColorDetector.getColor(cardImg)
            number = numberDetector.getNumber()
            shape = shapeDetector.getShape()
            shading = shadingDetector.getShading()
            card = Card(id, contour, color, number, shape, shading)
            self._cards.appendCard(card)

        # TODO: _sets should be set in updateContours
        self._sets = SetFinder(self._cards).getSets()
        if self._sets:
            self._processor = Processor.UPDATE_CONTOURS
        else:
            self._processor = Processor.OPTIMIZE

        self._isGeneratorRunning = False

    def _updateContours(self):
        localLower, localUpper = self._lower, self._upper

        cardDetector = CardDetector(
            self._resizedImg, localLower, localUpper, adaptiveFilter=False
        )
        self._cards = Cards()
        self._cardImages = cardDetector.getCardImages()
        cardImagesWithContours = cardDetector.getCardImagesWithContours()

        if len(cardImagesWithContours) == self._detectedCardsLength:
            for oneCard in self._sets[0]:
                oneCard.contour = cardImagesWithContours[oneCard.id][1]
            self._solutionReady = True
        else:
            self._solutionReady = False
            self._sets = []
            self._processor = Processor.GENERATE_SET

    def getSets(self):
        return self._sets

    def getSetImage(self):
        setImage = self._resizedImg.copy()
        if self._sets and self._solutionReady:
            contours = [card.contour for card in self._sets[0]]
            cv2.drawContours(
                setImage,
                contours,
                ALL_CONTOURS_MODE,
                GREEN_COLOR,
                THICKNESS,
            )
        return setImage

    def getDebugImage(self):
        return Debugger().getCardsImg(self._cardImages, self._cards.getCards())

    def _resizeImage(self, image):
        return resizer.getResizedImageKeepRatio(image, width=800)
