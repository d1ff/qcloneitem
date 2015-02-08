import QtQuick 2.0
import QCloneItem 1.0
import QtQuick.Controls 1.3

Item {
    id: root
    width: 640
    height: 480
    Rectangle {
        anchors.fill: parent
        color: "red"

        Row {
            id: buttonRow
            anchors.bottom: parent.bottom
            height: 30
            spacing: 10
            Button {
                text: "Visible: " + clone.visible
                onClicked: {
                    clone.visible = !clone.visible
                }
                height: 30
                width: 100
            }

            Button {
                text: "ClientAreaOnly: " + clone.sourceClientAreaOnly
                onClicked: {
                    clone.sourceClientAreaOnly = !clone.sourceClientAreaOnly
                }
                height: 30
                width: 150
            }

            Button {
                text: "opacity: " + clone.opacity
                onClicked: {
                    clone.opacity = clone.opacity - 0.1
                }
                height: 30
                width: 100
            }
        }

        Clone {
            id: clone
            anchors.fill: parent
            anchors.margins: 50
            windowTitle: "Steam"
        }

        Rectangle {
            id: leftMover
            color: "black"
            x: clone.source.x
            onXChanged: {
                if (leftArea.drag.active)
                    clone.source.x = x
            }
            width: 10
            anchors.top: parent.top
            anchors.bottom: buttonRow.top
            MouseArea {
                id: leftArea
                anchors.fill: parent
                drag.target: parent
            }
        }

        Rectangle {
            id: rightMover
            color: "black"
            x: clone.source.width
            onXChanged: {
                if (rightArea.drag.active)
                    clone.source.width = x
            }
            width: 10
            anchors.top: parent.top
            anchors.bottom: buttonRow.bottom
            MouseArea {
                id: rightArea
                anchors.fill: parent
                drag.target: parent
            }
        }

        Rectangle {
            id: topMover
            color: "black"
            y: clone.source.y
            onYChanged: {
                if (topArea.drag.active)
                    clone.source.y = y
            }
            height: 10
            anchors.left: parent.left
            anchors.right: parent.right
            MouseArea {
                id: topArea
                anchors.fill: parent
                drag.target: parent
            }
        }

        Rectangle {
            id: bottomMover
            color: "black"
            y: clone.source.height
            onYChanged: {
                if (bottomArea.drag.active)
                    clone.source.height = y
            }
            height: 10
            anchors.left: parent.left
            anchors.right: parent.right
            MouseArea {
                id: bottomArea
                anchors.fill: parent
                drag.target: parent
            }
        }
    }
}