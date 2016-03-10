import QtQuick 2.3
import QtQuick.Window 2.2
import QtQuick.Controls 1.3
import Clone 1.0
/*
  Copyright (c) 2015-2016, Jesper Hellesø Hansen
  jesperhh@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:
      * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
      * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
      * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
Window {
    id: root
    visible: true
    width: 640
    height: 480
    color: "white"

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

        Button {
            text: "Update"
            onClicked: {
                clone.source = Qt.rect(xSpinner.value, ySpinner.value, widthSpinner.value, heightSpinner.value)
            }
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

    Rectangle {
        color: "black"
        anchors.fill: parent
        anchors.margins: 50
        Clone {
            id: clone
            anchors.fill: parent
            visible: visibleCheckBox.checked
            opacity: opacitySlider.value
            sourceClientAreaOnly: clientAreaOnlyCheckBox.checked
            windowTitle: "User Promoted Notification Area"
            windowClass: "ToolbarWindow32"
        }
    }
}
