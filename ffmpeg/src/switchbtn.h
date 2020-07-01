/******************************
 * 自定义开关按钮 不太好看
 ******************************/


#ifndef SWITCHBTN_H
#define SWITCHBTN_H

#include <QWidget>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QTimer>

namespace Ui {
class SwitchBtn;
}

class SwitchBtn : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchBtn(QWidget *parent = 0);
    ~SwitchBtn();
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
private slots:
    void btnclick();
    void timerout();
signals:
    void clicked();
private:
    Ui::SwitchBtn *ui;
    QPushButton *btn;
    QPropertyAnimation *lanimation;
    QPropertyAnimation *ranimation;
    bool isleft;
    int center;
    QTimer *timer;
};

#endif // SWITCHBTN_H
