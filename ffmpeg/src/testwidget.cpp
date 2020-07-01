#include "testwidget.h"
#include "ui_testwidget.h"

#include <QPainter>
#include <QGridLayout>
#include <QTime>
#include <QDebug>

#define _SUSPENDED_WIDGET_HEIGHT_ 80
#define _DANMU_TIME_ 8.0
#define _DEM_ 150

XAudio::XAudio()
{
    fmt.setSampleRate(44100);   //采样率
    fmt.setSampleSize(16);      //样本大小
    fmt.setSampleType(QAudioFormat::UnSignedInt);   //数据类型
    fmt.setChannelCount(2);     //通道数
    fmt.setCodec("audio/pcm");  //解码格式
    fmt.setByteOrder(QAudioFormat::LittleEndian);   //端序

    pOutput = new QAudioOutput(fmt);
    pOutput->setVolume(1);
    pOut = NULL;
    isPlay = false;
    start();
}

XAudio::~XAudio()
{
    delete pOutput;
}

bool XAudio::start()
{
    if(!isPlay)
    {
        pOut = pOutput->start();
        isPlay = true;
        return true;
    }
    return false;
}

void XAudio::play()
{
    if(!pOut && !isPlay)
    {
        isPlay = true;
        mutex.lock();
        pOutput->resume();
        mutex.unlock();
    }
}

void XAudio::pause()
{
    if(!pOut && isPlay)
    {
        isPlay = false;
        mutex.lock();
        pOutput->suspend();
        mutex.unlock();
    }
}

bool XAudio::write(const char *data,int len)
{
    if(pOut && data)
    {
        int written = 0;
        mutex.lock();
        if(pOutput->bytesFree() > 0)
            written = pOut->write(data,len);
        mutex.unlock();
        return written>0 ? true:false;
    }
    return false;
}

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&TestWidget::videoshow);

    Atimer = new QTimer(this);
    connect(Atimer,&QTimer::timeout,this,&TestWidget::audioplay);

    pAudio = new XAudio;
    dMuKu = new DanMuKu;
    isPause = true;
    isOpen = false;
    isSPressed = false;
    isDanMu = true;

    timestamp = 0;
    InitWidget();
}

TestWidget::~TestWidget()
{
    delete ui;
    delete timer;
    delete Atimer;
    delete pAudio;
    delete dMuKu;
}

void TestWidget::InitWidget()
{
    //悬浮窗
    SuspendedWidget = new QWidget(this);
    //音量条
    VolumeSlider = new QSlider(this);
    VolumeSlider->setMaximum(100);
    VolumeSlider->setOrientation(Qt::Vertical);
    VolumeSlider->resize(25,100);

    QTimer::singleShot(100,this,[=](){
        VolumeSlider->hide();

        SuspendedWidget->setGeometry(0,height() - _SUSPENDED_WIDGET_HEIGHT_
                                     ,width(),_SUSPENDED_WIDGET_HEIGHT_);
        QWidget *w = new QWidget(SuspendedWidget);
        w->setObjectName("w");
        w->setStyleSheet("QWidget#w{"
                         "  background-color: rgb(190, 190, 190);"
                         "  border-radius: 5px;"
                         "}"
                         "QPushButton:hover{ "
                          "  background-color:rgb(170, 170, 170);"
                          "  border: 1px solid white;"
                          "}"
                          "QPushButton{border-radius:15px;}");

        QGridLayout *g = new QGridLayout(SuspendedWidget);
        g->addWidget(w,0,0,1,1);
        g->setMargin(5);
        //滚动条
        CurTimeSlider = new QSlider(w);
        CurTimeSlider->setMaximum(999);
        CurTimeSlider->setOrientation(Qt::Horizontal);

        connect(CurTimeSlider,&QSlider::sliderPressed,this,&TestWidget::SliderPressed);
        connect(CurTimeSlider,&QSlider::sliderReleased,this,&TestWidget::SliderReleased);
        //时间显示
        Timelab = new QLabel(w);
        Timelab->setMinimumWidth(120);
        Timelab->setText("00:00/00:00");
        Timelab->setAlignment(Qt::AlignCenter);
        //弹幕输入
        DanMuEdit = new QLineEdit(w);
        DanMuEdit->setMaximumHeight(30);
        DanMuEdit->setMinimumHeight(30);
        DanMuEdit->setStyleSheet("QLineEdit{"
                                 "  border: 1px solid white;"
                                 "  border-radius: 3px;"
                                 "  color: black;"
                                 "}");
        //弹幕发送
        DanMubtn = new QPushButton(w);
        DanMubtn->setText("发送");
        DanMubtn->setMaximumSize(80,30);
        DanMubtn->setMinimumSize(80,30);
        DanMubtn->setStyleSheet("QPushButton{"
                                "   border-radius:8px;"
                                "   border: 1px solid white;"
                                "   color: black;"
                                "}"
                                "QPushButton:hover{"
                                "   border: 3px solid white;"
                                "   color: white;"
                                "}");
        connect(DanMubtn,&QPushButton::clicked,[=](){
            if(!isOpen)
                return;

            QString text = DanMuEdit->text();
            if(!text.isEmpty()) {
                DanMuData d;
                QTime time;
                time = QTime::currentTime();
                qsrand(QTime(0,0,0).secsTo(time));
                d.index = m->getCDuration();
                d.arg[0] = 1;
                d.arg[1] = 0;
                d.arg[2] = qrand()%width();
                d.arg[3] = qrand()%height();
                d.list << "ff0000" << text;
                dMuKu->insertFile(d);
                DanMuEdit->clear();
            }
        });
        //弹幕开关
#if 0
        Filterbtn = new SwitchBtn(w);
        Filterbtn->setMinimumSize(60,30);
        Filterbtn->setMaximumSize(60,30);
        connect(Filterbtn,&SwitchBtn::clicked,[=](){
            if(isOpen) {
                //m->setAFilterOpen();
                isDanMu = !isDanMu;
            }
            });
#else
        DanMuSlider = new QSlider(w);
        DanMuSlider->setMaximumSize(30,30);
        DanMuSlider->setMaximum(1);
        DanMuSlider->setSingleStep(1);
        DanMuSlider->setPageStep(1);
        DanMuSlider->setValue(1);
        DanMuSlider->setOrientation(Qt::Horizontal);
        connect(DanMuSlider,&QSlider::valueChanged,[=](int value){
//            if(value == 0) {
                isDanMu = !isDanMu;
//            }
        });
#endif
        //音量按钮
        Volumebtn = new QPushButton(w);
        Volumebtn->setMaximumSize(30,30);
        Volumebtn->setMinimumSize(30,30);
        Volumebtn->setIcon(QIcon(":/close.png"));
        //打开文件
        Openbtn = new QPushButton(w);
        Openbtn->setMaximumSize(30,30);
        Openbtn->setMinimumSize(30,30);
        Openbtn->setIcon(QIcon(":/close.png"));

        connect(Openbtn,&QPushButton::clicked,this,&TestWidget::openvideo);
        connect(Volumebtn,&QPushButton::clicked,this,&TestWidget::VolumeShow);

        QGridLayout *wg = new QGridLayout(w);
        wg->setContentsMargins(11,0,11,0);
        wg->addWidget(CurTimeSlider,0,1,1,8);
        wg->addWidget(Timelab,0,9,1,3);
        //wg->addWidget(Filterbtn,1,1,1,1);
        wg->addWidget(DanMuEdit,1,1,1,7);
        wg->addWidget(DanMubtn,1,8,1,1);
        wg->addWidget(DanMuSlider,1,9,1,1);
        wg->addWidget(Volumebtn,1,10,1,1);
        wg->addWidget(Openbtn,1,11,1,1);
        //SuspendedWidget->hide();
    });

    connect(VolumeSlider,&QSlider::valueChanged,[=](int volume){
        pAudio->setVolume(volume/100.0f);
    });

}

void TestWidget::VolumeShow()
{
    static bool isshow = true;
    QPoint point = Volumebtn->mapTo(this,QPoint(0,0));
    VolumeSlider->move(point.x(),point.y() - VolumeSlider->height() - 10);
    VolumeSlider->setValue((int)(pAudio->getVolume() * 100));
    isshow ? VolumeSlider->show() : VolumeSlider->hide();
    isshow = !isshow;
}

void TestWidget::pause()
{
    isPause = true;
    timer->stop();
    Atimer->stop();
}

void TestWidget::start()
{
    timer->start();
    Atimer->start();
    isPause = false;
}

void TestWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        QWidget *w = qApp->widgetAt(QCursor::pos());
        QString name = w->objectName();
        if(name == "TestWidget") {
            if(!isPause)
                pause();
            else
                if(isOpen)
                    start();
        }
    }
}

void TestWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    SuspendedWidget->hide();
}

void TestWidget::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    SuspendedWidget->show();
}

void TestWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    SuspendedWidget->setGeometry(0,height()-_SUSPENDED_WIDGET_HEIGHT_
                                 ,width(),_SUSPENDED_WIDGET_HEIGHT_);
}

void TestWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    if(!img.isNull())
        painter.drawImage(0,0,img.scaled(size()));
    if(isDanMu)
        DrawDanMu(&painter);
}

void TestWidget::DrawDanMu(QPainter *painter)
{
    DanMuData *p = dMuKu->get();
    if(!p)
        return;

    QPen pen;
    QFont f; f.setPixelSize(22);
    painter->setFont(f);
    while(1) {
        if(!p || p->index > CDuration)
            break;
        int color = p->list.at(0).toInt(NULL,16);
        pen.setColor(QColor(color & 0xFF,((color >> 8) & 0xFF),(color >> 16)));
        painter->setPen(pen);
        if(p->arg[0] == 0) {
            painter->drawText(p->arg[2],p->arg[3],p->list.at(1));
        } else {
            //根据比例画弹幕
            int X = (_DEM_ - p->arg[1]) * width() / _DEM_;
            //int X =  p->arg[1] * width() / _DEM_;
            painter->drawText(X,p->arg[3],p->list.at(1));
        }
        p = p->next;
    }
}

void TestWidget::openvideo()
{
    if(!isOpen)
    {
        m = new AFFmpeg;
        //if(m->open("D:/Picture/avi/test.mp4"))
        if(m->open(path.toLocal8Bit()))
        {
            dMuKu->open();
            timer->start(1000/m->getFPS());
            Atimer->start(10);
            isOpen = true;
        }
        else
            qDebug() << "can't open.";
    }
}

void TestWidget::videoshow()
{
    if(isSPressed)
        return;

    int POs = m->readVideo();

    if(POs > 0)
    {
        AVFrame *f = m->getFrame();
        if(f)
        {
            img = QImage((uchar*)f->data[0],f->width,f->height,QImage::Format_RGB32);
            CDuration = m->getCDuration();
            if((int)CDuration > timestamp - 1) {
                dMuKu->read((int)CDuration);
                dMuKu->del(CDuration - _DANMU_TIME_);
                timestamp++;
            }
            dMuKu->update(CDuration);

            int minter = m->getDuration() / 60;
            int sec = m->getDuration() % 60;
            int cminter = CDuration / 60;
            int csec = (int)CDuration % 60;
            QString strtime = QString("%1:%2/%3:%4").arg(cminter).arg(csec,2,10,QLatin1Char('0')).arg(minter).arg(sec,2,10,QLatin1Char('0'));
            Timelab->setText(strtime);
            //qDebug() << m->getCDuration() << m->getDuration();
            CurTimeSlider->setValue(CDuration/m->getDuration() * 1000);
            update();
        }
    }
    else if(POs == -2)
    {
        qDebug() << "stop";
        timer->stop();
        Atimer->stop();
        isOpen = false;
        isPause = true;
        delete m;
    }
}

void TestWidget::audioplay()
{ 
    if(isSPressed)
        return;

    char *buf = NULL;
    int len = m->getAudio(&buf);
    if(len >= 0 && pAudio->getfreebytes() >= len)
    {
        //int size =
        m->readAudio();
        //qDebug() <<  len << freeByte <<  size;
        pAudio->write(buf,len);
    }
}

void TestWidget::SliderPressed()
{
    isSPressed = true;
}

void TestWidget::SliderReleased()
{
    float pos = CurTimeSlider->value()*1.0 /(CurTimeSlider->maximum() + 1);

    timestamp = m->seek(pos);
    qDebug() << timestamp;
    isSPressed = false;
}
