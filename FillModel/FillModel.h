#ifndef FILLMODEL_H
#define FILLMODEL_H

#include <CustomStandardItemModel.h>

#include <QDebug>
#include <QFile>
#include <QStandardItemModel>
#include <QXmlStreamReader>

class FillModel : public QObject {
    Q_OBJECT
private:
    QFile file;
    CustomStandardItemModel *model;

    void openFile();
    void close();

    void parseFile(QXmlStreamReader &xml, const QString &extension);

public:
    FillModel();
    ~FillModel();
    QStandardItemModel *getModel() const;
    void parseXML();
    void printModel();

public slots:
    void filterByExtension(const QString &extension);
};

#endif