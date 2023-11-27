import QtQuick 2.0
import KeyboardModule 1.0

// import FilterButtonsModule 1.0
Rectangle {
    id: root
    color: "#232323"
    height: 600
    width: 1024

    FlickableWrapper {
        anchors.centerIn: parent
        visible: false
    }

    OnYourMenuWidget {
        anchors.centerIn: parent
        visible: false
    }

    HealthScoreWidget {
        anchors.centerIn: parent
        visible: false
    }

    // FiltersTmpItem {
    //     anchors.left: parent.left
    //     anchors.top: parent.top
    //     visible: true

    // }
    CircularLevel {
        // not working
        anchors.centerIn: parent
        visible: false
    }

    Stepper {
        id: stepper
        anchors.centerIn: parent
        visible: false
    }

    ProjectListDynamicText {
        anchors.centerIn: parent
        visible: false
    }

    HorizontalListView {
        anchors.centerIn: parent
        visible: false
    }

    LiveMeasureBar {
        anchors.centerIn: parent
        visible: false
    }

    ThreePartsLevelBar {
        anchors.centerIn: parent
        visible: true
    }

    LevelBar {
        anchors.centerIn: parent
        visible: false
    }

    ProjectListPage {
        height: root.height
        width: root.width
        visible: false
    }

    Tumbler {
        height: root.height
        width: root.width
        visible: false
        feetRepresantation: true
        initValue: 6
        feetDecimalInitPart: 8
        suffix: " ft"
    }

    KeyboardPage {
        height: root.height
        width: root.width
        visible: false
    }
}
