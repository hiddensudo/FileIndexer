import QtQuick 2.12
import QtQuick.Controls 2.15 

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        text: "Почати індексацію"
        anchors.centerIn: parent
        onClicked: {
            pathQueue.detachRun();
        }
    }
}
