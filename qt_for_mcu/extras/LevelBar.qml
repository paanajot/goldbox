import QtQuick 2.0

Item {
    width: 88
    height: 42

    property double value: 70
    property double low: 20
    property double high: 50
    property double highest: 100

    readonly property int barHeight: 6
    readonly property int barToLevelSpacing: 8

    function updateState() {
        if (value < low)
            state = "Low";
        else if (value < high)
            state = "Great";
        else
            state = "High";
    }

    onValueChanged: updateState()

    readonly property color highColor: "#ff9900"
    readonly property color greatColor: "#81fba3"
    readonly property color lowColor: "#005ce6"

    state: "Great"

    Rectangle {
        id: levelbar
        width: Math.min(value / highest, 1) * parent.width
        height: barHeight

        color: greatColor
        radius: 32
    }

    Text {
        id: leveltext

        anchors.top: levelbar.bottom
        anchors.topMargin: barToLevelSpacing
        anchors.horizontalCenter: parent.horizontalCenter

        color: levelbar.color
        text: state
        font.pixelSize: 14
        horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignTop
        wrapMode: Text.NoWrap
        font.weight: Font.Bold
        // font.family: "Inter"
    }

    states: [
        State {
            name: "High"
            PropertyChanges {
                target: levelbar
                color: highColor
            }
        },
        State {
            name: "Great"
            PropertyChanges {
                target: levelbar
                color: greatColor
            }
        },
        State {
            name: "Low"
            PropertyChanges {
                target: levelbar
                color: lowColor
            }
        }
    ]
}
