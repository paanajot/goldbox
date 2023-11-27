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
import LoaderData

Rectangle {
    id: root
    color: SettingsData.backgroundColor

    readonly property int largePixelSize: 44
    readonly property int smallPixelSize: 24
    readonly property int minSideSize: Math.min(root.height, root.width)
    readonly property int onePieceLength: minSideSize / 5

    property int bestScore: MemoryData.bestScore
    property int score: 0
    property int currentScore: 0
    property bool play: false
    property bool drawColor: false

    property int seconds: 0

    readonly property color color1: "green"
    readonly property color color2: "blue"
    readonly property color color3: "red"
    readonly property color color4: "yellow"

    readonly property int maxScore: 30

    function playNewRound() {
        drawColors()
        timer.start()
    }

    function drawColors() {
        drawColor = true
        for (var i = 0; i < maxScore; ++i) {
            var number = Math.floor(Math.random() * 4)
            switch(number) {
                case 0:
                    randomColor.itemAt(i).color = color1
                    break;
                case 1:
                    randomColor.itemAt(i).color = color2
                    break;
                case 2:
                    randomColor.itemAt(i).color = color3
                    break;
                case 3:
                    randomColor.itemAt(i).color = color4
                    break;
            }
        }
    }

    Column {
        x: parent.width/10
        Text {
            text: "Best score: " + bestScore
            font.pixelSize: smallPixelSize
        }

        Text {
            text: "Score: " + score
            font.pixelSize: smallPixelSize
        }
    }

    Text {
        id: startTxt
        anchors.centerIn: mainRec
        text: "Touch me to play :)"
        font.pixelSize: smallPixelSize
    }

    Text {
        anchors.centerIn: mainRec
        visible: play
        text: "Go!"
        font.pixelSize: largePixelSize
    }

    Column {
        id: congratTxt
        visible: score === maxScore
        anchors.centerIn: mainRec
        Text {
            anchors.centerIn: parent
            text: "Congratulations!"
            font.pixelSize: largePixelSize
        }

        Text {
            anchors.centerIn: parent
            text: "You have max score :)"
            font.pixelSize: smallPixelSize
        }
    }

    Column {
        id: summary
        anchors.centerIn: mainRec
        visible: false
        Text {
            anchors.centerIn: parent
            text: "Wrong!"
            font.pixelSize: largePixelSize
        }

        Text {
            anchors.centerIn: parent
            text: "Your score: " + score
            font.pixelSize: smallPixelSize
        }
    }

    MouseArea {
        anchors.fill: parent
        enabled: !drawColor
        onClicked: {
            startTxt.visible = false
            summary.visible = false
            play = false
            score = 0
            currentScore = 0
            newRound.start()
        }
    }

    Repeater {
        id: randomColor
        model: 30 // FIXME: use maxscore
        Rectangle {
            color: "red"
        }
    }

    Timer {
        id: timer
        interval: SettingsData.memoryTime
        repeat: true
        running: false
        onTriggered: {
            drawColor = false
            if(score === seconds) {
                play = true
                timer.stop()
                seconds = 0
            } else {
                ++seconds
                blackShot.start()
            }
        }
    }

    Timer {
        id: blackShot
        interval: 80
        repeat: false
        running: false
        onTriggered: {
            drawColor = true
        }
    }

    Timer {
        id: newRound
        interval: 700
        repeat: false
        running: false
        onTriggered: {
            playNewRound()
        }
    }

    Rectangle {
        id: mainRec
        anchors.top: parent.top
        anchors.topMargin: parent.height/3
        anchors.horizontalCenter: parent.horizontalCenter

        visible: drawColor
        width: onePieceLength
        height: width
        color: randomColor.itemAt(seconds).color
        radius: 5
    }

    Row {
        anchors.top: mainRec.bottom
        anchors.topMargin: (parent.height-mainRec.y)/3
        anchors.horizontalCenter: mainRec.horizontalCenter

        spacing: 5
        Repeater {
            id: recs
            model: ["green", "blue", "red", "yellow"] // FIXME: use color1, color2 ...
            Rectangle {
                anchors.centerIn: parent
                width: onePieceLength
                height: width
                color: modelData
                radius: 5

                opacity: (ma.pressed && play)? 0 : 1

                MouseArea {
                    id: ma
                    anchors.fill: parent
                    onClicked: {
                        if(play) {
                            if(randomColor.itemAt(currentScore).color == parent.color) {
                                ++currentScore
                            } else {
                                blackShot.stop()
                                timer.stop()
                                summary.visible = true
                                play = false
                            }

                            if(currentScore > score) {
                                currentScore = 0
                                ++score
                                MemoryData.bestScore = (score > bestScore) ? score : bestScore
                                if(score != maxScore)
                                    newRound.start()

                                play = false
                            }
                        }
                    }
                }
            }
        }
    }
}
