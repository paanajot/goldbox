import QtQuick 2.0

Item {
    width: 240
    height: 204

    property int healthScore: 50
    state: "good"

    onHealthScoreChanged: {
        if (healthScore <= 20) {
            state = "offtrack";
        } else if (healthScore <= 40) {
            state = "imbalanced";
        } else if (healthScore <= 60) {
            state = "good";
        } else if (healthScore <= 80) {
            state = "great";
        } else if (healthScore <= 100) {
            state = "optimal";
        } else {
            console.log("not a valid healthscore value: ", healthScore);
            state = "optimal";
        }
    }

    Image {
        id: healthScoreBG
        source: "assets/healthScoreWidget/homeHealthScoreBG.png"
        anchors.centerIn: parent

        Image {
            id: healthScoreStageNames
            source: "assets/healthScoreWidget/homeHealthScoreStageNames.png"
            anchors {
                verticalCenter: healthScoreBG.verticalCenter
                horizontalCenter: healthScoreBG.horizontalCenter
                verticalCenterOffset: -14
            }
        }
        Image {
            id: healthScoreStateImg
            anchors.bottom: parent.bottom
            visible: healthScore != 0
        }

        Text {
            id: healthScoreText
            anchors.centerIn: parent
            anchors.verticalCenterOffset: 2
            text: healthScore
            font.pixelSize: 50
            font.bold: true
            color: "#FFFFFF"
        }

        Text {
            anchors.top: healthScoreText.bottom
            anchors.horizontalCenter: healthScoreText.horizontalCenter
            text: "Health Score"
            font.pixelSize: 14
            color: "#B3B3B3"
            width: 30
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    states: [
        State {
            name: "offtrack"
            PropertyChanges {
                target: healthScoreStateImg
                source: "assets/healthScoreWidget/homeHealthscoreStatsCircleBar_state_property_1_OffTrackState.png"
            }
        },
        State {
            name: "imbalanced"
            PropertyChanges {
                target: healthScoreStateImg
                source: "assets/healthScoreWidget/homeHealthscoreStatsCircleBar_state_property_1_ImbalancedState.png"
            }
        },
        State {
            name: "good"
            PropertyChanges {
                target: healthScoreStateImg
                source: "assets/healthScoreWidget/homeHealthscoreStatsCircleBar_state_property_1_GoodState.png"
            }
        },
        State {
            name: "great"
            PropertyChanges {
                target: healthScoreStateImg
                source: "assets/healthScoreWidget/homeHealthscoreStatsCircleBar_state_property_1_GreatState.png"
            }
        },
        State {
            name: "optimal"
            PropertyChanges {
                target: healthScoreStateImg
                source: "assets/healthScoreWidget/homeHealthscoreStatsCircleBar_state_property_1_OptimalState.png"
            }
        }
    ]
}
