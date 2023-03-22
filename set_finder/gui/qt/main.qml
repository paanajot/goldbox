import QtQuick
import QtMultimedia

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    MediaDevices {
        id: mediaDevices
    }

    Rectangle {
        anchors.fill: parent

        CaptureSession {
            camera: Camera {
                cameraDevice: mediaDevices.defaultVideoInput
            }
            videoOutput: videoOutput
        }

        VideoOutput {
            id: videoOutput
            orientation: 1
            anchors.fill: parent
        }
    }
}
