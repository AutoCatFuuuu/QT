#include "mainwindow.h"
#include "ui_MainWindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::videoshow);
    isopen = false;
    g = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(g)
        delete g;
}

void MainWindow::on_pushButton_clicked()
{

    if(!isopen) {
        isopen = true;
        g = new Gdigrab;
        if(g->open()) {
            timer->start(1000 / g->getFPS());
            qDebug() << g->getFPS();
        }else {
            qDebug() << "error";
        }
    }
}

void MainWindow::videoshow()
{
    if(g->read()) {
        AVFrame *f = g->getFrame();
        if(f) {
            QImage img((uchar*)f->data[0],f->width,f->height,QImage::Format_ARGB32);
            ui->label->setPixmap(QPixmap::fromImage(img).scaled(ui->label->size()));
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{

    timer->stop();
    delete g;
    g = new Gdigrab;
    g->setOptions(10,10,480,680);
    g->open();
    timer->start();
}

void MainWindow::on_pushButton_3_clicked()
{
    timer->stop();
    g->stop();
}
