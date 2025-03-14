#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include <QList>
class JsonItem {
public:
    JsonItem(const QVariant &value, JsonItem *parent = nullptr)
        : value(value), parentItem(parent) {}

    void appendChild(JsonItem *child) {
        childItems.append(child);
    }

    JsonItem *child(int row) {
        return childItems.value(row);
    }

    int childCount() const {
        return childItems.count();
    }

    QVariant getValue() const {
        return value;
    }

    // QJsonValue::Type getType() const {
    //     return type;
    // }

    // void setType(QJsonValue::Type newType) {
    //     type = newType;
    // }
    void setValue(QVariant newValue) {
        value = newValue;
    }


    JsonItem *parentItem;

private:
    QVariant value;
    // QJsonValue::Type type;
    QList<JsonItem*> childItems;
};

class JsonModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit JsonModel(QObject *parent = nullptr);
    void loadJson(const QString &filePath);
    void saveJson(const QString &filePath);
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
private:
    void parseJson(const QJsonValue &value, JsonItem *parentItem);
    // void traverseJson(const QJsonValue &value, JsonItem *parentItem);
    JsonItem *rootItem;
    QJsonDocument doc;
};

#endif // JSONMODEL_H
