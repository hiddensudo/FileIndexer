#include "XMLWriter.h"

#include <QDebug>
#include <QDomDocument>
#include <QDomElement>

void XMLWriter::openFile() {
    if (!this->file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: "
                 << qPrintable(file.errorString());
        return;
    }
}

void XMLWriter::openIfExist() {
    if (this->file.exists()) {
        openFile();
    } else {
        qDebug() << "File does not exist: " << qPrintable(file.fileName());
        return;
    }
}

void XMLWriter::writeInFile(const QString listName, const QString name,
                            const QString extension, const QString date,
                            const QString size) {
    openIfExist();
    QTextStream xmlContent(&file);
    QDomDocument document;

    QDomElement root = document.createElement(listName);

    document.appendChild(root);

    QDomElement fileElem = document.createElement("file");
    fileElem.setAttribute("name", name);
    fileElem.setAttribute("extension", extension);
    fileElem.setAttribute("date", date);
    fileElem.setAttribute("size", size);
    root.appendChild(fileElem);

    xmlContent << document.toString();
}

XMLWriter::XMLWriter(const QString fileName) : XMLParser(fileName) {}