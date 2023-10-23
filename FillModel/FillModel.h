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

    void parseFile(QXmlStreamReader &xml, const QString &criteria,
                   const QString &text, const QString &extension);

public:
    FillModel();
    ~FillModel();

    QStandardItemModel *getModel() const;

    void parseXML();

public slots:
    void filter(const QString &criteria, const QString &text,
                const QString &extension);
};

#endif