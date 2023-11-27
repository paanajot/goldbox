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

    readonly property int minSideSize: Math.min(root.height, root.width)

    readonly property int columns: 8
    readonly property int rows: 6
    readonly property int diameter: minSideSize/8
    readonly property int spacing: 6

    property int nextTurn: ConnectDots.Status.Blue

    enum Status {
        None,
        Red,
        Blue
    }

    function isDotAllowed(index: int) : bool {
        var column = index%columns
        var row = Math.floor(index/columns)
        return !((row+1) != rows && boardRepeater.itemAt(column+(row+1)*columns).status === ConnectDots.Status.None)
    }

    function reset() {
        for(var i = 0; i < rows*columns; ++i) {
            boardRepeater.itemAt(i).status = ConnectDots.Status.None
            boardRepeater.itemAt(i).color = root.color
        }
        nextTurn = ConnectDots.Status.Blue
        winnerTxt.visible = false
    }

    function match(row: int, column: int, color: ConnectDots.Status) : bool {
            return color === boardRepeater.itemAt(column+row*columns).status;
    }

    function isWinningInRow(row: int, column: int, color: ConnectDots.Status) : bool {
        var counter = 1
        for(var i = column+1; i < columns && match(row, i, color); ++i)
            ++counter

       for(var i = column-1; i >= 0 && match(row, i, color); --i)
            ++counter

        return (counter >= SettingsData.numberOfDots)
    }

    function isWinningInColumn(row: int, column: int, color: ConnectDots.Status) : bool {
        var counter = 1
        for(var i = row+1; i < rows && match(i, column, color); ++i)
            ++counter

        return (counter >= SettingsData.numberOfDots)
    }

    function isWinningInDiagonal1(row: int, column: int, color: ConnectDots.Status) : bool {
        var counter = 1
        for(var i = column+1, j = row+1; i < columns && j < rows && match(j, i, color); ++i, ++j)
            ++counter

        for(var i = column-1, j = row-1; i >= 0 && j >= 0 && match(j, i, color); --i, --j)
            ++counter

        return (counter >= SettingsData.numberOfDots)
    }

    function isWinningInDiagonal2(row: int, column: int, color: ConnectDots.Status) : bool {
        var counter = 1
        for(var c = column-1, r = row+1; c >= 0 && r < rows && match(r, c, color); --c, ++r)
            ++counter

        for(var c = column+1, r = row-1; c < columns && r >= 0 && match(r, c, color); ++c, --r)
            ++counter

        return (counter >= SettingsData.numberOfDots)
    }

    function isMoveWinning(index: int) : bool {
        var column = index%columns
        var row = Math.floor(index/columns)
        var color = boardRepeater.itemAt(index).status

        return isWinningInRow(row, column, color) ||
               isWinningInColumn(row, column, color) ||
               isWinningInDiagonal1(row, column, color) ||
               isWinningInDiagonal2(row, column, color)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: reset()
    }

    Rectangle {
        id: background
        anchors.centerIn: parent
        color: "black"
        width: columns*(diameter+spacing)+spacing
        height: rows*(diameter+spacing)+spacing
        radius: 5
        MouseArea {
            anchors.fill: parent
        }
    }

    Repeater {
        id: boardRepeater
        model: 48 // FIXME: rows*columns
        Rectangle {
            property int status: ConnectDots.Status.None
            color: root.color

            x: (modelData%columns)*(diameter+spacing) + background.x+spacing
            y: Math.floor(modelData/columns)*(diameter+spacing) + background.y+spacing

            width: diameter
            height: width
            radius: width / 2

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if(status === ConnectDots.Status.None && isDotAllowed(modelData)) {
                        status = nextTurn
                        if(nextTurn === ConnectDots.Status.Red) {
                            nextTurn = ConnectDots.Status.Blue
                            color = "red"
                        } else {
                            nextTurn = ConnectDots.Status.Red
                            color = "blue"
                        }
                        if(isMoveWinning(modelData)) {
                            winnerTxt.winner = status
                            winnerTxt.visible = true
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: winnerTxt
        visible: false
        anchors.centerIn: parent
        width: background.width
        height: 80
        color: root.color
        opacity: 0.85

        property int winner

        Text {
            anchors.centerIn: parent
            text: (winnerTxt.winner === ConnectDots.Status.Blue) ? "Blue wins!" : "Red wins!"
            font.pixelSize: 40
        }
    }

    MouseArea {
        id: maBackground
        enabled: winnerTxt.visible
        anchors.fill: background
        onClicked: reset()
    }
}
