import QtQuick

Rectangle {
    id: root
    width: 500
    height: 100
    color: "lightblue"

    ProjectListModel {
        id: listModel
    }

    Component {
        id: listDelegate
        Text {
            y: 40
            color: "white"
            font: Qt.font({
                    "unicodeCoverage": [Font.UnicodeBlock_BasicLatin]
                })
            height: 40
            text: model.projectName
            width: root.width / 3
        }
    }
    ListView {
        clip: true
        delegate: listDelegate
        height: root.height
        model: listModel
        width: root.width
        orientation: Qt.Horizontal

        VersaHorizontalScrollbar {
            id: scrollbar
            anchors.top: parent.top
            contentWidth: parent.contentWidth
            contentX: parent.contentX
            width: parent.width
        }
    }
}
