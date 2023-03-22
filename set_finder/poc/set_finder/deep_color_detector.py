import keras

import numpy as np
from cards import Color


class DeepColorDetector:
    def __init__(self):
        self._model = keras.models.load_model("/repo/data/models/color_detector.h5")

    def getColor(self, card):
        reshapedCard = np.expand_dims(card, axis=0)

        prediction = self._model.predict(
            reshapedCard, verbose=0, workers=1, use_multiprocessing=False
        )
        greenLabel = 0
        redLabel = 1
        purpleLabel = 2

        switcher = {
            greenLabel: Color.GREEN,
            redLabel: Color.RED,
            purpleLabel: Color.PURPLE,
        }

        return switcher.get(np.argmax(prediction), Color.UNKNOWN)
