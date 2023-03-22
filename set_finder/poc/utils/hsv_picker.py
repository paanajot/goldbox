import cv2
import numpy as np

from enum import Enum
from os import listdir

import set_finder

from set_finder.color_detector import ColorDetector
from set_finder.card_detector import CardDetector
from set_finder.debugger import Debugger
from set_finder.image_filter import ImageFilter


class Mode(Enum):
    CARD_DETECTOR = 1
    COLOR_DETECTOR = 2


class HsvPicker:
    def __init__(self, mode, path="/repo/data/images/hsv_picker/"):
        self._folderPath = path
        self.generateImages()

        if mode == Mode.COLOR_DETECTOR:
            self.initColorDetector()
            debug = self.debugColorDetector
        else:
            debug = self.debugCardDetector

        cv2.namedWindow("hsv picker")

        cv2.createTrackbar("hue min", "hsv picker", 0, 179, debug)
        cv2.createTrackbar("hue max", "hsv picker", 179, 179, debug)
        cv2.createTrackbar("sat min", "hsv picker", 0, 255, debug)
        cv2.createTrackbar("sat max", "hsv picker", 255, 255, debug)
        cv2.createTrackbar("val min", "hsv picker", 0, 255, debug)
        cv2.createTrackbar("val max", "hsv picker", 255, 255, debug)

    def generateImages(self):
        files = listdir(self._folderPath)

        self._images = []
        for fileName in files:
            orgImg = cv2.imread(self._folderPath + fileName)
            image = set_finder.resizer.getResizedImageKeepRatio(orgImg, width=800)
            self._images.append(image)

    def getLowerAndUpper(self):
        h_min = cv2.getTrackbarPos("hue min", "hsv picker")
        h_max = cv2.getTrackbarPos("hue max", "hsv picker")
        s_min = cv2.getTrackbarPos("sat min", "hsv picker")
        s_max = cv2.getTrackbarPos("sat max", "hsv picker")
        v_min = cv2.getTrackbarPos("val min", "hsv picker")
        v_max = cv2.getTrackbarPos("val max", "hsv picker")

        # filter
        lower = np.array([h_min, s_min, v_min])
        upper = np.array([h_max, s_max, v_max])
        return lower, upper

    def debugCardDetector(self, arg):
        # NOTE: upper SMAX is calculated inside the CardDetector
        debugImages = []
        lower, upper = self.getLowerAndUpper()

        for image in self._images:
            cardDetector = CardDetector(
                image, lower, upper, adaptiveFilter=False, debugMode=True
            )
            debugImages.append(cardDetector.getDebugImages())

        debugImage = Debugger().getVerticalDebugImage(debugImages)
        cv2.imshow("hsv picker", debugImage)

    def initColorDetector(self):
        self._cardDetectors = []
        for image in self._images:
            self._cardDetectors.append(CardDetector(image))

    def debugColorDetector(self, arg):
        filteredCardsImages = []
        lower, upper = self.getLowerAndUpper()

        for cardDetector in self._cardDetectors:
            cards = cardDetector.getCards()
            if len(cards) != 0:
                cardsImg = Debugger().getCardsImg(cards)
                preprocessedImg = ColorDetector.preprocessCard(cardsImg)
                filteredCardsImages.append(
                    [cardsImg, ImageFilter(preprocessedImg).filter(lower, upper)]
                )

        if len(filteredCardsImages) != 0:
            filteredDebugImage = Debugger().getVerticalDebugImage(filteredCardsImages)
            cv2.imshow("hsv picker", filteredDebugImage)


if __name__ == "__main__":
    hsvPicker = HsvPicker(Mode.CARD_DETECTOR)

    while True:
        if cv2.waitKey(50) == 27:
            cv2.destroyAllWindows()
            break
