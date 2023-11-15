#ifndef CUSTOMSTANDARDITEMMODEL_H
#define CUSTOMSTANDARDITEMMODEL_H

#include <QStandardItemModel>

// CustomStandard sounds weirdly
class CustomStandardItemModel : public QStandardItemModel {
    Q_OBJECT

public:
    enum Roles { // enum classes are better
        FileIdRole = Qt::UserRole + 1,
        NameRole,
        ExtensionRole,
        DateRole,
        SizeRole
    };

    explicit CustomStandardItemModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif  // CUSTOMSTANDARDITEMMODEL_H
