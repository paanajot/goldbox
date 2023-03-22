from set_finder.set_manager import SetManager
from video_manager import VideoManager


videoManager = VideoManager(4).start()
setManager = SetManager()


while True:
    frame = videoManager.getFrame()

    setManager.run(frame)

    try:
        setFrame = setManager.getSetImage()
    except Exception as err:
        print("Oops!  Something went wrong.  Try again...")

    videoManager.showFrame(setFrame)

    if videoManager.isStopped():
        break
