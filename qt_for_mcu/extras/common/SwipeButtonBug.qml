import QtQuick 2.0

Rectangle {
    clip: true
    width: 400
    height: 64
    color: "#282828"
    radius: 52

    signal swiped

    property bool inProgress: false
    property int initialPressX: 0

    Rectangle {
        id: swipeButton
        height: parent.height
        width: 200
        color: "#FFFFFF"
        opacity: 0.8
        radius: 52
    }

    MouseArea {
        id: swipeButtonMA
        anchors.fill: parent

        onPressed: {
            initialPressX = mouse.x;
            inProgress = true;
        }

        onPositionChanged: {
            swipeButton.x = Math.max(0, Math.min(200, mouse.x - initialPressX));
            if (mouse.x - initialPressX < 0)
                initialPressX = mouse.x;
            if (swipeButton.x == 200)
                swiped();
        }

        onReleased: {
            swipeButton.x = 0;
            inProgress = false;
        }
    }
}
