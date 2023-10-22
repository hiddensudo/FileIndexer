import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

ApplicationWindow {
    //FileListView {}

    id: window

    visible: true
    width: 1000
    height: 540
    title: qsTr("File Indexer")

    QtObject {
        id: sharedContext

        property string text: ""
        property int comboBoxIndex: 0
    }

    Row {
        anchors.fill: parent

        Rectangle {
            width: parent.width / 2.7
            height: parent.height

            Column {
                id: col

                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.topMargin: 10

                Rectangle {
                    id: indexLabel

                    width: parent.width
                    height: 25

                    Label {
                        text: "Enter path"
                    }

                }

                IndexInput {
                    width: parent.width
                    height: 75
                    sharedContext: sharedContext
                }

                ButtonRow {
                    sharedContext: sharedContext
                }

                Rectangle {
                    id: extension

                    width: parent.width
                    height: 25

                    Label {
                        text: "Choose extension"
                    }

                }

                ExtentionChooser {
                    width: parent.width - 20
                    height: 25
                }

                SearchBar {
                    height: 100
                }

            }

        }

        Rectangle {
            width: parent.width
            height: parent.height
            color: "#dbd9d9"

            FilesTableView {
                model: myModel
            }

        }

    }

}
