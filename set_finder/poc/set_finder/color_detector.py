import numpy as np
import cv2

from cards import Color
from image_filter import ImageFilter

NOT_NEEDED = 0
GREEN_LOWER = np.array([25, NOT_NEEDED, 0])
GREEN_UPPER = np.array([110, 255, 255])

RED_LOWER = np.array([170, NOT_NEEDED, 0])
RED_UPPER = np.array([255, 255, 255])

PURPLE_LOWER = np.array([100, NOT_NEEDED, 0])
PURPLE_UPPER = np.array([150, 255, 255])

CANNY_TH1, CANNY_TH2 = 30, 200

UNKNOWN_COLOR_TH = 50


class ColorDetector:
    def __init__(self, card):
        preprocessedCard = self.preprocessCard(card)

        imageFilter = ImageFilter(preprocessedCard)

        self._greenCard = imageFilter.adaptiveColorDetectorFilter(
            GREEN_LOWER, GREEN_UPPER
        )
        self._redCard = imageFilter.adaptiveColorDetectorFilter(RED_LOWER, RED_UPPER)
        self._purpleCard = imageFilter.adaptiveColorDetectorFilter(
            PURPLE_LOWER, PURPLE_UPPER
        )

        greenContours = self._getContours(self._greenCard)
        redContours = self._getContours(self._redCard)
        purpleContours = self._getContours(self._purpleCard)

        totalGreenArcLength = self._getTotalArcLength(greenContours)
        totalRedArcLength = self._getTotalArcLength(redContours)
        totalPurpleArcLength = self._getTotalArcLength(purpleContours)

        colors = [
            (Color.UNKNOWN, UNKNOWN_COLOR_TH),
            (Color.GREEN, totalGreenArcLength),
            (Color.RED, totalRedArcLength),
            (Color.PURPLE, totalPurpleArcLength),
        ]

        self._color = max(colors, key=lambda el: el[1])[0]

    @staticmethod
    def preprocessCard(card):
        blurCardsImg = cv2.bilateralFilter(card, 9, 75, 75)
        kernel = np.ones((5, 5), np.uint8)
        erodeImg = cv2.erode(blurCardsImg, kernel, iterations=3)
        return erodeImg

    def getColor(self):
        return self._color

    def getDebugImages(self):
        return self._greenCard, self._redCard, self._purpleCard

    def _getContours(self, image):
        grayImg = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        edgedImg = cv2.Canny(grayImg, CANNY_TH1, CANNY_TH2)
        contours, _ = cv2.findContours(
            edgedImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        return contours

    def _getTotalArcLength(self, contours):
        return sum(
            cv2.arcLength(contour, False)
            if cv2.arcLength(contour, False) > UNKNOWN_COLOR_TH
            else 0
            for contour in contours
        )
