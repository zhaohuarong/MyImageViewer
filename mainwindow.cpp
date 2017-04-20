#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLoad_clicked()
{
    QImage img("C:/Users/zhaohr/Desktop/2.jpg");
    ui->viewer->setImage(&img);
}

void MainWindow::on_btnHand_clicked()
{
    ui->viewer->setCurrentMode(Hand_Move_Mode);
}

void MainWindow::on_btnCross_clicked()
{
    ui->viewer->setCurrentMode(Selected_Zoom_Mode);
}
