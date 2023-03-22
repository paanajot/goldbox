import itertools  # TODO: learn more


class SetFinder:
    def __init__(self, cards):
        self._setList = []
        combination = list(itertools.combinations(cards.getCards(), 3))
        for threeCards in combination:
            isSet = self.findSet(*threeCards)
            if isSet:
                self._setList.append(threeCards)

    def getSets(self):
        return self._setList

    def findSet(self, card1, card2, card3):
        shape = (card1.shape.value + card2.shape.value + card3.shape.value) % 3 == 0
        color = (card1.color.value + card2.color.value + card3.color.value) % 3 == 0
        number = (card1.number + card2.number + card3.number) % 3 == 0
        shading = (
            card1.shading.value + card2.shading.value + card3.shading.value
        ) % 3 == 0
        return shape and color and shading and number
