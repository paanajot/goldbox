import cv2
import math
import numpy as np

from enum import Enum
from image_filter import ImageFilter
from debugger import Debugger

GREEN_COLOR = (0, 255, 0)
THICKNESS = 5
ALL_CONTOURS_MODE = -1

CANNY_TH1, CANNY_TH2 = 30, 200

NOT_NEEDED = 0
LOWER = np.array([0, 0, NOT_NEEDED])
UPPER = np.array([179, NOT_NEEDED, 255])


CARD_WIDTH, CARD_HEIGHT = 200, 133
CONTOUR_AREA_TH = 3000
MARGINS = 5


class Status(Enum):
    SUCCESS = 0
    PARTIAL_SUCCESS = 1
    INFEASIBLE_PROBLEM = 2


class InternalStatus(Enum):
    SUCCESS = 0
    SOLUTION_IMPROVED = 1
    FAIL = 2
    COMPLETE_FAIL = 3


class FilterOptimizer:
    def __init__(self, imageFilter, lower=None, upper=None):
        self._imageFilter = imageFilter
        self._lower = lower.copy() if lower is not None else LOWER
        self._upper = upper.copy() if upper is not None else UPPER
        self._optimizedLower = self._lower
        self._optimizedUpper = self._upper

    def run(self, solver):
        self._status = Status.INFEASIBLE_PROBLEM
        internalStatus = InternalStatus.FAIL
        vMin = 120
        vMinIterations = 5
        while vMinIterations != 0 and internalStatus != InternalStatus.SUCCESS:
            self._lower[2] = vMin
            sMax = int(40 + 2.5 * self._imageFilter.getSMean())
            # sMax = 175
            sMaxIterations = 20
            while sMaxIterations != 0 and internalStatus != InternalStatus.SUCCESS:
                self._upper[1] = sMax
                filteredImg = self._imageFilter.filter(self._lower, self._upper)

                internalStatus = solver(filteredImg)
                isSuccess = InternalStatus.SUCCESS == internalStatus
                isImproved = InternalStatus.SOLUTION_IMPROVED == internalStatus
                if isSuccess or isImproved:
                    self._optimizedLower = self._lower.copy()
                    self._optimizedUpper = self._upper.copy()
                    self._status = Status.PARTIAL_SUCCESS
                elif InternalStatus.COMPLETE_FAIL == internalStatus:
                    break

                sMax -= int(0.12 * sMax + 2)
                # sMax -= 0.12 * sMax
                sMaxIterations -= 1

            vMin += 15
            vMinIterations -= 1

        if internalStatus == InternalStatus.SUCCESS:
            self._status = Status.SUCCESS

        return self._status

    def getSolution(self):
        return self._optimizedLower, self._optimizedUpper


"""
Steps to detect the cards:
1. Filter out the background and noises
    * blur the image applying gaussian filter
    * change to HSV format
    * create a mask
    * aply the mask onto the original image
2. Find the contours of the cards
    * change filtered image to gray format
    * use canny to get image with edges
    * find external contours
3. Get corners of the cards
    * create convex hull from a single contour
    * use goodFeaturesToTrack to get the corners
4. Warp the cards
    * order corners
    * create perspective transform matrix
    * use warpPerspective to warp the card
    * calculate and return center of the card (will be helpfull to order the cards)
    * crop the margins
5. Order cards to keep original positions
    * order the cards wrt y axis
    * order the small groups of the cards wrt x axis
"""


class CardContoursGenerator:
    def __init__(self):
        self._cornersWithContour = []
        self._minAllContours = 1000

    def runAlgorithm(self, filteredImg):
        contours = self._findContours(filteredImg)
        allContours = len(contours)

        if allContours > 30:
            return InternalStatus.FAIL

        if allContours < 3:
            return InternalStatus.COMPLETE_FAIL

        fourCornersWithContour = []
        for contour in contours:
            if cv2.contourArea(contour) > CONTOUR_AREA_TH:
                # 3.
                corners = self._getCorners(filteredImg, contour)
                # we need to find four contours to warp the card
                if len(corners) == 4:
                    fourCornersWithContour.append((corners, contour))
                else:
                    return InternalStatus.FAIL

        # 5.
        # TODO: describe it
        firstCondition = len(fourCornersWithContour) > len(self._cornersWithContour)
        secondCondition = len(fourCornersWithContour) == len(self._cornersWithContour)
        thirdCondition = allContours < self._minAllContours

        status = InternalStatus.FAIL
        if firstCondition or (secondCondition and thirdCondition):
            self._cornersWithContour = fourCornersWithContour
            self._minAllContours = min(self._minAllContours, allContours)
            status = InternalStatus.SOLUTION_IMPROVED

        if allContours == len(fourCornersWithContour):
            status = InternalStatus.SUCCESS

        return status

    def getCornersWithContour(self):
        return self._cornersWithContour

    def _findContours(self, filteredImg):
        grayImg = cv2.cvtColor(filteredImg, cv2.COLOR_BGR2GRAY)
        edgedImg = cv2.Canny(grayImg, CANNY_TH1, CANNY_TH2)
        contours, _ = cv2.findContours(
            edgedImg, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE
        )
        return contours

    def _getCorners(self, filteredImg, contour):
        # create convex full from contour
        hull = cv2.convexHull(contour)

        # Create image for good features to track:
        (height, width) = filteredImg.shape[:2]
        # Black image same size as original input:
        hullImg = np.zeros((height, width), dtype=np.uint8)
        cv2.drawContours(hullImg, [hull], 0, 255, 2)

        # Set the corner detection:
        maxCorners = 4
        qualityLevel = 0.01
        minDistance = 50

        # Get the corners:
        corners = cv2.goodFeaturesToTrack(
            hullImg, maxCorners, qualityLevel, minDistance
        )

        return corners


class CardDetector:
    def __init__(
        self, sourceImg, lower=None, upper=None, adaptiveFilter=True, debugMode=False
    ):
        self._contours = []
        self._debugMode = debugMode
        self._adaptiveFilter = adaptiveFilter
        self._cards = []
        self._cardsWithContours = []

        self._lower = lower.copy() if lower is not None else LOWER
        self._upper = upper.copy() if upper is not None else UPPER

        self._sourceImg = sourceImg
        imageFilter = ImageFilter(sourceImg)

        contoursGenerator = CardContoursGenerator()
        # TODO: improve performance !!!!!
        if self._adaptiveFilter:
            optimizer = FilterOptimizer(imageFilter)
            optimizer.run(contoursGenerator.runAlgorithm)
            self._lower, self._upper = optimizer.getSolution()
            cornersWithContour = contoursGenerator.getCornersWithContour()
        else:
            self._filteredImg = imageFilter.filter(self._lower, self._upper)
            contoursGenerator.runAlgorithm(self._filteredImg)
            cornersWithContour = contoursGenerator.getCornersWithContour()

        # 4.
        unorderedCards = []
        for corners, contour in cornersWithContour:
            warpedCard, center = self._getWarpedCardWithCenter(corners)
            croppedCard = warpedCard[MARGINS:-MARGINS, MARGINS:-MARGINS, :]
            unorderedCards.append((croppedCard, center, contour))

            if self._debugMode:
                for c in corners:
                    (x, y) = c.ravel()
                    cv2.circle(self._edgedImg, (int(x), int(y)), 5, 255, 5)

        if unorderedCards:
            self._cardsWithContours = self._orderCards(unorderedCards)
            self._cards = list(zip(*self._cardsWithContours))[0]

    def getCardImages(self):
        return self._cards

    def getCardImagesWithContours(self):
        return self._cardsWithContours

    def getNumberOfCards(self):
        return len(self._cards)

    def getDebugImages(self):
        debugger = Debugger()
        self._contoursImg = self._filteredImg.copy()
        cv2.drawContours(
            self._contoursImg,
            self._contours,
            ALL_CONTOURS_MODE,
            GREEN_COLOR,
            THICKNESS,
        )

        outputImage = self._contoursImg
        if len(self._cards) != 0:
            outputImage = debugger.getCardsImg(self._cards)

        statsImg = debugger.generateStats(
            self._sourceImg.shape[0],
            self._sourceImg.shape[1],
            [
                str(self._lower),
                str(self._upper),
                str(len(self._contours)),
                str(len(self._cornersWithContour)),
            ],
        )
        return (
            self._sourceImg,
            self._contoursImg,
            self._edgedImg,
            outputImage,
            statsImg,
        )

    def _orderCorners(self, corners):
        # initialzie a list of coordinates that will be ordered
        # such that the first entry in the list is the top-left,
        # the second entry is the top-right, the third is the
        # bottom-right, and the fourth is the bottom-left
        reshaped_corners = np.reshape(corners, (4, 2))
        ordered_corners = np.zeros((4, 2), dtype="float32")

        # the top-left point will have the smallest sum, whereas
        # the bottom-right point will have the largest sum
        s = reshaped_corners.sum(axis=1)
        ordered_corners[0] = reshaped_corners[np.argmin(s)]
        ordered_corners[2] = reshaped_corners[np.argmax(s)]

        # now, compute the difference between the points, the
        # top-right point will have the smallest difference,
        # whereas the bottom-left will have the largest difference
        diff = np.diff(reshaped_corners, axis=1)
        ordered_corners[1] = corners[np.argmin(diff)]
        ordered_corners[3] = corners[np.argmax(diff)]

        return ordered_corners

    def _orderCards(self, cards):
        # Multiply by 5 to group cards in rows
        cards.sort(key=lambda el: el[1][1] * 5 + el[1][0])
        return [(el[0], el[2]) for el in cards]

    def _getWarpedCardWithCenter(self, corners):

        pts1 = self._orderCorners(corners)
        firstSide = math.dist(pts1[0], pts1[1])
        secondSide = math.dist(pts1[0], pts1[3])
        if firstSide > secondSide:
            pts2 = np.float32(
                [
                    [0, 0],
                    [CARD_WIDTH, 0],
                    [CARD_WIDTH, CARD_HEIGHT],
                    [0, CARD_HEIGHT],
                ]
            )
        else:
            pts2 = np.float32(
                [
                    [CARD_WIDTH, 0],
                    [CARD_WIDTH, CARD_HEIGHT],
                    [0, CARD_HEIGHT],
                    [0, 0],
                ]
            )

        matrix = cv2.getPerspectiveTransform(pts1, pts2)

        warpedCard = cv2.warpPerspective(
            self._sourceImg, matrix, (CARD_WIDTH, CARD_HEIGHT)
        )
        center = pts1.sum(axis=0) / 4

        return warpedCard, center
