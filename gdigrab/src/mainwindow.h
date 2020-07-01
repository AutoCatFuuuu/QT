/***************************
 * 这里是个展示窗口(测试窗口)
 ***************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "gdigrab.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:

private slots:
    void on_pushButton_clicked();
    void videoshow();
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    Gdigrab *g;
    QTimer *timer;
    bool isopen;

};

#endif // MAINWINDOW_H
