import QtQuick 2.0

Rectangle {
    id: root
    color: "green"
    height: 500
    width: 300

    ProjectListModel {
        id: listModel
    }

    property int spacing: 10
    property int elementWidth: 100
    property int elementHeight: 50
    property int numberOfElementsInRow: 2

    Flickable {
        anchors.centerIn: parent
        flickableDirection: Flickable.VerticalFlick
        height: 2 * (elementHeight + spacing)
        width: numberOfElementsInRow * (elementWidth + spacing)
        clip: true
        contentHeight: Math.ceil(listModel.count() / numberOfElementsInRow) * (elementHeight + spacing)

        Repeater {
            model: listModel
            Rectangle {
                x: (index % numberOfElementsInRow) * (width + spacing)
                y: Math.floor(index / numberOfElementsInRow) * (height + spacing)
                width: elementWidth
                height: elementHeight
                color: "blue"
                Text {
                    anchors.centerIn: parent
                    text: model.projectName
                }
            }
        }
    }

    // Component {
    //     id: listDelegate
    //     Text {
    //         font: Qt.font({
    //                 "unicodeCoverage": [Font.UnicodeBlock_BasicLatin]
    //             })
    //         height: 40
    //         text: model.projectName
    //         width: root.width
    //     }
    // }
    // ListView {
    //     clip: true
    //     delegate: listDelegate
    //     height: root.height / 3
    //     model: listModel
    //     width: root.width
    //     orientation: Qt.Vertical

    //     Scrollbar {
    //         id: scrollbar
    //         anchors.right: parent.right
    //         contentHeight: parent.contentHeight
    //         contentY: parent.contentY
    //         height: parent.height
    //     }
    // }
}
