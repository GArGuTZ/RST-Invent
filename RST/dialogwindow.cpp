#include "dialogwindow.h"
#include "ui_dialogwindow.h"

DialogWindow::DialogWindow(QWidget *parent) :
    QDialog(parent),
    ui_(new Ui::DialogWindow)
{
    ui_->setupUi(this);
    //ui_->XMLTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

DialogWindow::~DialogWindow()
{
    delete ui_;
}

void DialogWindow::fill_Document()
{
    QString enteredEntry = this->ui_->EntryInput->text();
    if(enteredEntry.size() <= 0)
    {
        return;
    }

    enteredEntry = enteredEntry.toUpper();
    entryList_ = enteredEntry.split(QRegularExpression("[\\s]+"), QString::SkipEmptyParts);
    entryList_.removeDuplicates();
    int entryListLength = entryList_.size();
    //qDebug() << entryList;

    QDomElement root = enteredEntries_.createElement("dictionary");
    enteredEntries_.appendChild(root);

    QRegularExpression hexMatcher("^[0-9A-F]{24}$", QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < entryListLength; ++i)
    {
        QRegularExpressionMatch match = hexMatcher.match(entryList_.at(i));

        if (match.hasMatch())
        {
            //qDebug() << i << ": Entry text is hex string of length 24";

            QDomElement object = enteredEntries_.createElement("object");
            root.appendChild(object);

            QDomElement id = enteredEntries_.createElement("id");
            QDomText idText = enteredEntries_.createTextNode(entryList_.at(i));
            id.appendChild(idText);
            object.appendChild(id);

            QDomElement name = enteredEntries_.createElement("name");
            QDomText nameText = enteredEntries_.createTextNode("");
            name.appendChild(nameText);
            object.appendChild(name);
        }
        else
        {
            //qDebug() << i << ": Entry text is bad";
        }
    }

    //qDebug() << enteredEntries_.toString();
    if(!root.hasChildNodes())
    {
        enteredEntries_.clear();
    }
}

void DialogWindow::newXML()
{
    fill_Document();
    save();
}

void DialogWindow::save()
{
    //qDebug() << enteredEntries_.hasChildNodes();
    if(enteredEntries_.hasChildNodes())
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath(), "*.xml");

        QFile file(filename);
        file.open(QIODevice::WriteOnly);
        QTextStream out(&file);
        enteredEntries_.save(out, QDomNode::EncodingFromTextStream);
        enteredEntries_.clear();
        file.close();
    }
}

void DialogWindow::fill_DocumentFromTable()
{
    QMap<QString, QString> dictionary = TableModel_.get_Entries();
    int dictonaryLength = dictionary.size();
    if (dictionary.size() <= 0)
    {
        return;
    }

    QDomElement root = enteredEntries_.createElement("dictionary");
    enteredEntries_.appendChild(root);

    for (int i = 0; i < dictonaryLength; ++i)
    {
        QDomElement object = enteredEntries_.createElement("object");
        root.appendChild(object);

        QDomElement id = enteredEntries_.createElement("id");
        QDomText idText = enteredEntries_.createTextNode(dictionary.keys().at(i));
        id.appendChild(idText);
        object.appendChild(id);

        QDomElement name = enteredEntries_.createElement("name");
        QDomText nameText = enteredEntries_.createTextNode(dictionary.values().at(i));
        name.appendChild(nameText);
        object.appendChild(name);
    }

    //qDebug() << enteredEntries_.toString();
    if(!root.hasChildNodes())
    {
        enteredEntries_.clear();
    }
}

void DialogWindow::saveXML()
{
    fill_DocumentFromTable();
    save();
}

void DialogWindow::openXML()
{
    QString fileName = QDir::toNativeSeparators(dialog_.getOpenFileName(this, tr("Find Files"), QDir::currentPath(), "*.xml"));
    if (fileName.isEmpty())
    {
        return;
    }
    QFile file(fileName);

    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument parsingDocument;
    if (!parsingDocument.setContent(&file, true, &errorStr, &errorLine, &errorColumn))
    {
        qDebug() << "Line " << errorLine << ", Column " << errorColumn << ' ' << errorStr;
        return;
    }

    QDomElement root = parsingDocument.documentElement();
    if (root.tagName() != "dictionary")
    {
        qWarning("The file is not a dictionary");
        return;
    }

    QDomNode objectNode = root.firstChild();
    QMap<QString, QString> newList;
    while (!objectNode.isNull())
    {
        if (objectNode.toElement().tagName() == "object")
        {
            QString key;
            QString value;
            QDomNode objectChild = objectNode.firstChild();

            while (!objectChild.isNull())
            {
                if (objectChild.toElement().tagName() == "id")
                {
                    //qDebug() << "Approach id tag";
                    QDomNode childNode = objectChild.firstChild();
                    while (!childNode.isNull())
                    {
                        if (childNode.nodeType() == QDomNode::TextNode)
                        {
                            key = childNode.toText().data();
                            break;
                        }
                        childNode = childNode.nextSibling();
                    }
                }
                else if (objectChild.toElement().tagName() == "name")
                {
                    //qDebug() << "Approach name tag";
                    QDomNode childNode = objectChild.firstChild();
                    while (!childNode.isNull())
                    {
                        if (childNode.nodeType() == QDomNode::TextNode)
                        {
                            value = childNode.toText().data();
                            break;
                        }
                        childNode = childNode.nextSibling();
                    }
                }

                objectChild = objectChild.nextSibling();
            }

            if(key.size() > 0)
            {
                newList.insert(key, value);
                //qDebug() << "Loading " << key << ' ' << value;
            }

        }
        objectNode = objectNode.nextSibling();
    }

    //qDebug() << newList;
    if(newList.size() > 0)
    {
        TableModel_.addEntries(&newList);
        ui_->XMLTable->setModel(&TableModel_);
        ui_->XMLTable->resizeColumnsToContents();
    }
}

void DialogWindow::addEntry()
{
    QString enteredEntry = this->ui_->EntryInput->text();
    QStringList matchedEntries;
    enteredEntry = enteredEntry.toUpper();
    entryList_ = enteredEntry.split(QRegularExpression("[\\s]+"), QString::SkipEmptyParts);
    entryList_.removeDuplicates();
    int entryListLength = entryList_.size();
    //qDebug() << entryList;

    QRegularExpression hexMatcher("^[0-9A-F]{24}$", QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < entryListLength; ++i)
    {
        QRegularExpressionMatch match = hexMatcher.match(entryList_.at(i));

        if (match.hasMatch())
        {
            //qDebug() << i << ": Entry text is hex string of length 24";
            matchedEntries.append(entryList_.at(i));
        }
        else
        {
            //qDebug() << i << ": Entry text is bad";
        }
    }

    //qDebug() << matchedEntries;
    TableModel_.addEntries(&matchedEntries);
    ui_->XMLTable->setModel(&TableModel_);
    ui_->XMLTable->resizeColumnsToContents();

}

QMap<QString, QString> DialogWindow::get_dictionary(QStringList* _list)
{
    return TableModel_.get_ValidEntries(_list);
}
