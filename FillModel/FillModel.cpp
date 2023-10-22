#include "FillModel.h"

void FillModel::parseXML() {
    openFile();
    QFile file("../db.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file";
        return;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "file") {
                parseFile(xml, "All");
            }
        }
    }
    close();
}

void FillModel::printModel() {
    for (int row = 0; row < model->rowCount(); ++row) {
        QStringList rowData;
        for (int column = 0; column < model->columnCount(); ++column) {
            rowData << model->item(row, column)->text();
        }
        qDebug() << rowData.join(" ");
    }
}

void FillModel::filterByExtension(const QString &extension) {
    model->clear();

    openFile();
    QFile file("../db.xml");
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file";
        return;
    }

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "file") {
                parseFile(xml, extension);
            }
        }
    }
    close();
}

void FillModel::parseFile(QXmlStreamReader &xml, const QString &extension) {
    QString fileID = xml.attributes().value("fileID").toString();
    QString name, ext, date, size;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
             xml.name().toString() == "file")) {
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "name") {
                xml.readNext();
                name = xml.text().toString();
            } else if (xml.name().toString() == "extension") {
                xml.readNext();
                ext = xml.text().toString();
                if (ext.isEmpty()) {
                    ext = "missing";
                }
            } else if (xml.name().toString() == "date") {
                xml.readNext();
                date = xml.text().toString();
            } else if (xml.name().toString() == "size") {
                xml.readNext();
                size = xml.text().toString();
            }
        }
        xml.readNext();
    }

    if (extension == "All" || ext == extension) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(fileID));
        items.append(new QStandardItem(name));
        items.append(new QStandardItem(ext));
        items.append(new QStandardItem(date));
        items.append(new QStandardItem(size));

        model->appendRow(items);
    }
}

void FillModel::openFile() {
    if (!this->file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file";
        return;
    }
}

void FillModel::close() {
    if (this->file.isOpen()) {
        this->file.close();
    }
}

QStandardItemModel *FillModel::getModel() const { return model; }

FillModel::FillModel()
    : file("../db.xml"), model(new CustomStandardItemModel()) {
    this->model->setHorizontalHeaderLabels(QStringList() << "fileID"
                                                         << "name"
                                                         << "extension"
                                                         << "date"
                                                         << "size");
}

FillModel::~FillModel() { close(); }