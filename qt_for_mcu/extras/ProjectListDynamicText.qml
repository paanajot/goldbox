import QtQuick

Item {
    id: root
    width: 500
    height: 456

    function getY(index:int):int {
        var y = 0
        for (var i = 0; i < index; i++) {
            y += repeater.itemAt(index - 1).height
        }
        return y
    }

    Flickable {
        id: flick
        anchors.fill: parent
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        clip: true

        VersaScrollbar {
            id: scrollbar
            height: parent.height
            anchors.right: parent.right
            contentHeight: parent.contentHeight
            contentY: parent.contentY
        }

        Repeater {
            id: repeater
            model: 15
            Item {
                height: recipeInstructionsText.height
                y: getY(index)
                Image {
                    id: img
                    height: 24
                    width: 24
                    source: "assets/recipeInstructionsBG.png"
                    Text {
                        anchors.centerIn: parent
                        text: index
                        color: "#000000"
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.weight: Font.Medium
                    }
                }

                Text {
                    id: recipeInstructionsText
                    anchors.left: img.right
                    anchors.leftMargin: 16
                    color: "#cccccc"
                    text: qsTr("In a 12- to 16-quart pot bring 2 gallons water to boiling. ")
                    width: root.width - img.width - 16 - scrollbar.width
                    font.pixelSize: 18
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.Wrap
                    font.weight: Font.Normal
                }
            }

            Component.onCompleted: {
                 flick.contentHeight = getY(15)
            }
        }
    }
}
