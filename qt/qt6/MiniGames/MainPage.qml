/******************************************************************************
**
** Copyright (C) 2022 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Quick Ultralite module.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
******************************************************************************/
import QtQuick 2.15
//import Settings 1.0

Window {
    id: root
    width: 480
    height: 320
    visible: true
    color: "#41CD52" //SettingsData.backgroundColor

    readonly property int numberOfPages: 5
    property color pressedColor: pressedColor1
    property int currentPage: 0

    readonly property color pressedColor1: "#239130"
    readonly property color pressedColor2: "#1F6266"
    readonly property color pressedColor3: "#917B2D"
    readonly property color pressedColor4: "#8A3073"

    function loadPage(pageNumber: int) {
        switch (pageNumber) {
            case 0:
                mainLoader.setSource("")
                break;
            case 1:
                mainLoader.setSource("TicTacToe.qml")
                break;
            case 2:
                mainLoader.setSource("Memory.qml")
                break;
            case 3:
                mainLoader.setSource("ConnectDots.qml")
                break;
            case 4:
                mainLoader.setSource("Settings.qml")
                break;
        }
    }

    function changePressedColor() {
        switch(root.color) {
            case "#41CD52":
                root.pressedColor = pressedColor1
                break;
            case "#3CC2C9":
                root.pressedColor = pressedColor2
                break;
            case "#FFD952":
                root.pressedColor = pressedColor3
                break;
            case "#FF5CD6":
                root.pressedColor = pressedColor4
                break;
        }
    }

    Connections {
        target: SettingsData
        onBackgroundColorChanged: changePressedColor()
    }

    Loader {
        id: mainLoader
        anchors.fill: parent

        Image {
            anchors.centerIn: parent
            source: "qrc:/images/qt_logo.png"
            visible: mainLoader.status === Loader.Null
        }
    }

    Rectangle {
        color: (previous.pressed) ? pressedColor : root.color
        height: root.height
        width: root.width / 10
        anchors.left: root.left

        Image {
            anchors.centerIn: parent
            source: "qrc:/images/previous_page.png"
            scale: 0.7
        }

        MouseArea {
            id: previous
            anchors.fill: parent
            onClicked: {
                if(--currentPage < 0) currentPage = numberOfPages-1
                loadPage(currentPage)
            }
        }
    }

    Rectangle {
        color: (next.pressed) ? pressedColor : root.color
        x: root.width-width
        height: root.height
        width: root.width / 10

        Image {
            anchors.centerIn: parent
            source: "qrc:/images/next_page.png"
            scale: 0.7
        }

        MouseArea {
            id: next
            anchors.fill: parent
            onClicked: {
                currentPage = (++currentPage)%(numberOfPages)
                loadPage(currentPage)
            }
        }
    }
}
