#include "CustomStandardItemModel.h"

CustomStandardItemModel::CustomStandardItemModel(QObject *parent)
    : QStandardItemModel(parent) {}

QVariant CustomStandardItemModel::data(const QModelIndex &index,
                                       int role) const {
    if (!index.isValid()) return QVariant();

    switch (role) {
        case FileIdRole:
            return QStandardItemModel::data(this->index(index.row(), 0),
                                            Qt::DisplayRole);
        case NameRole:
            return QStandardItemModel::data(this->index(index.row(), 1),
                                            Qt::DisplayRole);
        case ExtensionRole:
            return QStandardItemModel::data(this->index(index.row(), 2),
                                            Qt::DisplayRole);
        case DateRole:
            return QStandardItemModel::data(this->index(index.row(), 3),
                                            Qt::DisplayRole);
        case SizeRole:
            return QStandardItemModel::data(this->index(index.row(), 4),
                                            Qt::DisplayRole);
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> CustomStandardItemModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FileIdRole] = "fileID";
    roles[NameRole] = "name";
    roles[ExtensionRole] = "extension";
    roles[DateRole] = "date";
    roles[SizeRole] = "size";
    return roles;
}