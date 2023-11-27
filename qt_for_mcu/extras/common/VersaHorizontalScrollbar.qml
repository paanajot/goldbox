import QtQuick 2.15

Rectangle {
    id: root
    height: 11
    color: "#3A3A3B"
    clip: true
    radius: 10
    z: 1
    visible: contentWidth > root.width

    readonly property int scrollbarMargin: 0

    property int contentX: 0
    property int contentWidth: 0

    onWidthChanged: updateInnerScrollbarWidth()
    onContentWidthChanged: updateInnerScrollbarWidth()
    onContentXChanged: updateContentX(contentX)

    function updateInnerScrollbarWidth() {
        var fullWidth = root.width - scrollbarMargin;
        if (contentWidth < root.width) {
            innerScrollbar.width = fullWidth;
        } else {
            var widthCoeff = root.width / contentWidth;
            innerScrollbar.width = fullWidth * (widthCoeff + (1 - widthCoeff) * 0.3);
        }
    }

    function updateContentX(contentX: int) {
        var scrollbarWidthDiff = root.width - innerScrollbar.width;
        var invisibleWidth = contentWidth - root.width;
        innerScrollbar.x = (contentX / invisibleWidth) * scrollbarWidthDiff;
        root.x = contentX;
    }

    Rectangle {
        id: innerScrollbar
        anchors.verticalCenter: parent.verticalCenter
        height: root.height - scrollbarMargin
        radius: root.radius

        color: "#727272"
    }
}
