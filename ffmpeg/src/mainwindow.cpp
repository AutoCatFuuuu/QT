#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setContextMenuPolicy(Qt::ActionsContextMenu);
    for(int i=0;i<16;i++)
    {
        TestWidget *w = new TestWidget(this);
        w->setVisible(false);
        Twlist << w;
    }

    QAction *a1 = new QAction("1",this);
    QAction *a4 = new QAction("4",this);
    QAlist << a1 << a4;

    foreach(QAction* a,QAlist){
        connect(a,&QAction::triggered,this,&MainWindow::QASlot);
    }

    addActions(QAlist);

    Twlist.at(0)->setPath("D:/Picture/avi/test.mp4");
    Twlist.at(1)->setPath("D:/Music/badapple.avi");
    Twlist.at(2)->setPath("D:/Picture/avi/video.mp4");
    Twlist.at(3)->setPath("D:/Picture/avi/test2.mp4");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setTWVisible(bool visible)
{
    for(int i=0;i<Twlist.size();i++)
        Twlist.at(i)->setVisible(visible);
}

void MainWindow::QASlot()
{
    QAction *a = (QAction *)sender();
    QString name = a->text();

    setTWVisible(false);
    if(name == "1")
    {
        ui->gridLayout->addWidget(Twlist.at(0),0,0);
        Twlist.at(0)->setVisible(true);
    }else if(name == "4") {
        ui->gridLayout->addWidget(Twlist.at(0),0,0);
        ui->gridLayout->addWidget(Twlist.at(1),0,1);
        ui->gridLayout->addWidget(Twlist.at(2),1,0);
        ui->gridLayout->addWidget(Twlist.at(3),1,1);
        for(int i=0;i<4;i++)
            Twlist.at(i)->setVisible(true);
    }
}
