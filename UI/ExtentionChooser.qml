import QtQuick
import QtQuick.Controls

ComboBox {
    id: comboBox

    property alias chosenExtension: comboBox.currentText
    property string indexInputText: ""
    property string indexInputCombo: ""

    anchors.leftMargin: 10
    width: parent.width - 20
    height: 25
    model: ["All", ".3gp", ".7z", ".ai", ".apk", ".app", ".aspx",".avi", ".bat", ".bin", ".bmp", ".c", ".cab", ".css", ".csv",
    ".dat", ".db", ".dll", ".dmg", ".doc", ".docx", ".dwg", ".dxf",".eps", ".exe", ".flv", ".gif", ".h", ".html", ".ico", ".iso",
    ".java", ".jpeg", ".jpg", ".js", ".json", ".jsp", ".log", ".m4a",".m4v", ".mkv", ".mov", ".mp3", ".mp4", ".mpeg", ".mpg", ".odp",
    ".ods", ".odt", ".ogg", ".otf", ".pdf", ".php", ".png", ".ppt",".pptx", ".psd", ".py", ".rar", ".rb", ".rtf", ".sh", ".sql",".svg",
    ".swf", ".sys", ".tar.gz", ".tex", ".tif", ".tiff",".tsv", ".txt", ".wav", ".webm", ".webp", ".xls", ".xlsx", ".xml", ".zip"]
    // What if we get this list from our DB
    // .tar.gz will never be shown
    currentIndex: 0
    onCurrentTextChanged: fillModel.filter(indexInputCombo, indexInputText, comboBox.currentText)
}
