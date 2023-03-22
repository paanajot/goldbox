import cv2

from debugger import Debugger
from set_manager import SetManager

from os import listdir

if __name__ == "__main__":
    debugger = Debugger()

    folderPath = "/repo/data/images/card_detector/tmp/"
    files = listdir(folderPath)

    debugImages = []
    setImages = []
    setManager = SetManager()
    for fileName in files:
        image = cv2.imread(folderPath + fileName)
        setManager.fullRun(image)
        # setManager.run(image, useThreads=False)

        try:
            debugImages.append(setManager.getDebugImage())
            setImages.append(setManager.getSetImage())
        except Exception as err:
            print("Oops!  Something went wrong.  Try again...")

    cv2.imshow("result with prints", debugger.getVerticalDebugImage([debugImages]))
    cv2.imshow("set images", debugger.getVerticalDebugImage([setImages]))

    cv2.waitKey(0)
    cv2.destroyAllWindows()
