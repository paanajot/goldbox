import QtQuick 2.0

Flickable {
    flickableDirection: Flickable.VerticalFlick
    boundsBehavior: Flickable.StopAtBounds
    height: 600
    width: 1000
    contentHeight: 1200

    SwipeButton {
        id: swipeButton
        anchors.centerIn: parent
        visible: true
    }
}
