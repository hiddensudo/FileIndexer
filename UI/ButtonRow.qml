import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

Row {
    property var sharedContext

    spacing: 10
    width: parent.width

    Button {
        id: startButton

        text: "Start Indexing"
        onClicked: {
            if (!indexer.isStarted) {
                var startDirectory = sharedContext.text;
                var inCurrentDirectory = (sharedContext.comboBoxIndex === 0);
                indexer.detachRun(startDirectory, inCurrentDirectory);
                busyIndicator.running = true;
            }
        }
    }

    Button {
        id: pauseResumeButton

        text: indexer.isPaused ? "Resume" : "Pause"
        onClicked: {
            if (indexer.isPaused) {
                indexer.resume();
            } else {
                indexer.pause();
            }
        }
    }

    Button {
        id: stopButton

        text: "Stop"
        onClicked: {
            if (!indexer.isCancelled) {
                indexer.stop();
            }
        }
    }

    BusyIndicator {
        id: busyIndicator

        running: false
        Layout.bottomMargin: 100
    }

    Connections {
        target: indexer
        onIsCancelledChanged: {
            if (indexer.isCancelled)
                busyIndicator.running = false;
        }
        onIsPausedChanged: {
            if (indexer.isPaused)
                busyIndicator.running = false;
            else
                busyIndicator.running = true;
        }
    }

}
