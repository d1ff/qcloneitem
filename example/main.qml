import QtQuick 2.0
import QtQuick.Controls 1.3
import Clone 1.0

Rectangle {
    id: root
    width: 640
    height: 480

    Row {
        id: buttonRow1
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        height: 30
        spacing: 20
        SpinBox {
            id: xSpinner
            prefix: "x: "
            maximumValue: 10000
            height: 30
            width: 100
            value: clone.source.x
        }

        SpinBox {
            id: ySpinner
            prefix: "y: "
            maximumValue: 10000
            height: 30
            width: 100
            value: clone.source.y
        }

        SpinBox {
            id: widthSpinner
            prefix: "width: "
            maximumValue: 10000
            height: 30
            width: 100
            value: clone.source.width
        }

        SpinBox {
            id: heightSpinner
            prefix: "height: "
            maximumValue: 10000
            height: 30
            width: 100
            value: clone.source.height
        }
    }

    Row {
        id: buttonRow
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        height: 30
        spacing: 20
        CheckBox {
            id: visibleCheckBox
            text: "Visible"
            checked: true
            height: 30
            width: 100
        }

        CheckBox {
            id: clientAreaOnlyCheckBox
            text: "ClientAreaOnly"
            checked: true
            height: 30
            width: 150
        }

        Slider {
            id: opacitySlider
            height: 30
            width: 100
            value: 1.0
        }
    }

    Clone {
        id: clone
        anchors.fill: parent
        anchors.margins: 50
        visible: visibleCheckBox.checked
        opacity: opacitySlider.value
        sourceClientAreaOnly: clientAreaOnlyCheckBox.checked
        windowTitle: "Calculator"
        windowClass: "CalcFrame"
        source.x: xSpinner.value
        source.y: ySpinner.value
        source.width: widthSpinner.value
        source.height: heightSpinner.value
    }
}
