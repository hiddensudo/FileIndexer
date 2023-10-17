import QtQuick 2.12
import QtQuick.Controls 2.15  // Додайте імпорт контролів

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        text: "Почати індексацію"
        anchors.centerIn: parent
        onClicked: {
            indexer.startIndexing();
        }
    }
}
