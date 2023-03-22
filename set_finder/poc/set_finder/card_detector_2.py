import cv2
import numpy as np

CANNY_TH1, CANNY_TH2 = 50, 200

GREEN_COLOR = (0, 255, 0)
THICKNESS = 5
ALL_CONTOURS_MODE = -1


class CardDetector2:
    def __init__(self, sourceImg):
        self._sourceImg = sourceImg
        self._grayImg = cv2.cvtColor(self._sourceImg, cv2.COLOR_BGR2GRAY)

        self._blurImg = cv2.GaussianBlur(self._grayImg, (5, 5), 0)
        # self._blurImg = cv2.bilateralFilter(self._grayImg, 9, 75, 75)
        # self._blurImg = cv2.medianBlur(self._grayImg, 5)
        # self._blurImg = cv2.blur(self._grayImg, (5, 5))

        _, self._binaryImg = cv2.threshold(
            self._blurImg, 0, 255, cv2.THRESH_BINARY + cv2.THRESH_OTSU
        )

        kernel = np.ones((5, 5), np.uint8)
        self._openingImg = cv2.morphologyEx(
            self._binaryImg, cv2.MORPH_OPEN, kernel, iterations=1
        )

        self._edgedImg = cv2.Canny(self._openingImg, CANNY_TH1, CANNY_TH2)
        contours, _ = cv2.findContours(
            self._edgedImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )

        self._contoursImg = self._sourceImg.copy()
        cv2.drawContours(
            self._contoursImg,
            contours,
            ALL_CONTOURS_MODE,
            GREEN_COLOR,
            THICKNESS,
        )

        # _, self._binaryImg = cv2.threshold(self._blurImg, 150, 255, cv2.THRESH_BINARY)

        # self._binaryImg = cv2.adaptiveThreshold(
        #     self._blurImg,
        #     255,
        #     cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
        #     cv2.THRESH_BINARY,
        #     7,
        #     2,
        # )

        self._cards = []

    def getCards(self):
        return self._cards

    def getNumberOfCards(self):
        return len(self._cards)

    def getDebugImages(self):
        return [
            self._sourceImg,
            self._grayImg,
            self._binaryImg,
            self._openingImg,
            self._contoursImg,
        ]
