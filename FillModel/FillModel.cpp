#include "FillModel.h"

#include <QtConcurrent/QtConcurrent>

void FillModel::parseXML() {
    openFile();

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "file") {
                parseFile(xml, "All", "", "");
            }
        }
    }
    close();
}

void FillModel::filter(const QString &criteria, const QString &text,
                       const QString &extension) {
    model->clear();

    openFile();

    QXmlStreamReader xml(&file);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }

        if (token == QXmlStreamReader::StartElement) {
            if (xml.name().toString() == "file") {
                parseFile(xml, criteria, text, extension);
            }
        }
    }
    close();
}

bool matchesCriteria(const QString &criteria, const QString &name,
                     const QString &date, const QString &size,
                     const QString &text) {
    return ((criteria == "name" && name.contains(text, Qt::CaseInsensitive)) ||
            (criteria == "date" && date.contains(text, Qt::CaseInsensitive)) ||
            (criteria == "size" && size.contains(text, Qt::CaseInsensitive)));
}

bool matchesExtension(const QString &ext, const QString &extension) {
    return extension == "All" || ext.contains(extension);
}

void FillModel::parseFile(QXmlStreamReader &xml, const QString &criteria,
                          const QString &text, const QString &extension) {
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

    qDebug() << fileID << name << ext << date << size;

    if ((text.isEmpty() || matchesCriteria(criteria, name, date, size, text)) &&
        matchesExtension(ext, extension)) {
        QList<QStandardItem *> items;
        items.append(new QStandardItem(fileID));
        items.append(new QStandardItem(name));
        items.append(new QStandardItem(ext));
        items.append(new QStandardItem(date));
        items.append(new QStandardItem(size));

        model->appendRow(items);
        // We can use emit Model
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