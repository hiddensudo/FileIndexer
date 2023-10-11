#include "XMLParser.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>
#include <filesystem>
#include <iostream>

void XMLParser::openReadOnly() {
    if (!this->file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading: "
                 << qPrintable(file.errorString());
        return;
    }
}

void XMLParser::openWriteOnly() {
    if (!this->file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for writing: "
                 << qPrintable(file.errorString());
        return;
    }
}

void XMLParser::openIfFileExist() {
    if (file.exists()) {
        this->isReadOnly ? openReadOnly() : openWriteOnly();
    } else {
        qDebug() << "File does not exist";
    }
}

void XMLParser::close() { this->file.close(); }

XMLParser::XMLParser(const QString fileName, bool isReadOnly)
    : file(fileName), isReadOnly(isReadOnly) {
    openIfFileExist();
}

XMLParser::~XMLParser() { close(); }
