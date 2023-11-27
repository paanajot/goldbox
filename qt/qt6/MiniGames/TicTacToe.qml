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
import QtQuick.Shapes 1.0
import LoaderData

Rectangle {
    id: root
    color: SettingsData.backgroundColor

    readonly property int onePieceLength: Math.min(root.height, root.width) / 4
    readonly property int lineWidth: 5
    readonly property int lineLength: 3*onePieceLength + 2*lineWidth

    property int turn: TicTacToe.Status.Cross
    property bool stop: false

    function reset() {
        for (var i = 0; i < 3; i++) {
            firstLine.itemAt(i).status = TicTacToe.Status.None
            secondLine.itemAt(i).status = TicTacToe.Status.None
            thirdLine.itemAt(i).status = TicTacToe.Status.None
        }
        turn = TicTacToe.Status.Cross
        stop = false
    }

    function checkRows(mark: int) : bool {
        var firstLineResult = 0;
        var secondLineResult = 0;
        var thirdLineResult = 0;

        for (var i = 0; i < 3; i++) {
            if(firstLine.itemAt(i).status != mark) ++firstLineResult
            if(secondLine.itemAt(i).status != mark) ++secondLineResult
            if(thirdLine.itemAt(i).status != mark) ++thirdLineResult
        }

        if(!firstLineResult) drawRowWinnerLine(0)
        if(!secondLineResult) drawRowWinnerLine(1)
        if(!thirdLineResult) drawRowWinnerLine(2)

        return (!firstLineResult || !secondLineResult || !thirdLineResult)
    }

    function checkColumns(mark: int) : bool {
        for (var i = 0; i < 3; i++) {
            if(firstLine.itemAt(i).status == mark &&
               secondLine.itemAt(i).status == mark &&
               thirdLine.itemAt(i).status == mark)
            {
                drawColumnWinnerLine(i)
                return true
            }
        }
        return false
    }

    function checkDiagonals(mark: int) : bool {
        if(firstLine.itemAt(0).status == mark &&
           secondLine.itemAt(1).status == mark &&
           thirdLine.itemAt(2).status == mark)
        {
            drawDiagonalWinnerLine(0)
            return true

        } else if(firstLine.itemAt(2).status == mark &&
           secondLine.itemAt(1).status == mark &&
           thirdLine.itemAt(0).status == mark)
        {
            drawDiagonalWinnerLine(1)
            return true
        }

        return false
    }

    function checkWin(mark: int) {

        if(checkRows(mark) || checkColumns(mark) || checkDiagonals(mark)) {
            stop = true
        }
    }

    function drawRowWinnerLine(row: int) {
        shape.startX = board.x
        shape.startY = board.y + row*(onePieceLength+lineWidth) + onePieceLength/2
        endLine.x = shape.startX + lineLength
        endLine.y = shape.startY
    }

    function drawColumnWinnerLine(column: int) {
        shape.startX = board.x + column*(onePieceLength+lineWidth) + onePieceLength/2
        shape.startY = board.y
        endLine.x = shape.startX
        endLine.y = shape.startY + lineLength
    }

    function drawDiagonalWinnerLine(diagonal: int) {
        switch(diagonal) {
            case 0:
                shape.startX = board.x
                shape.startY = board.y
                endLine.x = shape.startX + board.width
                endLine.y = shape.startY + board.height
                break;
            case 1:
                shape.startX = board.x + board.width
                shape.startY = board.y
                endLine.x = board.x
                endLine.y = shape.startY + board.height
                break;
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: reset()
    }

    enum Status {
        None,
        Cross,
        Nought
    }

    Component {
        id: onePiece
        Rectangle {
            width: onePieceLength
            height: onePieceLength
            color: root.color
            property int status: TicTacToe.Status.None

            Image {
                visible: status
                anchors.centerIn: parent
                source: (status === TicTacToe.Status.Cross) ? "images/cross.png" : "images/nought.png"
                scale: onePieceLength / 100
            }

            MouseArea {
                anchors.fill: parent
                enabled: !stop
                onClicked: {
                    if(status === TicTacToe.Status.None) {
                        parent.status = turn
                        checkWin(turn)
                        turn = (turn === TicTacToe.Status.Cross) ? TicTacToe.Status.Nought : TicTacToe.Status.Cross
                    }
                }
            }
        }
    }

    Column {
        id: board
        anchors.centerIn: parent
        spacing: lineWidth

        Row {
            spacing: lineWidth
            Repeater {
                id: firstLine
                model: 3
                delegate: onePiece
            }
        }

        Row {
            spacing: lineWidth
            Repeater {
                id: secondLine
                model: 3
                delegate: onePiece
            }
        }

        Row {
            spacing: lineWidth
            Repeater {
                id: thirdLine
                model: 3
                delegate: onePiece
            }
        }
    }

    Column {
        anchors.centerIn: parent
        spacing: onePieceLength
        Repeater {
            model: 2
            Rectangle {
                width: lineLength
                height: lineWidth
                color: "black"
                radius: 5
            }
        }
    }

    Row {
        anchors.centerIn: parent
        spacing: onePieceLength
        Repeater {
            model: 2
            Rectangle {
                width: lineWidth
                height: lineLength
                color: "black"
                radius: 5
            }
        }
    }

    Shape {
        anchors.fill: parent
        visible: stop

        ShapePath {
            id: shape
            strokeColor: "black"
            strokeWidth: 3
            capStyle: ShapePath.RoundCap
            startX: 0
            startY: 0

            PathLine {
                id: endLine
                x: 100
                y: 100
            }
        }
    }
}
