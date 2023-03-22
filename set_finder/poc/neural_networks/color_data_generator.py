import cv2
import numpy as np

from os import listdir


class ColorDataGenerator:
    def __init__(self, augmentation):
        self._trainImages = []
        self._trainLabels = []

        self._testImages = []
        self._testLabels = []

        greenInputPath = "/repo/data/images/color_detector/train/green/"
        greenLabel = 0

        redInputPath = "/repo/data/images/color_detector/train/red/"
        redLabel = 1

        purpleInputPath = "/repo/data/images/color_detector/train/purple/"
        purpleLabel = 2

        self._addTrainData(greenInputPath, greenLabel, augmentation)
        self._addTrainData(redInputPath, redLabel, augmentation)
        self._addTrainData(purpleInputPath, purpleLabel, augmentation)

        greenInputPath = "/repo/data/images/color_detector/test/green/"
        redInputPath = "/repo/data/images/color_detector/test/red/"
        purpleInputPath = "/repo/data/images/color_detector/test/purple/"

        self._addTestData(greenInputPath, greenLabel)
        self._addTestData(redInputPath, redLabel)
        self._addTestData(purpleInputPath, purpleLabel)

    def getTrainData(self):
        return (np.asarray(self._trainImages), np.asarray(self._trainLabels))

    def getTestData(self):
        return (np.asarray(self._testImages), np.asarray(self._testLabels))

    def getInputShape(self):
        return self._trainImages[0].shape

    def _addTrainData(self, inputPath, label, augmentation):
        files = listdir(inputPath)
        for fileName in files:
            orgImg = cv2.imread(inputPath + fileName)
            self._trainImages.append(orgImg)
            self._trainLabels.append(label)

            if augmentation:
                hlsImg = cv2.cvtColor(orgImg, cv2.COLOR_BGR2HLS)

                lighterImg = hlsImg.copy()
                lighterImg[:, :, 1] += 30  # todo up to 255
                lighterImg = cv2.cvtColor(lighterImg, cv2.COLOR_HLS2BGR)
                self._trainImages.append(lighterImg)
                self._trainLabels.append(label)

                darkerImg = hlsImg.copy()
                darkerImg[:, :, 1] -= 30  # todo min 0
                darkerImg = cv2.cvtColor(darkerImg, cv2.COLOR_HLS2BGR)
                self._trainImages.append(darkerImg)
                self._trainLabels.append(label)

    def _addTestData(self, inputPath, label):
        files = listdir(inputPath)
        for fileName in files:
            orgImg = cv2.imread(inputPath + fileName)
            self._testImages.append(orgImg)
            self._testLabels.append(label)
