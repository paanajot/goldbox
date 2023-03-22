import keras
import matplotlib.pyplot as plt

from keras import layers, models
from keras.callbacks import EarlyStopping
from color_data_generator import ColorDataGenerator


class ColorDetectorCnn:
    def __init__(self):
        self._colorDataGenerator = ColorDataGenerator(augmentation=True)
        (
            self._train_images,
            self._train_labels,
        ) = self._colorDataGenerator.getTrainData()

        imgShape = self._colorDataGenerator.getInputShape()
        self._model = self._getModel(imgShape)

    def _getModel(self, imgShape):
        model = models.Sequential()
        model.add(layers.BatchNormalization(input_shape=imgShape))
        model.add(layers.Conv2D(16, (3, 3), activation="relu"))
        # model.add(layers.BatchNormalization())
        model.add(layers.MaxPooling2D((2, 2)))
        # model.add(layers.Dropout(0.25))

        model.add(layers.Conv2D(32, (3, 3), activation="relu"))
        # model.add(layers.BatchNormalization())
        model.add(layers.MaxPooling2D((2, 2)))
        # model.add(layers.Dropout(0.25))

        model.add(layers.Flatten())
        model.add(layers.Dense(21, activation="relu"))
        model.add(layers.Dense(3, activation="softmax"))

        model.compile(
            optimizer="adam",
            loss=keras.losses.SparseCategoricalCrossentropy(from_logits=True),
            metrics=["accuracy"],
        )

        return model

    def train(self):
        early_stopping = EarlyStopping(
            monitor="val_accuracy",
            min_delta=0.001,  # minimium amount of change to count as an improvement
            patience=10,  # how many epochs to wait before stopping
            restore_best_weights=True,
        )

        self._history = self._model.fit(
            self._train_images,
            self._train_labels,
            batch_size=64,
            epochs=25,
            validation_split=0.2,
            callbacks=[early_stopping],
        )

    def test(self):
        test_images, test_labels = self._colorDataGenerator.getTestData()
        test_loss, test_acc = self._model.evaluate(test_images, test_labels, verbose=2)
        print(test_loss, test_acc)

    def save(self):
        self._model.save("/repo/data/models/color_detector.h5")

    def plotTestData(self):
        plt.figure(figsize=(10, 10))
        for i in range(25):
            plt.subplot(5, 5, i + 1)
            plt.xticks([])
            plt.yticks([])
            plt.grid(False)
            plt.imshow(self._train_images[i])
            plt.xlabel(self._train_labels[i])

        plt.show()

    def plotTrainingHistory(self):
        plt.plot(self._history.history["accuracy"], label="accuracy")
        plt.plot(self._history.history["val_accuracy"], label="val_accuracy")
        plt.xlabel("Epoch")
        plt.ylabel("Accuracy")
        plt.ylim([0.5, 1])
        plt.legend(loc="lower right")
        plt.show()


if __name__ == "__main__":
    cnn = ColorDetectorCnn()
    cnn.train()
    cnn.plotTrainingHistory()
    cnn.test()
    cnn.save()
