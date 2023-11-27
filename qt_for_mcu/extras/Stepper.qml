import QtQuick 2.0

Item {
    id: root

    width: Math.min(300, 24 * numberOfSteps)
    height: 8

    property int numberOfSteps: 20
    property int currentStep: 1
    property int stepSpacing: 3

    property int singleStepWidth: width / numberOfSteps - stepSpacing

    Row {
        spacing: stepSpacing
        Repeater {
            model: numberOfSteps
            Rectangle {
                id: buildStepsStepperBG
                color: (index == currentStep) ? "#81fba3" : "#1affffff"
                radius: 5
                width: singleStepWidth
                height: root.height
            }
        }
    }
}
