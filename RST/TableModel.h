#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QDebug>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum MapRoles
    {
        KeyRole = Qt::UserRole + 1,
        ValueRole
    };

    explicit TableModel(QObject* _parent = 0);
    virtual int rowCount(const QModelIndex& _parent = QModelIndex()) const = 0;
    virtual int columnCount(const QModelIndex& _parent = QModelIndex()) const = 0;
    virtual QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const = 0;
    virtual QVariant headerData(int _section, Qt::Orientation _orientation, int _role) const = 0;
    virtual bool setData(const QModelIndex& _index, const QVariant& _value, int _role = Qt::EditRole) = 0;
    virtual Qt::ItemFlags flags(const QModelIndex& _index) const = 0;
};

class DictionaryTableModel: public TableModel
{
    Q_OBJECT
public:
    explicit DictionaryTableModel(QObject* _parent = 0);
    int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
    QVariant headerData(int _section, Qt::Orientation _orientation, int _role) const override;
    bool setData(const QModelIndex& _index, const QVariant& _value, int _role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& _index) const override;

    QMap<QString, QString> get_ValidEntries(QStringList* _keyList); // Function takes a list of keys & return QMap<key, name> of objects from dictionary
    QMap<QString, QString> get_Entries(); // Function returns all entries from dictionary
    void addEntries(QStringList* _list); // Used for adding entries from input to loaded dictionary
    void addEntries(QMap<QString, QString>* _list); // Used for adding entries from dictionary
    void removeEntries(QStringList* _list);
private:
    QMap<QString, QString> dictionaryMap_;
signals:
    //void editCompleted(const QString &);
};

class MainTableModel: public TableModel
{
    Q_OBJECT
public:
    explicit MainTableModel(QObject* _parent = 0);
    int rowCount(const QModelIndex& _parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& _parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& _index, int _role = Qt::DisplayRole) const override;
    QVariant headerData(int _section, Qt::Orientation _orientation, int _role) const override;
    bool setData(const QModelIndex& _index, const QVariant& _value, int _role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& _index) const override;

    void addEntries(QMap<QString, QString>* _list);
    void removeEntries(QStringList* _list);
private:
    QMap<QString, QString> dictionaryMap_;
    QMap<QString, int> tableMap_;
signals:
    //void editCompleted(const QString &);
};

#endif // TABLEMODEL_H
