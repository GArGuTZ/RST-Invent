#include "TableModel.h"

TableModel::TableModel(QObject* _parent): QAbstractTableModel(_parent) {}

DictionaryTableModel::DictionaryTableModel(QObject* _parent): TableModel(_parent) {}

MainTableModel::MainTableModel(QObject* _parent): TableModel(_parent) {}

int DictionaryTableModel::rowCount(const QModelIndex& _parent) const
{
    return dictionaryMap_.size();
}

int DictionaryTableModel::columnCount(const QModelIndex& _parent) const
{
    return 2;
}

QVariant DictionaryTableModel::data(const QModelIndex& _index, int _role) const
{
    if (_index.row() < 0 || _index.row() >= dictionaryMap_.size())
    {
        return QVariant();
    }

    if( _role == Qt::DisplayRole)
    {
        if (_index.column() == 0)
        {
            return dictionaryMap_.keys().at(_index.row());
        }
        if (_index.column() == 1)
        {
            return dictionaryMap_.values().at(_index.row());
        }
    }
    if( _role == Qt::EditRole)
    {
        if (_index.column() == 0)
        {
            return dictionaryMap_.keys().at(_index.row());
        }
        if (_index.column() == 1)
        {
            return dictionaryMap_.values().at(_index.row());
        }
    }
    return QVariant();
}

QVariant DictionaryTableModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
{
    if (_role == Qt::DisplayRole && _orientation == Qt::Horizontal)
    {
        switch (_section)
        {
        case 0:
            return QString("Id");
        case 1:
            return QString("Наименование");
        }
    }
    return QVariant();
}

bool DictionaryTableModel::setData(const QModelIndex& _index, const QVariant& _value, int _role)
{

    if (_role == Qt::EditRole)
    {
        if (!checkIndex(_index))
        {
            return false;
        }

        dictionaryMap_.insert(dictionaryMap_.keys().at(_index.row()), _value.toString());
        emit dataChanged(_index, _index, {_role});

        return true;
    }
    return false;
}

Qt::ItemFlags DictionaryTableModel::flags(const QModelIndex& _index) const
{
    if(_index.column() == 0)
    {
        return QAbstractTableModel::flags(_index);
    }
    else
    {
        return Qt::ItemIsEditable | QAbstractTableModel::flags(_index);
    }
}

QMap<QString, QString> DictionaryTableModel::get_ValidEntries(QStringList* _keyList)
{
    QMap<QString, QString> validEntries;
    int entryListLength = _keyList->size();
    if (entryListLength <= 0)
    {
        return validEntries;
    }

    for (int i = 0; i < entryListLength; ++i)
    {
        if (dictionaryMap_.contains(_keyList->at(i)))
        {
            validEntries.insert(_keyList->at(i), dictionaryMap_.value(_keyList->at(i)));
        }
    }
    qDebug() << "Getting ValidEntries: " << validEntries;
    return validEntries;
}

QMap<QString, QString> DictionaryTableModel::get_Entries()
{
    return dictionaryMap_;
}

void DictionaryTableModel::addEntries(QStringList* _list)
{
    emit layoutAboutToBeChanged();
    int entryListLength = _list->size();
    for (int i = 0; i < entryListLength; ++i)
    {
        if (!dictionaryMap_.contains(_list->at(i)))
        {
            dictionaryMap_.insert(_list->at(i), "");
        }
    }
    emit layoutChanged();
}

void DictionaryTableModel::addEntries(QMap<QString, QString>* _list)
{
    emit layoutAboutToBeChanged();
    dictionaryMap_.clear();
    int entryListLength = _list->size();
    for (int i = 0; i < entryListLength; ++i)
    {
        dictionaryMap_.insert(_list->keys().at(i), _list->values().at(i));
    }
    emit layoutChanged();
}

void DictionaryTableModel::removeEntry(int _position, const QModelIndex& _parent)
{
    beginRemoveRows(QModelIndex(), _position, _position);
    dictionaryMap_.remove(dictionaryMap_.keys().at(_position));
    //qDebug() << "Delete row " << _position;
    endRemoveRows();
}

int MainTableModel::rowCount(const QModelIndex& parent) const
{
    return tableMap_.size();
}

int MainTableModel::columnCount(const QModelIndex & parent) const
{
    return 2;
}

void MainTableModel::addEntries(QMap<QString, QString>* _list)
{
    emit layoutAboutToBeChanged();
    int entryListLength = _list->size();
    for (int i = 0; i < entryListLength; ++i)
    {
        if (tableMap_.contains(_list->keys().at(i)))
        {
            ++tableMap_[_list->keys().at(i)];
        }
        else
        {
            tableMap_.insert(_list->keys().at(i), 1);
            dictionaryMap_.insert(_list->keys().at(i), _list->values().at(i));
        }
    }
    emit layoutChanged();
}

void MainTableModel::removeEntries(QMap<QString, QString>* _list)
{
    emit layoutAboutToBeChanged();
    int entryListLength = _list->size();
    for (int i = 0; i < entryListLength; ++i)
    {
        if (tableMap_.contains(_list->keys().at(i)))
        {
            --tableMap_[_list->keys().at(i)];
        }
        if(tableMap_.value(_list->keys().at(i)) <= 0)
        {
            tableMap_.remove(_list->keys().at(i));
            dictionaryMap_.remove(_list->keys().at(i));
        }
    }
    emit layoutChanged();
}

void MainTableModel::clear()
{
    emit layoutAboutToBeChanged();
    tableMap_.clear();
    dictionaryMap_.clear();
    emit layoutChanged();
}

QVariant MainTableModel::data(const QModelIndex& index, int role) const
{
    if (index.row() < 0 || index.row() >= tableMap_.size() || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    if (index.column() == 0)
    {
        return dictionaryMap_.values().at(index.row());
    }
    if (index.column() == 1)
    {
        return tableMap_.values().at(index.row());
    }
    return QVariant();
}

QVariant MainTableModel::headerData(int _section, Qt::Orientation _orientation, int _role) const
{
    if (_role == Qt::DisplayRole && _orientation == Qt::Horizontal)
    {
        switch (_section)
        {
        case 0:
            return QString("Наименование");
        case 1:
            return QString("Количество");
        }
    }
    return QVariant();
}

bool MainTableModel::setData(const QModelIndex& _index, const QVariant& _value, int _role)
{
    return false;
}

Qt::ItemFlags MainTableModel::flags(const QModelIndex& _index) const
{
    return QAbstractTableModel::flags(_index);
}
