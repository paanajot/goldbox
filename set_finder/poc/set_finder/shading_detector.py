import cv2
import numpy as np
from cards import Shading
from matplotlib import pyplot as plt

CANNY_TH1, CANNY_TH2 = 40, 120

GREEN_COLOR = (0, 255, 0)
THICKNESS = 2
ALL_CONTOURS_MODE = -1

class ShadingDetector:
    def __init__(self, cardPreprocessor):
        self._shading = Shading.UNKNOWN

        erodeImg = cardPreprocessor.getErodeImg()
        contours = cardPreprocessor.getContours()

        erodeImg = cv2.bilateralFilter(erodeImg, 7, 40, 40)

        grayImg = cv2.cvtColor(erodeImg, cv2.COLOR_BGR2GRAY)
        x, y, w, h = cv2.boundingRect(contours[0])
        roi = grayImg[y-1: y+1 + h, x-1 : x+1 + w]

        canny_roi = cv2.Canny(roi, CANNY_TH1, CANNY_TH2)
        roi_contours, _ = cv2.findContours(
            canny_roi, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE
        )

        # print(len(roi_contours))

        # cv2.drawContours(
        #     roi,
        #     roi_contours,
        #     ALL_CONTOURS_MODE,
        #     GREEN_COLOR,
        #     THICKNESS,
        # )

        # cv2.imshow("roi", roi)
        # while True:
        #     if cv2.waitKey(50) == 27:
        #         cv2.destroyAllWindows()
        #         break

        # plt.plot(hist, color="b")
        # plt.xlim([0, 64])
        # plt.show()

        # TODO: filter contours
        # for c in roi_contours:
        #     print(cv2.arcLength(c, closed=True))

        hist = cv2.calcHist([roi], [0], None, [64], [0, 256])

        reversedHist = hist[::-1]

        left_x = np.argmax(hist >= 100)
        left_x_2 = left_x + np.argmax(hist[left_x:] <= 100)

        right_x = np.argmax(reversedHist >= 100)
        right_x_2 = right_x + np.argmax(reversedHist[right_x:] <= 100)

        left_sum = sum(hist[left_x - 2 : left_x_2 + 2])
        right_sum = sum(reversedHist[right_x - 2 : right_x_2 + 2])

        contour_area = cv2.contourArea(contours[0])
        outer_pixels = (w+2)*(h+2) - contour_area

        if len(roi_contours) <= 2: # left_sum >= 0.75*abs(contour_area): # works with 0.6 - 0.9
            self._shading = Shading.SOLID
        elif (right_sum-outer_pixels) >= 0.40*(abs(contour_area)-left_sum): # works with 0.3 - 0.5
            self._shading = Shading.OUTLINED
        else:
            self._shading = Shading.STRIPED


    def getShading(self):
        return self._shading
