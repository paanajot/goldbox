class NumberDetector:
    def __init__(self, cardPreprocessor):
        contours = cardPreprocessor.getContours()
        self._number = len(contours)

    def getNumber(self):
        return self._number
