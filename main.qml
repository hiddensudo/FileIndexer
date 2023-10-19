import QtQuick 2.12
import QtQuick.Controls 2.15 

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        id: startButton
        text: "Start Indexing"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 10
        onClicked: {
            indexer.detachRun();
        }
    }

    Button {
        id: pauseResumeButton
        text: "Pause"
        anchors.left: parent.left
        anchors.top: startButton.bottom
        anchors.margins: 10
        property bool isPaused: false
        onClicked: {
            if (isPaused) {
                indexer.resume();
                text = "Pause";
            } else {
                indexer.pause();
                text = "Resume";
            }
            isPaused = !isPaused;
        }
    }

    Button {
        id: stopButton
        text: "Stop"
        anchors.left: parent.left
        anchors.top: pauseResumeButton.bottom
        anchors.margins: 10
        onClicked: {
            indexer.stop();
        }
    }
}