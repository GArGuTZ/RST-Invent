#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QWidget(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    ui_->ShipmentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_->ReceptionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::openDictionaryPressed()
{
    //dialogWindow_.setWindowModality(Qt::WindowModal);
    //dialogWindow_.setModal(true);
    dialogWindow_.open();
}

void MainWindow::clearPressed()
{

}

void MainWindow::receptionEntered()
{

}

void MainWindow::shipmentEntered()
{

}
