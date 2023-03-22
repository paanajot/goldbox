import cv2

from cards import Shape


class ShapeDetector:
    def __init__(self, cardPreprocessor):
        self._shape = Shape.UNKNOWN
        contours = cardPreprocessor.getContours()

        biggestContour = max(contours, key=abs and cv2.contourArea)
        contourArea = abs(cv2.contourArea(biggestContour, True))
        _, _, w, h = cv2.boundingRect(biggestContour)

        diffArea = w * h - contourArea
        if diffArea >= 1800:
            self._shape = Shape.DIAMOND
        elif contourArea >= 3600 and diffArea <= 1000:
            self._shape = Shape.OVAL
        else:
            self._shape = Shape.SQUIGGLE

    def getShape(self):
        return self._shape
