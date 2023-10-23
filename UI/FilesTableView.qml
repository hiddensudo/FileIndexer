import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ListView {
    property var columnWidths: ({
        "fileID": parent.width / 7.9,
        "name": parent.width / 7.9,
        "extension": parent.width / 7.9,
        "date": parent.width / 5.9,
        "size": parent.width / 9.9
    })
    property var calculatedColumns: ["fileID", "name", "extension", "date", "size"]

    boundsBehavior: Flickable.StopAtBounds
    orientation: Qt.Vertical
    anchors.fill: parent
    model: listModel

    Connections {
        onFilterFinished: {
            for (var i = 0; i < calculatedColumns.length; i++) {
                var role = calculatedColumns[i];
                if (!columnWidths[role])
                    columnWidths[role] = 0;

                var modelWidth = columnWidths[role];
                for (var j = 0; j < model.count; j++) {
                    textMetrics.text = model.get(j)[role];
                    modelWidth = Math.max(textMetrics.width, modelWidth);
                }
                columnWidths[role] = modelWidth;
            }
        }
    }

    TextMetrics {
        id: textMetrics
    }

    delegate: RowLayout {
        property var columnWidths: ListView.view.columnWidths

        spacing: 0

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: columnWidths.fileID
            text: fileID

            background: Rectangle {
                border.color: "black"
                anchors.leftMargin: 10
            }

        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: columnWidths.name
            text: name

            background: Rectangle {
                border.color: "black"
                anchors.leftMargin: 10
            }

        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: columnWidths.extension
            text: extension

            background: Rectangle {
                border.color: "black"
                anchors.leftMargin: 10
            }

        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: columnWidths.date
            text: date

            background: Rectangle {
                border.color: "black"
                anchors.leftMargin: 10
            }

        }

        Label {
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.preferredHeight: implicitHeight
            Layout.preferredWidth: columnWidths.size
            text: size

            background: Rectangle {
                border.color: "black"
                anchors.leftMargin: 10
            }

        }

    }

}
