import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: searchBar

    property alias indexInputCombo: comboBox.currentText
    property alias indexInputText: indexInput.text
    property string extensionText: ""

    width: parent.width
    anchors.topMargin: 10
    height: 90

    Rectangle {
        id: spacing

        width: parent.width
        height: 10

        Label {
        }

    }

    Rectangle {
        id: criteriaLabel

        width: parent.width
        height: 25
        y: spacing.y + spacing.height

        Label {
            text: "Choose criteria and fill input"
        }

    }

    ComboBox {
        id: comboBox

        width: parent.width - 20
        height: 25
        y: criteriaLabel.y + criteriaLabel.height
        model: ["name", "date", "size"]
    }

    Rectangle {
        id: searchInput

        width: parent.width - 20
        height: 25
        border.color: 'gray'
        border.width: 1
        y: comboBox.y + comboBox.height + 5

        TextInput {
            id: indexInput

            anchors.fill: parent
            anchors.margins: 4
        }

    }

    Button {
        id: startButton

        y: searchInput.y + searchInput.height + 5
        text: "Find"
        onClicked: {
            if (!fillModel.isFilteringStarted) {
                fillModel.filter(comboBox.currentText, indexInput.text, extensionText);
            }
        }
    }

}
