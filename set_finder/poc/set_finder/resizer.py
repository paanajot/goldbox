import cv2


def getResizedImageKeepRatio(sourceImg, width=400):
    height = int(sourceImg.shape[0] * (width / sourceImg.shape[1]))
    dim = (width, height)

    return cv2.resize(sourceImg, dim, interpolation=cv2.INTER_AREA)


def getResizedImage(card, width=400, height=250):
    dim = (width, height)

    return cv2.resize(card, dim, interpolation=cv2.INTER_AREA)


def getResizedImageRatio(card, ratio=1):
    dim = (int(card.shape[1] * ratio), int(card.shape[0] * ratio))

    return cv2.resize(card, dim, interpolation=cv2.INTER_AREA)
