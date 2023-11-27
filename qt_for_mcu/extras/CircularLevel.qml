import QtQuick
import QtQuick.Shapes 1.0

Rectangle {
    id: root
    width: 200
    height: 200
    color: "grey"

    Shape {
        id: circularBackground
        anchors.fill: parent

        ShapePath {
            fillColor: "transparent"
            strokeColor: "#404141"
            strokeWidth: 10
            capStyle: ShapePath.RoundCap

            startX: 30
            startY: 190
            PathArc {
                x: 170
                y: 190
                radiusX: 100
                radiusY: 100
                useLargeArc: true
            }
        }
    }

    Shape {
        id: circularLevel
        anchors.fill: parent

        ShapePath {
            fillColor: "transparent"
            strokeColor: "#81FBA3"
            strokeWidth: 10
            capStyle: ShapePath.RoundCap

            startX: 30
            startY: 190
            PathArc {

                // xAxisRotation: 45
                x: 90
                y: 8
                // x: 99
                // y: 7
                radiusX: 100
                radiusY: 100
                useLargeArc: true
            }
        }
    }
}
