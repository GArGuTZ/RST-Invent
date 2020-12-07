#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>

#include "dialogwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openDictionaryPressed();
    void clearPressed();
    void receptionEntered();
    void shipmentEntered();

private:
    DialogWindow dialogWindow_;
    //QMap<QString, QString>* dictionary_;

    QStringList enteredReception_;
    QStringList enteredShipment_;

    MainTableModel TableReception_;
    MainTableModel TableShipment_;

    Ui::MainWindow *ui_;
};
#endif // MAINWINDOW_H
