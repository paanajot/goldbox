import QtQuick

Window {
    width: 480
    height: 272
    visible: true

    Rectangle {
        anchors.fill: parent
        state: "Init"
        Text {
            id: sampleText
            anchors.centerIn: parent
            color: "salmon"
            text: "Hello World!"
            font.pixelSize: 14
            scale: 1
            opacity: 1
        }
        states: [
            State {
                name: "Init"
                PropertyChanges {
                    target: sampleText
                    opacity: 0
                    scale: 1
                }
            },
            State {
                name: "On"
                PropertyChanges {
                    target: sampleText
                    opacity: 1
                    scale: 2
                }
            }
        ]
        transitions: [
            Transition {
                from: "Init"
                to: "On"
                ParallelAnimation {
                    NumberAnimation {
                        property: "scale"
                        duration: 3000
                        onStopped: {
                            console.log("scale NumberAnimation stopped")
                        }
                    }
                    NumberAnimation {
                        property: "opacity"
                        duration: 3000
                        onStopped: {
                            console.log("opacity NumberAnimation stopped")
                        }
                    }
                    onStarted: console.log("ParallelAnimation started")
                    onStopped: {
                        console.log("ParallelAnimation stopped")
                        sampleText.text = "ParallelAnimation stopped"
                    }
                }
            }
        ]
        Component.onCompleted: {
            state = "On"
        }
    }
}
