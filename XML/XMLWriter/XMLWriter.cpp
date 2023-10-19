#include "XMLWriter.h"

#include <QDebug>
#include <QDomDocument>
#include <QTextStream>

void XMLWriter::openFile() {
    if (!this->file.open(QIODevice::ReadWrite | QIODevice::Text)) {
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

QDomElement XMLWriter::getRoot(const QString &listName,
                               QDomDocument &document) {
    QDomElement root;
    if (document.elementsByTagName(listName).isEmpty()) {
        root = document.createElement(listName);
        document.appendChild(root);
    } else {
        root = document.elementsByTagName(listName).at(0).toElement();
    }
    return root;
}

void XMLWriter::writeToFile(QDomDocument &document) {
    this->file.resize(0);
    QTextStream xmlContent(&file);
    xmlContent << document.toString();
}

void XMLWriter::writeInFile(const QString listName, const QString name,
                            const QString extension, const QString date,
                            const QString size) {
    if (isFileExists(listName, name)) {
        // qDebug() << "File already exists: " << name;
        return;
    }
    QDomElement root = getRoot(listName, document);

    int nextFileNumber = 1;
    QDomNodeList files = root.elementsByTagName("file");
    if (!files.isEmpty()) {
        QDomElement lastFile = files.at(files.count() - 1).toElement();
        nextFileNumber = lastFile.attribute("fileID").toInt() + 1;
    }

    QDomElement fileElem = document.createElement("file");
    fileElem.setAttribute("fileID", QString::number(nextFileNumber));
    fileElem.appendChild(createTextElement("name", name, document));
    fileElem.appendChild(createTextElement("extension", extension, document));
    fileElem.appendChild(createTextElement("date", date, document));
    fileElem.appendChild(createTextElement("size", size, document));

    root.appendChild(fileElem);
    writeToFile(document);
}

QDomElement XMLWriter::createTextElement(const QString &tagName,
                                         const QString &text,
                                         QDomDocument &document) {
    QDomElement element = document.createElement(tagName);
    QDomText elementText = document.createTextNode(text);
    element.appendChild(elementText);
    return element;
}

bool XMLWriter::isFileExists(const QString &listName, const QString &fileName) {
    QDomElement root = getRoot(listName, document);
    QDomNodeList files = root.elementsByTagName("file");
    for (int i = 0; i < files.count(); i++) {
        QDomElement fileElem = files.at(i).toElement();
        QDomNodeList names = fileElem.elementsByTagName("name");
        if (!names.isEmpty()) {
            QDomElement nameElem = names.at(0).toElement();
            if (nameElem.text() == fileName) {
                return true;
            }
        }
    }
    return false;
}

XMLWriter::XMLWriter(const QString &fileName) : file(fileName) {
    openIfExist();
    if (this->file.size() != 0) {
        document.setContent(&this->file);
    }
}

XMLWriter::~XMLWriter() {
    if (this->file.isOpen()) {
        this->file.close();
    }
}