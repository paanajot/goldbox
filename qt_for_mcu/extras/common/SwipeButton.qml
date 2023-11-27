import QtQuick 2.0

Image {
    source: "assets/swipeButtonBG.png"
    clip: true

    signal swiped

    Flickable {
        id: flickArea
        width: parent.width
        height: parent.height
        contentWidth: width + swipeButton.width
        flickableDirection: Flickable.HorizontalFlick
        boundsBehavior: Flickable.StopAtBounds
        contentX: swipeButton.width
        onMovingHorizontallyChanged: {
            if (!movingHorizontally) {
                if (contentX == 0) {
                    swiped();
                }
                flickArea.contentX = swipeButton.width;
            }
        }

        Rectangle {
            id: swipeButton
            height: parent.height
            width: 280
            color: "#FFFFFF"
            opacity: 0.8
            radius: 52
            x: swipeButton.width
        }
    }
}
