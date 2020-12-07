#ifndef DIALOGWINDOW_H
#define DIALOGWINDOW_H

#include <QDialog>
#include <QDomDocument>
#include <QDir>
#include <QFileDialog>

#include "TableModel.h"

namespace Ui {
class DialogWindow;
}

class DialogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWindow(QWidget *parent = nullptr);
    bool is_XML_Exist();
    QMap<QString, QString> get_dictionary(QStringList* _list);
    ~DialogWindow();

private slots:
    void newXML(); // Used for creating new dictionary from input
    void saveXML(); // Used for creating new dictionary from opened table
    void openXML(); // Used for loading dictionary
    void addEntry(); // Used for adding entries from input to the opened table
    void removeEntries();

private:
    void fill_Document(); // Function takes user input & builds DomDocument
    void fill_DocumentFromTable(); // Function takes opened table & builds DomDocument
    void save(); // Function takes DomDocument & save it in new file

    QFileDialog dialog_;
    QStringList entryList_;
    QDomDocument enteredEntries_;
    QDomDocument loadedXML_;

    DictionaryTableModel TableModel_;
    Ui::DialogWindow *ui_;
};

#endif // DIALOGWINDOW_H
