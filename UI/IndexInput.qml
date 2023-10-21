import QtQuick
import QtQuick.Controls

Item {
    property var sharedContext

    width: parent.width
    height: 75

    Rectangle {
        id: indexLabel

        width: parent.width
        height: 25

        Label {
            text: "Enter path"
        }

    }

    Rectangle {
        id: frame

        width: parent.width - 20
        height: 25
        border.color: 'gray'
        border.width: 1

        TextInput {
            id: indexInput

            anchors.fill: parent
            anchors.margins: 4
            onTextChanged: {
                sharedContext.text = text;
            }
        }

    }

    ComboBox {
        id: comboBox

        width: parent.width - 20
        height: 25
        y: frame.y + frame.height + 5
        model: ["In current directory", "In current and sub"]
        onCurrentIndexChanged: {
            sharedContext.comboBoxIndex = currentIndex;
        }
    }

}
