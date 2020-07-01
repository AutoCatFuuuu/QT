#if 1

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

#else

#include "aencode.h"
#include <QDebug>

int main()
{
    AEncode E;
    E.setPath("D:/test.mp4");
    E.initOutFile();
    return 0;
}

#endif
