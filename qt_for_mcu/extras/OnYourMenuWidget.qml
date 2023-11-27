import QtQuick 2.0

Image {
    id: healthScoreBG
    source: "assets/onYourMenuBG.png"

    ListView {
        height: parent.height
        width: parent.width
        anchors.top: parent.top
        anchors.left: parent.left
        clip: true

        orientation: Qt.Horizontal
        boundsBehavior: Flickable.StopAtBounds

        delegate: component
        model: listModel
    }

    Component {
        id: component

        Item {
            height: healthScoreBG.height
            width: 207

            Rectangle {
                id: cookingTimeRect
                height: 19
                width: 100
                anchors.left: mealImg.left
                anchors.leftMargin: 63
                anchors.top: mealImg.top
                anchors.topMargin: 18
                radius: 19
                color: "#FFFFFF"
                opacity: 0.06
            }

            Image {
                id: cookingImg
                source: "assets/onMealCookingTime.png"
                anchors.left: cookingTimeRect.left
                anchors.verticalCenter: cookingTimeRect.verticalCenter
                anchors.leftMargin: 25
            }

            Text {
                anchors.left: cookingImg.right
                anchors.leftMargin: 5
                anchors.verticalCenter: cookingImg.verticalCenter
                text: model.cookingTime + " Minutes"
                font.pixelSize: 9
                color: "#FFFFFF"
            }

            Rectangle {
                id: healthScoreRect
                height: 19
                width: 122
                anchors.left: mealImg.left
                anchors.leftMargin: 63
                anchors.top: mealImg.top
                anchors.topMargin: 49
                radius: 19
                color: "#FFFFFF"
                opacity: 0.06
            }
            Image {
                id: healthScoreImg
                source: "assets/onMealHealthScore.png"
                anchors.left: healthScoreRect.left
                anchors.verticalCenter: healthScoreRect.verticalCenter
                anchors.leftMargin: 25
            }
            Text {
                anchors.left: healthScoreImg.right
                anchors.leftMargin: 5
                anchors.verticalCenter: healthScoreImg.verticalCenter
                text: model.healthScore + " Health Score"
                font.pixelSize: 9
                color: "#FFFFFF"
            }

            Text {
                anchors.bottom: mealImg.top
                anchors.bottomMargin: 9
                anchors.horizontalCenter: mealImg.horizontalCenter
                text: model.type
                font.pixelSize: 13
                color: "#FFFFFF"
            }

            Image {
                id: mealImg
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.verticalCenter: parent.verticalCenter

                width: 80
                height: 80
                fillMode: Image.PreserveAspectFit
                source: model.sourceImg
            }

            Text {
                anchors.top: mealImg.bottom
                anchors.topMargin: 9
                anchors.horizontalCenter: mealImg.horizontalCenter
                text: model.name
                font.pixelSize: 13
                color: "#FFFFFF"
            }
        }
    }

    MealPlanListModel {
        id: listModel
    }
}
