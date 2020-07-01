#if 1
#include <QApplication>
#include "mainwindow.h"

#include "danmuku.h"
#include <QDebug>

int main(int argc, char** argv)
{
#if 1
    QApplication a(argc,argv);

    MainWindow w;
    w.show();

    return a.exec();
#else
    DanMuKu D;
    D.open();
//    D.read(98.552);
//    D.show();
    DanMuData d;
    d.index = 90.488;
    d.arg[0] = 1;
    d.arg[1] = 0;
    d.arg[2] = 960;
    d.arg[3] = 480;
    d.list << "ffffff" << "这是一个测试";
    qDebug() << D.insertFile(d);
#endif
}

#else
//生成测试数据用的
#include <QTime>
#include <QFile>
#include <QDebug>

int main(){
    QTime time;

    QFile file("D:/QTtest/build-ffmpeg-Desktop_Qt_5_7_0_MinGW_32bit-Debug/danmuku.xml");
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    int num = 1000;
    time = QTime::currentTime();
    qsrand(QTime(0,0,0).secsTo(time));
    file.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<i>\n");
    while (num>0) {
        int timestamp = qrand()%120;
        int decimal = qrand()%1000;
        //int type = qrand()%2;
        int x = qrand()%960;
        int y = qrand()%540;
        int color = qrand()%0xFFFFFF;

        char str[256]={0};
        sprintf(str,"<d timestamp=\"%d.%d\"> <type>1</type> <time>0</time> <x>%d</x> <y>%d</y> <color>%s</color> <text>过于真实 !!!</text> </d>\n",
                timestamp,decimal,x,y,QString("%1").arg(color,6,16,QLatin1Char('0')).toLatin1().data());
        num--;
        file.write(str,strlen(str));
        //qDebug() << str;
    }
    file.write("</i>");
    file.close();

}

#endif
































