import cv2
import resizer
import math
import numpy as np

from cards import CARD_WIDTH, CARD_HEIGHT


class Debugger:
    def getCardsImg(self, detectedCards, analyzedCards=None):
        if not detectedCards:
            raise Exception("detectedCards list is empty")

        num_of_cards = len(detectedCards)
        num_of_card_in_row = 3
        total_width = (
            num_of_card_in_row * CARD_WIDTH
            if num_of_cards >= num_of_card_in_row
            else num_of_cards * CARD_WIDTH
        )
        total_height = math.ceil(num_of_cards / num_of_card_in_row) * CARD_HEIGHT
        final_image = np.zeros((total_height, total_width, 3), dtype=np.uint8)

        current_y = 0
        current_x = 0
        for idx, card in enumerate(detectedCards):
            card = resizer.getResizedImage(card, CARD_WIDTH, CARD_HEIGHT)
            final_image[
                current_y : card.shape[0] + current_y,
                current_x : card.shape[1] + current_x,
                :,
            ] = card

            if analyzedCards:
                debugText = (
                    str(analyzedCards[idx].number)
                    + ", "
                    + analyzedCards[idx].color.name[:3]
                    + ", "
                    + analyzedCards[idx].shape.name[:3]
                    + ", "
                    + analyzedCards[idx].shading.name[:3]
                )
                cv2.putText(
                    final_image,
                    debugText,
                    (current_x, current_y + 14),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    fontScale=0.6,
                    color=(0, 0, 0),
                    thickness=1,
                )

            current_x += card.shape[1]
            if (idx + 1) % num_of_card_in_row == 0:
                current_y += card.shape[0]
                current_x = 0

        return final_image

    def getCardsInRowImg(self, cards, num_of_card_in_row=12):
        num_of_cards = len(cards)
        total_width = (
            num_of_card_in_row * CARD_WIDTH
            if num_of_cards >= num_of_card_in_row
            else num_of_cards * CARD_WIDTH
        )
        total_height = math.ceil(num_of_cards / num_of_card_in_row) * CARD_HEIGHT
        final_image = np.zeros((total_height, total_width, 3), dtype=np.uint8)

        current_y = 0
        current_x = 0
        for idx, card in enumerate(cards):
            card = resizer.getResizedImage(card, CARD_WIDTH, CARD_HEIGHT)
            final_image[
                current_y : card.shape[0] + current_y,
                current_x : card.shape[1] + current_x,
                :,
            ] = card

            current_x += card.shape[1]
            if (idx + 1) % num_of_card_in_row == 0:
                current_y += card.shape[0]
                current_x = 0

        return resizer.getResizedImageRatio(final_image, ratio=0.5)

    def getDebugImage(self, images):
        debugImages = []
        for image in images:
            image = resizer.getResizedImage(image, width=350)
            if len(image.shape) < 3:
                image = cv2.cvtColor(image, cv2.COLOR_GRAY2BGR)

            debugImages.append(image)

        return np.hstack(debugImages)

    def getVerticalDebugImage(self, images):
        debugImages = []
        for himages in images:
            debugImages.append(self.getDebugImage(himages))

        return np.vstack(debugImages)

    def generateStats(self, width, height, stats):
        image = np.zeros((height, width, 3), dtype=np.uint8)
        yOffset = int(height / len(stats))
        for idx, stat in enumerate(stats):
            cv2.putText(
                image,
                stat,
                (int(width / 20), idx * yOffset + 60),
                cv2.FONT_HERSHEY_SIMPLEX,
                fontScale=2,
                color=(0, 255, 0),
                thickness=2,
                bottomLeftOrigin=False,
            )
        return image
