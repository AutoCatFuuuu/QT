#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include "testwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setTWVisible(bool visible);
private slots:
    void QASlot();
private:
    Ui::MainWindow *ui;
    QList<TestWidget *> Twlist;
    QList<QAction*> QAlist;
    QGridLayout *g;
};

#endif // MAINWINDOW_H
