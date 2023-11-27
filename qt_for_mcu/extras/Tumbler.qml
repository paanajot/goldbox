import QtQuick 2.0

Item {
    id: root
    height: 500
    width: 100

    property double selectedValue: (feetRepresantation) ? inches(newContentOffset / elementHeight) : number(newContentOffset / elementHeight)
    property double initValue: 6
    property string suffix: ""
    property double increment: 0.1  // increment doesn't work with feet representation
    property bool feetRepresantation: false
    // qt for mcu doesn't support Math.trunc(), so there is no option to get fractional init part from initValue
    property int feetDecimalInitPart: 8

    // number of digits to appear after the decimal point
    property int decimalDigits: 1
    property int numberOfElements: 29
    property bool fromHighToLow: true
    property int indexInit: 10
    property int numberOfElementsMargin: 3

    readonly property int sign: (fromHighToLow)? -1 : 1
    readonly property int pixelSize: 26
    readonly property int elementHeight: 40
    readonly property int contentOffset: indexInit * elementHeight
    property int newContentOffset: contentOffset


    function inches(index: int) : int {
        return (fromHighToLow)? hightToLowFeetToInches(index) : feetToInches(index)
    }

    function feetToInches(index: int) : int {
        var decimalPart = feetDecimalInitPart + index
        var integralPart = Math.floor(initValue) + Math.floor(decimalPart / 12)
        return integralPart*12 + decimalPart%12
    }

    function hightToLowFeetToInches(index: int) : int {
        var decimalPart = (numberOfElements*12+feetDecimalInitPart - index)
        var integralPart = Math.floor(initValue) + Math.floor((feetDecimalInitPart - index) / 12)
        return integralPart*12 + decimalPart%12
    }

    function number(index: int) : double {
        return initValue + sign * index * increment
    }

    function feetNumberText(index: int) : string {
        var decimalPart = initValue - Math.floor(initValue) + index
        var integralPart = Math.floor(initValue) + Math.floor(decimalPart / 12)
        return integralPart + "'" + decimalPart%12 + "''"
    }

    function hightToLowFeetNumberText(index: int) : string {
        var decimalPart = (numberOfElements*12+feetDecimalInitPart - index)
        var integralPart = Math.floor(initValue) + Math.floor((feetDecimalInitPart - index) / 12)
        return integralPart + "'" + decimalPart%12 + "''"
    }

    function numberText(index: int) : string {
        if(feetRepresantation) return (fromHighToLow)? hightToLowFeetNumberText(index) : feetNumberText(index)
        return number(index).toFixed(decimalDigits)
    }

    Component {
        id: listDelegate
        Text {
            anchors.centerIn: parent
            height: elementHeight
            color: "white"
            font.weight: Font.Bold
            // font.family: "Inter"
            font.pixelSize: root.pixelSize

            width: root.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: numberText(index-numberOfElementsMargin) + suffix

            opacity: 1 - Math.abs(((index - numberOfElementsMargin) * elementHeight - listView.contentY) / ((numberOfElementsMargin + 1) * elementHeight))
            scale: 1 - (1 - opacity) / 2
            transformOrigin: Item.Center
            visible: index >= numberOfElementsMargin && index < numberOfElements - numberOfElementsMargin
        }
    }

    ListView {
        id: listView
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        delegate: listDelegate
        height: elementHeight * 7
        width: root.width
        clip: true
        model: numberOfElements
        contentY: contentOffset
        boundsBehavior: Flickable.StopAtBounds

        onMovingVerticallyChanged: {
            if (!movingVertically) {
                var offset = contentY % elementHeight;
                var normalizedOffset = (offset < (elementHeight / 2)) ? -offset : (elementHeight - offset);
                newContentOffset = contentY + normalizedOffset;
                animation.start();
            }
        }

        PropertyAnimation {
            id: animation
            target: listView
            property: "contentY"
            to: newContentOffset
            easing.type: Easing.OutCubic
            duration: 200
        }
    }
}
