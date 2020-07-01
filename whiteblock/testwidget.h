#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include "blockdata.h"

namespace Ui {
class TestWidget;
}

class TestWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TestWidget(QWidget *parent = 0);
    ~TestWidget();
    void setDSize(int width,int height);    //设置方块宽高
    void start();                           //开始游戏
    void restart();                         //重新开始
protected:
    void paintEvent(QPaintEvent *event);        //绘制界面
    void mousePressEvent(QMouseEvent *event);   //点击事件
    void keyReleaseEvent(QKeyEvent *event);     //键盘事件
signals:
    void failure();         //失败信号
    void value(int value);  //成绩信号
private slots:
    void updateData();      //更新数据
private:
    Ui::TestWidget *ui;
    QTimer timer;       //定时器
    BlockData bData;    //方块坐标数据类
    int step;           //步进速度
    int Dwidth;         //小方块宽度
    int Dheight;        //小方块高度
    bool isfailure;     //失败标志
    int sum;            //总分
};

#endif // TESTWIDGET_H
