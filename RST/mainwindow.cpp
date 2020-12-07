#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QWidget(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->ReceptionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->ShipmentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::openDictionaryPressed()
{
    dialogWindow_.open();
}

void MainWindow::clearPressed()
{
    TableReception_.clear();
    ui_->ReceptionTable->setModel(&TableReception_);
    //ui_->ReceptionTable->resizeColumnsToContents();

    TableShipment_.clear();
    ui_->ShipmentTable->setModel(&TableShipment_);

}

void MainWindow::receptionEntered()
{
    QString enteredEntry = ui_->ReceptionInput->text();
    if(enteredEntry.size() <= 0)
    {
        return;
    }

    enteredEntry = enteredEntry.toUpper();
    QStringList entryList = enteredEntry.split(QRegularExpression("[\\s]+"), QString::SkipEmptyParts);
    entryList.removeDuplicates();
    int entryListLength = entryList.size();
    QStringList requestList;

    QRegularExpression hexMatcher("^[0-9A-F]{24}$", QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < entryListLength; ++i)
    {
        QRegularExpressionMatch match = hexMatcher.match(entryList.at(i));
        if (match.hasMatch())
        {
            //qDebug() << i << ": Entry text is hex string of length 24";
            requestList.append(entryList.at(i));
        }
        else
        {
            //qDebug() << i << ": Entry text is bad";
        }
    }

    if (requestList.size() <= 0)
    {
        return;
    }
    QMap<QString, QString> dictionaryPart = dialogWindow_.get_dictionary(&requestList);

    TableReception_.addEntries(&dictionaryPart);
    ui_->ReceptionTable->setModel(&TableReception_);
    ui_->ReceptionTable->resizeColumnsToContents();

    TableShipment_.removeEntries(&dictionaryPart);
    ui_->ShipmentTable->setModel(&TableShipment_);
}

void MainWindow::shipmentEntered()
{
    QString enteredEntry = ui_->ShipmentInput->text();
    if(enteredEntry.size() <= 0)
    {
        return;
    }

    enteredEntry = enteredEntry.toUpper();
    QStringList entryList = enteredEntry.split(QRegularExpression("[\\s]+"), QString::SkipEmptyParts);
    entryList.removeDuplicates();
    int entryListLength = entryList.size();
    QStringList requestList;

    QRegularExpression hexMatcher("^[0-9A-F]{24}$", QRegularExpression::CaseInsensitiveOption);

    for (int i = 0; i < entryListLength; ++i)
    {
        QRegularExpressionMatch match = hexMatcher.match(entryList.at(i));
        if (match.hasMatch())
        {
            //qDebug() << i << ": Entry text is hex string of length 24";
            requestList.append(entryList.at(i));
        }
        else
        {
            //qDebug() << i << ": Entry text is bad";
        }
    }

    if (requestList.size() <= 0)
    {
        return;
    }
    QMap<QString, QString> dictionaryPart = dialogWindow_.get_dictionary(&requestList);

    TableShipment_.addEntries(&dictionaryPart);
    ui_->ShipmentTable->setModel(&TableShipment_);
    ui_->ShipmentTable->resizeColumnsToContents();

    TableReception_.removeEntries(&dictionaryPart);
    ui_->ReceptionTable->setModel(&TableReception_);
}
