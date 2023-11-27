import QtQuick 2.0

Item {
    id: root
    width: 49
    height: 150

    property double level: 5
    property double limit: 100
    property alias barColor: levelBar.color

    Rectangle {
        id: bar
        anchors.fill: parent

        color: "#FFFFFF"
        opacity: 0.15
    }

    Rectangle {
        id: levelBar

        anchors.horizontalCenter: bar.horizontalCenter
        anchors.bottom: bar.bottom
        width: parent.width
        height: Math.min(1, level / limit) * parent.height
        color: "#81fba3"
    }
}

// Item {
//     width: 49
//     height: 150

//     property double level: 0
//     property double limit: 100

//     readonly property double minLevel: 0.10
//     readonly property double maxLevel: 0.95

//     Rectangle {
//         id: bar
//         anchors.fill: parent

//         color: "#FFFFFF"
//         opacity: 0.15
//         radius: 10
//     }

//     Rectangle {
//         id: levelBar

//         anchors.horizontalCenter: bar.horizontalCenter
//         anchors.bottom: bar.bottom
//         width: parent.width
//         height: Math.min(maxLevel, Math.max(minLevel, level / limit)) * parent.height

//         color: "#81fba3"
//         radius: bar.radius
//     }

//     Rectangle {
//         id: levelCap

//         anchors.top: levelBar.top
//         width: parent.width
//         height: levelBar.height / 2

//         color: levelBar.color
//     }

// Rectangle {
//     id: levelCap
//     visible: (levelBar.height - levelBar.radius) > 0

//     anchors.bottom: levelBar.bottom
//     anchors.bottomMargin: levelBar.radius
//     width: parent.width

//     height: levelBar.height - levelBar.radius

//     color: levelBar.color
// }
// }
