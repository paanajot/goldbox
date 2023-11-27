import QtQuick

Item {
    width: 320
    height: 8

    property int currentHealthScore: 50
    property int diffHealthScore: -200

    readonly property int highestScore: 100

    Rectangle {
        id: backgroundBar
        anchors.fill: parent
        radius: 32
        color: "#FFFFFF"
        opacity: 0.05
    }

    Rectangle {
        id: currentScoreBar
        color: "#666666"
        height: parent.height
        anchors.left: backgroundBar.left
        radius: backgroundBar.radius

        width: Math.min(currentHealthScore / highestScore, 1) * parent.width
    }

    readonly property int newScoreBarMargin: 5
    property int newScoreBarWidth: Math.min(Math.abs(diffHealthScore) / highestScore, 1) * width
    Rectangle {
        id: newScoreBar
        color: (diffHealthScore >= 0) ? "#81FBA3" : "#DE464D"
        height: parent.height
        radius: backgroundBar.radius

        x: (diffHealthScore >= 0) ? currentScoreBar.width - newScoreBarMargin : currentScoreBar.width - width
        width: (diffHealthScore >= 0) ? Math.min(parent.width - x, newScoreBarWidth + newScoreBarMargin) : Math.min(currentScoreBar.width, newScoreBarWidth)
        z: (diffHealthScore >= 0) ? -1 : 1
    }
}
