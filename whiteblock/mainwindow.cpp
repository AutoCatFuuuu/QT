#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->widget,&TestWidget::value,this,[=](int value){
        ui->label->setText(QString::number(value));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    ui->widget->start();
    ui->widget->setFocus();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->restart();
    ui->widget->setFocus();
}
