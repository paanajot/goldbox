from threading import Thread
import cv2
import time

directory = "/repo/data/images/captured/"


class VideoManager:
    def __init__(self, src=0):
        self.stream = cv2.VideoCapture(src)
        if not self.stream.isOpened():
            print("Cannot open camera")
            exit()

        (self._grabbed, self._frame) = self.stream.read()
        self._outputFrame = self._frame
        self._stopped = False

    def start(self):
        Thread(target=self._get, args=()).start()
        Thread(target=self._show, args=()).start()
        return self

    def isStopped(self):
        return self._stopped

    def getFrame(self):
        return self._frame

    def showFrame(self, frame):
        self._outputFrame = frame

    def _show(self):
        while not self._stopped:
            cv2.imshow("Video", self._outputFrame)

            key = cv2.waitKey(1)
            if key == ord("q"):
                self._stopped = True
            elif key == ord("s"):
                timeStr = time.strftime("%Y%m%d-%H%M%S")
                cv2.imwrite(directory + timeStr + ".jpg", self._frame)

        cv2.destroyAllWindows()

    def _get(self):
        while not self._stopped:
            if not self._grabbed:
                self._stopped = True
            else:
                (self._grabbed, self._frame) = self.stream.read()
