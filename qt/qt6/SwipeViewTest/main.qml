import QtQuick
import QtQuick.Controls 2.15

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#41CD52"
    SwipeView{
        id: theSwipe
        anchors.fill: parent
        clip: true
        Rectangle
        {
            color: "red"
        }
        Rectangle
        {
            color: "yellow"
        }
        Rectangle
        {
            color: "lightGreen"
        }
        onCurrentIndexChanged:
        {
            console.log("changed!")
        }
    }
}
