import QtQuick 2.15

Rectangle {
    id: root
    width: 12
    color: "#9A9A9A"
    clip: true
    radius: 0
    z: 1
    visible: contentHeight >= root.height

    readonly property int scrollbarMargin: 6

    property int contentY: 0
    property int contentHeight: 0

    onHeightChanged: updateInnerScrollbarHeight()
    onContentHeightChanged: updateInnerScrollbarHeight()
    onContentYChanged: updateContentY(contentY)

    function updateInnerScrollbarHeight() {
        var fullHeight = root.height-scrollbarMargin

        if(contentHeight < root.height) {
            innerScrollbar.height = fullHeight
        } else {
            var heightCoeff = root.height/contentHeight
            innerScrollbar.height = fullHeight*(heightCoeff + (1-heightCoeff)*0.3)
        }
    }

    function updateContentY(contentY: int) {
        var scrollbarHeightDiff = root.height - innerScrollbar.height
        var invisibleHeight = contentHeight - root.height
        innerScrollbar.y = (contentY / invisibleHeight) * scrollbarHeightDiff
        root.y = contentY
    }

    Rectangle {
        id: innerScrollbar
        anchors.horizontalCenter: parent.horizontalCenter
        width: root.width - scrollbarMargin
        radius: root.radius

        color: "#D8D6D6"
    }
}
