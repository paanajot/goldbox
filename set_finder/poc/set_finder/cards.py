from enum import Enum

CARD_WIDTH, CARD_HEIGHT = 200, 133


class Color(Enum):
    UNKNOWN = 0
    RED = 1
    GREEN = 2
    PURPLE = 3


class Shape(Enum):
    UNKNOWN = 0
    OVAL = 1
    DIAMOND = 2
    SQUIGGLE = 3


class Shading(Enum):
    UNKNOWN = 0
    SOLID = 1
    STRIPED = 2
    OUTLINED = 3


class Card:
    def __init__(self, id, contour, color, number, shape, shading):
        self.id = id
        self.contour = contour
        self.color = color
        self.number = number
        self.shape = shape
        self.shading = shading


class Cards:
    def __init__(self):
        self._cards = []

    def appendCard(self, card):
        self._cards.append(card)

    def getCards(self):
        return self._cards
