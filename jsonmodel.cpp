#include "jsonmodel.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>

JsonModel::JsonModel(QObject *parent)
    : QAbstractItemModel(parent), rootItem(new JsonItem(QVariant())) {}

void JsonModel::loadJson(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning("Could not open JSON file.");
        return;
    }

    doc = QJsonDocument::fromJson(file.readAll());
    QJsonValue value;
    if (doc.isArray())
        value = QJsonValue(doc.array());
    else
        value = QJsonValue(doc.object());
    parseJson(value, rootItem);
    file.close();
}

void JsonModel::saveJson(const QString &filePath){
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Could not save JSON file.");
        return;
    }
    file.write(doc.toJson());
    file.close();
}

// void JsonModel::traverseJson(const QJsonValue &value, JsonItem *parentItem) {

// }
void JsonModel::parseJson(const QJsonValue &value, JsonItem *parentItem) {
    if (value.isObject()) {
        QJsonObject obj = value.toObject();

        foreach (const QString key, obj.keys()) {
            JsonItem *item = new JsonItem(key, parentItem);
            parentItem->appendChild(item);
            parseJson(obj.value(key), item);
        }
    } else if (value.isArray()) {
        QJsonArray array = value.toArray();
        for (int i = 0; i < array.size(); ++i) {
            JsonItem *item = new JsonItem(array[i].toVariant(), parentItem);
            // item->setType(array[i].toVariant().metaType());
            // item->setType(array[i].type());
            parentItem->appendChild(item);

        }
    } else {
        JsonItem *item = new JsonItem(value, parentItem);
        // item->setType(QJsonValue::fromVariant(value).type());
        parentItem->appendChild(item);

    }
}
bool JsonModel::hasChildren(const QModelIndex &index) const {
    if (!index.isValid())
        return false;
    JsonItem *item = static_cast<JsonItem*>(index.internalPointer());
    return item->childCount() > 0;
}
QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    JsonItem *parentItem = parent.isValid() ? static_cast<JsonItem*>(parent.internalPointer()) : rootItem;
    JsonItem *childItem = parentItem->child(row);
    return childItem ? createIndex(row, column, childItem) : QModelIndex();
}

QModelIndex JsonModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    JsonItem *childItem = static_cast<JsonItem*>(index.internalPointer());
    JsonItem *parentItem = childItem->parentItem;

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childCount(), 0, parentItem);
}

int JsonModel::rowCount(const QModelIndex &parent) const {
    JsonItem *parentItem = parent.isValid() ? static_cast<JsonItem*>(parent.internalPointer()) : rootItem;
    return parentItem->childCount();
}

int JsonModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant JsonModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    JsonItem *item = static_cast<JsonItem*>(index.internalPointer());
    if (role == Qt::DisplayRole) {
        return item->getValue();
    }
    return QVariant();
}

bool JsonModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid())
        return false;
    JsonItem *item = static_cast<JsonItem*>(index.internalPointer());
    beginResetModel();
    QJsonValue tempValue = QJsonValue::fromVariant(value);
    item->setValue(tempValue.toVariant());
    endResetModel();
    return true;

}
