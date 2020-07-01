#include "switchbtn.h"
#include "ui_switchbtn.h"

#include <QPainter>

SwitchBtn::SwitchBtn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SwitchBtn)
{
    ui->setupUi(this);
    isleft = true;
    btn = new QPushButton(this);
    btn->setStyleSheet("QPushButton{"
                       "    border-radius:15px;"
                       "    border: 0;"
                       "    background-color: qradialgradient(spread:pad, cx:0.5, cy:0.5, radius:1, fx:0.5, fy:0.5, stop:0 rgba(233, 233, 233, 255), stop:0.479904 rgba(233, 233, 233, 255), stop:0.522685 rgba(0, 0, 0, 255), stop:1 rgba(0, 0, 0, 255));"
                       "}");
    btn->move(0,0);
    connect(btn,&QPushButton::clicked,this,&SwitchBtn::btnclick);

    lanimation = new QPropertyAnimation(btn,"pos");
    connect(lanimation,&QPropertyAnimation::finished,[=](){
        timer->stop();
    });
    lanimation->setDuration(100);
    lanimation->setEndValue(pos());

    ranimation = new QPropertyAnimation(btn,"pos");
    connect(ranimation,&QPropertyAnimation::finished,[=](){
        timer->stop();
    });
    ranimation->setDuration(100);
    ranimation->setStartValue(pos());

    center = -1;
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&SwitchBtn::timerout);
}

SwitchBtn::~SwitchBtn()
{
    delete ranimation;
    delete lanimation;
    delete ui;
}

void SwitchBtn::paintEvent(QPaintEvent *event)
{
    QPoint p = btn->mapToParent(QPoint(btn->width()/2,btn->height()/2));
    center = p.x();

    QPainter painter(this);
    int w = width();
    int h = height();

    painter.setRenderHint(QPainter::HighQualityAntialiasing);
    //painter.setCompositionMode(QPainter::CompositionMode_Xor);

    painter.fillRect(rect(),QColor(255,255,255,1));
    QPainterPath path1;
    path1.moveTo(h/2,0);
    path1.arcTo(QRect(0,0,h,h),90,180);
    path1.moveTo(h/2,0);
    path1.lineTo(center,0);
    path1.lineTo(center,h-1);
    path1.lineTo(h/2,h-1);

    QPainterPath path2;
    path2.moveTo(w-h/2,h);
    path2.arcTo(QRect(w-h,0,h,h),-90,180);
    path2.moveTo(w-h/2,0);
    path2.lineTo(center,0);
    path2.lineTo(center,h-1);
    path2.lineTo(w-h/2,h-1);

    painter.setBrush(QBrush(QColor(100,100,100)));
    painter.drawPath(path1);
    painter.setBrush(QBrush(QColor(0,100,0)));
    painter.drawPath(path2);
}

void SwitchBtn::resizeEvent(QResizeEvent *event)
{
    int w = (width() - height())/2;

    if(!isleft)
        btn->move(width()-height(),0);
    btn->resize(height(),height());
    //btn->setMask(QRegion(0,0,btn->width(),btn->height(),QRegion::Ellipse));

    lanimation->setStartValue(QPoint(w,0));

    int right = width() - btn->width();
    ranimation->setEndValue(QPoint(right,0));
}

void SwitchBtn::btnclick()
{
    isleft?ranimation->start():lanimation->start();
    isleft = !isleft;
    timer->start(10);
    emit clicked();
}

void SwitchBtn::timerout()
{
    update();
}
