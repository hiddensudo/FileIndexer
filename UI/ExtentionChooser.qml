import QtQuick
import QtQuick.Controls

ComboBox {
    id: comboBox
    anchors.leftMargin: 10
    width: parent.width - 20
    height: 25
    model: ["All", ".txt", ".cpp", ".h", ".html"]
}
