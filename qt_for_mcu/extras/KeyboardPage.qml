import QtQuick 2.0
import KeyboardModule 1.0

Rectangle {
    id: root
    color: "#333333"
    height: 500
    width: 300

    // MouseArea {
    //     id: mouseArea
    //     anchors.fill: parent
    //     onPressed: {
    //         // Question: Is there a better way to get either absolute touch y or absolute text input top and bottom y?
    //         keyboard.absoluteTouchY = mouseY - mainView.y;
    //         mouse.accepted = false;
    //     }
    // }

    // Connections {
    //     target: keyboard
    //     onMoveView: mainView.y = yPos
    // }
    Keyboard {
        id: keyboard
        height: parent.height
        width: parent.width
    }

    PasswordTextInput {
        id: psswordTextInput

        anchors.top: root.top
        anchors.topMargin: 60
        anchors.horizontalCenter: parent.horizontalCenter
        height: 52
        width: 368

        text: "password"
        font: Qt.font({
                "unicodeCoverage": [Font.UnicodeBlock_BasicLatin],
                "pixelSize": 16
            })
    }
}
