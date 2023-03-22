import cv2
import numpy as np

CANNY_TH1, CANNY_TH2 = 40, 120
CONTOUR_AREA_TH = 200

GREEN_COLOR = (0, 255, 0)
THICKNESS = 2
ALL_CONTOURS_MODE = -1


class CardPreprocessor:
    def __init__(self, card):
        self._card = card
        self._clearedCard = self._removeNoises(card)
        self._contours = self._generateContours(self._clearedCard)

    def getContours(self):
        return self._contours

    def getCardImg(self):
        return self._card

    def getErodeImg(self):
        return self._clearedCard

    def getDebugImage(self):
        contoursImg = self._clearedCard.copy()
        if len(self._contours) != 0:
            cv2.drawContours(
                contoursImg,
                self._contours,
                ALL_CONTOURS_MODE,
                GREEN_COLOR,
                THICKNESS,
            )
        return contoursImg

    def _removeNoises(self, image):
        bilateralCard = cv2.bilateralFilter(image, 7, 40, 40)
        kernel = np.ones((5, 5), np.uint8)
        erodeImg = cv2.erode(bilateralCard, kernel, iterations=1)
        return erodeImg

    def _generateContours(self, image):
        grayImg = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
        edgedImg = cv2.Canny(grayImg, CANNY_TH1, CANNY_TH2)
        contours, _ = cv2.findContours(
            edgedImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        filtereContours = [
            c for c in contours if cv2.arcLength(c, closed=True) > CONTOUR_AREA_TH
        ]
        return filtereContours
