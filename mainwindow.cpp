#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->btnLoad, SIGNAL(clicked()), this, SLOT(onLoadImage()));
    connect(ui->btnHand, SIGNAL(clicked()), ui->viewer, SLOT(onHandMode()));
    connect(ui->btnCross, SIGNAL(clicked()), ui->viewer, SLOT(onCrossMode()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onLoadImage()
{
    QImage img("C:/Users/zhaohr/Desktop/1.jpg");
    ui->viewer->setImage(&img);
}
