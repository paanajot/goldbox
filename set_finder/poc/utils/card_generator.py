import cv2
import set_finder

from os import listdir


def generateCards(inputPath, outputPath, idxName=0):
    files = listdir(inputPath)

    images = []
    for fileName in files:
        orgImg = cv2.imread(inputPath + fileName)
        image = set_finder.resizer.getResizedImageKeepRatio(orgImg, width=800)
        images.append(image)

    for image in images:
        cardDetector = set_finder.CardDetector(image)

        for card in cardDetector.getCardImages():
            cv2.imwrite(outputPath + str(idxName) + ".jpg", card)
            idxName += 1


def generateColors():
    for folder in ["train", "test"]:
        for feature in ["red", "green", "purple"]:
            inputPath = f"/repo/data/images/color_detector/{folder}/{feature}_inputs/"
            outputPath = f"/repo/data/images/color_detector/{folder}/{feature}/"
            generateCards(inputPath, outputPath)


def generateShapes():
    for feature in ["diamond", "oval", "squiggle"]:
        inputPath = f"/repo/data/images/shape_detector/test/{feature}_inputs/"
        outputPath = f"/repo/data/images/shape_detector/test/{feature}/"
        generateCards(inputPath, outputPath)


def generateShadings():
    for feature in ["solid", "outlined", "striped"]:
        inputPath = f"/repo/data/images/shading_detector/test/{feature}_inputs/"
        outputPath = f"/repo/data/images/shading_detector/test/{feature}/"
        generateCards(inputPath, outputPath)


def generateNumbers():
    for feature in ["one", "two", "three"]:
        inputPath = f"/repo/data/images/number_detector/test/{feature}_inputs/"
        outputPath = f"/repo/data/images/number_detector/test/{feature}/"
        generateCards(inputPath, outputPath)


if __name__ == "__main__":
    generateShapes()
    generateShadings()
    generateNumbers()
    generateColors()
