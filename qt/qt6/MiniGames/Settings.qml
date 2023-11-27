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

    readonly property int pixelSize: 22
    readonly property int onePieceLength: Math.min(height, width) / 8
    readonly property color buttonColor: "#00718A"

    ListModel {
        id: memoryModel
        ListElement { level: "easy"; time: 1500 }
        ListElement { level: "medium"; time: 1000 }
        ListElement { level: "difficult"; time: 600 }
    }

    Column {
        x: root.width/10
        anchors.verticalCenter: parent.verticalCenter
        Row {
            Text {
                id: colorTxt
                text: "background color: "
                font.pixelSize: root.pixelSize
            }

            Rectangle {
                readonly property int spacing: 5

                id: recBack
                anchors.verticalCenter: colorTxt.verticalCenter
                width: (onePieceLength + spacing + 1) * 4
                height: onePieceLength + spacing * 3
                color: buttonColor

                Row {
                    anchors.centerIn: parent
                    spacing: recBack.spacing
                    Repeater {
                        id: recs
                        model: ["#41CD52", "#3CC2C9", "#FFD952", "#FF5CD6"]
                        Rectangle {
                            color: (colorMa.pressed) ? recBack.color : modelData
                            width: onePieceLength
                            height: width
                            anchors.centerIn: parent
                            radius: width / 2
                            MouseArea {
                                id: colorMa
                                anchors.fill: parent
                                onClicked: SettingsData.backgroundColor = modelData
                            }
                        }
                    }
                }
            }
        }

        Row {
            height: recBack.height
            Text {
                anchors.verticalCenter: parent.verticalCenter
                id: memoryTxt
                text: "memory level: "
                font.pixelSize: root.pixelSize
            }

            Repeater {
                id: levelRepeater
                model: memoryModel
                property string level: "medium"
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    width: root.width / 6.5
                    height: memoryTxt.height
                    radius: 5
                    color: (levelRepeater.level === model.level) ? buttonColor : root.color
                    Text {
                        anchors.centerIn: parent
                        text: model.level
                        font.pixelSize: root.pixelSize/1.5
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            levelRepeater.level = model.level
                            SettingsData.memoryTime = model.time
                        }
                    }
                }
            }
        }

        Row {
            height: recBack.height
            Text {
                anchors.verticalCenter: parent.verticalCenter
                id: connectDotsTxt
                text: "winning dots: "
                font.pixelSize: root.pixelSize
            }

            Repeater {
                id: dotsRepeater
                model: [3, 4]
                property int numberOfDots: SettingsData.numberOfDots
                Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    width: root.width / 6.5
                    height: connectDotsTxt.height
                    radius: 5
                    color: (dotsRepeater.numberOfDots === modelData) ? buttonColor : root.color
                    Text {
                        anchors.centerIn: parent
                        text: modelData + " dots"
                        font.pixelSize: root.pixelSize/1.5
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: SettingsData.numberOfDots = modelData
                    }
                }
            }
        }
    }
}
