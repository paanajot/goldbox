import cv2
import numpy as np


class ImageFilter:
    def __init__(self, sourceImg):
        sourceImg = cv2.GaussianBlur(sourceImg, (7, 7), 0)
        self._sourceImg = sourceImg
        self._hsvImg = cv2.cvtColor(sourceImg, cv2.COLOR_BGR2HSV)

        nopixels = int(self._hsvImg.shape[0] * self._hsvImg.shape[1] * 0.20)
        sChannel = self._hsvImg[:, :, 1]
        self._sMean = int(cv2.mean(np.sort(sChannel.flatten())[:nopixels])[0])

        vChannel = self._hsvImg[:, :, 2]
        self._vMean = int(cv2.mean(np.sort(vChannel.flatten())[:nopixels])[0])

        sHistr = cv2.calcHist([self._hsvImg], [1], None, [256], [0, 256])
        self._sPeak = np.argmax(sHistr)

        vHistr = cv2.calcHist([self._hsvImg], [2], None, [256], [0, 256])
        self._vPeak = np.argmax(vHistr)

    def getSMean(self):
        return self._sMean

    def filter(self, lower, upper):
        mask = cv2.inRange(self._hsvImg, lower, upper)
        return cv2.bitwise_and(self._sourceImg, self._sourceImg, mask=mask)

    def adaptiveColorDetectorFilter(self, lower, upper):
        sMinOffset = self._sPeak + 10

        adaptedUpper = upper.copy()
        adaptedLower = lower.copy()

        adaptedLower[1] += sMinOffset

        mask = cv2.inRange(self._hsvImg, adaptedLower, adaptedUpper)
        return cv2.bitwise_and(self._sourceImg, self._sourceImg, mask=mask)
