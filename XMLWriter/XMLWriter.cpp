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

QDomElement XMLWriter::createFileElement(const QString &name,
                                         const QString &extension,
                                         const QString &date,
                                         const QString &size,
                                         QDomDocument &document) {
    QDomElement fileElem = document.createElement("file");
    fileElem.setAttribute("name", name);
    fileElem.setAttribute("extension", extension);
    fileElem.setAttribute("date", date);
    fileElem.setAttribute("size", size);
    return fileElem;
}

void XMLWriter::writeToFile(QDomDocument &document) {
    this->file.resize(0);
    QTextStream xmlContent(&file);
    xmlContent << document.toString();
}

bool XMLWriter::checkAndUpdateFile(const QString &name,
                                   const QString &extension,
                                   const QString &date, const QString &size,
                                   QDomElement &root) {
    QDomNodeList files = root.elementsByTagName("file");
    for (int i = 0; i < files.count(); i++) {
        QDomElement fileElem = files.at(i).toElement();
        if (fileElem.attribute("name") == name) {
            if (fileElem.attribute("extension") != extension ||
                fileElem.attribute("date") != date ||
                fileElem.attribute("size") != size) {
                fileElem.setAttribute("extension", extension);
                fileElem.setAttribute("date", date);
                fileElem.setAttribute("size", size);
            }
            return true;
        }
    }
    return false;
}

void XMLWriter::writeInFile(const QString listName, const QString name,
                            const QString extension, const QString date,
                            const QString size) {
    openIfExist();

    QDomDocument document;

    if (this->file.size() != 0) {
        document.setContent(&this->file);
    }

    QDomElement root = getRoot(listName, document);

    if (!checkAndUpdateFile(name, extension, date, size, root)) {
        QDomElement fileElem =
            createFileElement(name, extension, date, size, document);

        root.appendChild(fileElem);
    }

    writeToFile(document);
}

XMLWriter::XMLWriter(const QString &fileName) : XMLParser(fileName) {}
