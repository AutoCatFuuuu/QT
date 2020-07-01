/****************************************************
 * 开两定时器分别去读取音频数据和视频数据
 ****************************************************/

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include "affmpeg.h"
#include <QWidget>
#include <QTimer>
#include <QMouseEvent>
#include <QAudioOutput>
#include <QMutex>
#include <QSlider>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "switchbtn.h"
#include "danmuku.h"

namespace Ui {
class TestWidget;
}

class XAudio
{
public:
    explicit XAudio();
    bool start();       //开启
    void play();        //播放
    void pause();       //暂停
    bool write(const char *data,int len);                       //写入数据
    int getfreebytes(){return pOutput->bytesFree();}            //获取数据剩余空间大小
    void setVolume(double volume){ pOutput->setVolume(volume);} //调节音量
    double getVolume(){ return pOutput->volume();}              //获取音量
    ~XAudio();
private:
    QAudioOutput *pOutput;  //输出
    QAudioFormat fmt;       //音频参数
    QMutex mutex;
    QIODevice *pOut;
    bool isPlay;            //启动标志
};


class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();
    void InitWidget();          //初始化悬浮窗体
    void setPath(QString p){path = p;}
protected:
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    void enterEvent(QEvent *event);
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
private slots:
    void openvideo();           //打开视频
    void videoshow();           //刷新视频
    void audioplay();           //刷新音频
    void SliderPressed();       //滚动条按下
    void SliderReleased();      //滚动条放开
    void VolumeShow();          //音量显示
    void pause();               //暂停
    void start();               //启动
private:
    void DrawDanMu(QPainter *painter); //弹幕

    Ui::TestWidget *ui;
    QTimer *timer;              //视频定时器
    QTimer *Atimer;             //音频定时器
    AFFmpeg *m;                 //视频处理
    XAudio *pAudio;             //音频播放器
    DanMuKu *dMuKu;             //弹幕处理

    bool isPause;               //暂停
    bool isOpen;                //打开
    bool isSPressed;            //滚动条按下暂停
    bool isDanMu;               //弹幕开关

    QSlider *VolumeSlider;      //音量滚动条
    QWidget *SuspendedWidget;   //悬浮窗体
    QPushButton *Volumebtn;     //音量按钮
    QPushButton *Openbtn;       //开始按钮
    SwitchBtn *Filterbtn;       //弹幕开关
    QLabel *Timelab;            //时间显示
    QSlider *CurTimeSlider;     //时间滚动条
    QLineEdit *DanMuEdit;       //弹幕输入
    QPushButton *DanMubtn;      //弹幕发送
    QSlider *DanMuSlider;       //弹幕开关
    QString path;               //播放内容路径
    QImage img;                 //显示
    int timestamp;              //当前时间戳
    double CDuration;           //当前时间戳
};

#endif // TESTWIDGET_H
