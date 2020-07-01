#include "danmuku.h"

#define _DANMUKUPATH_ "./danmuku.xml"
#include <QDebug>
#include <QTime>

DanMuKu::DanMuKu()
{
    file = new QFile;
    head = new DanMuData;
    init(&head);
}
DanMuKu::~DanMuKu()
{
    file->close();
    delete file;
    clear();
    delete head;
}

void DanMuKu::open()
{
    file->setFileName(_DANMUKUPATH_);
    if(!file->exists()) {
        file->open(QFile::ReadOnly);
        file->close();
    }
    if(file->open(QFile::ReadWrite | QFile::Text)) {

    }
}

void DanMuKu::init(DanMuData **data)
{
   DanMuData *d = *data;
   d->index = 0.0;
   memset(d->arg,0,sizeof(d->arg));
   d->next = NULL;
}

void DanMuKu::read(int timestamp)
{
    double time = 0;        //读取的时间戳
    int index = 0;          //参数索引
    bool isTrue = false;    //找到一条弹幕的标识

    DanMuData *d = NULL;

    file->seek(0);
    QXmlStreamReader reader;
    reader.setDevice(file);

    while(!reader.atEnd()) {
        QXmlStreamReader::TokenType type = reader.readNext();

//        if (type == QXmlStreamReader::StartDocument) {
//            qDebug() << reader.documentEncoding() << reader.documentVersion();
//        }
        //xml 的头标签
        if (type == QXmlStreamReader::StartElement) {
            //标签里的属性
            if(reader.attributes().hasAttribute("timestamp")) {
                time = reader.attributes().value("timestamp").toDouble();

                if((int)time == timestamp) {
                    qDebug() << time << timestamp;
                    isTrue = true;
                    index = 0;
                    d = new DanMuData;
                    init(&d);
                    d->index = time;
                }
            }
        }
        if(isTrue) {
            //xml的尾标签
            if (type == QXmlStreamReader::EndElement) {
                if(reader.name() == "text") {
                    insert(d);
                    isTrue = false;
                }
            }
            //xml的标签内的值
            else if (type == QXmlStreamReader::Characters && !reader.isWhitespace()) {
                switch(index) {
                    case 0:case 1:case 2:case 3:
                        d->arg[index] = reader.text().toInt();break;
                    case 4:case 5:
                        d->list << reader.text().toString();break;
                    default:
                        break;
                }
                index ++;
            }
        }
//        if(reader.hasError()) {
//            qDebug() << reader.errorString();
//        }
    }
}

/********************
 *  下边都是C语言链表的基本操作了
 *  这里根据时间戳排序过的，目的是减少遍历时间
 *******************/

void DanMuKu::insert(DanMuData *data)
{
    DanMuData *n = head->next;
    DanMuData *p = head;
    while(1) {
        if(!n) {
            p->next = data;
            break;
        }
        if(n->index > data->index) {
            p->next = data;
            data->next = n;
            break;
        }
        p = n;
        n = n->next;
    }
}

int DanMuKu::insertFile(DanMuData data)
{
    file->seek(file->size() - 6);
    QTime time;
    time = QTime::currentTime();
    qsrand(QTime(0,0,0).secsTo(time));

    int x = qrand()%data.arg[2];
    int y = qrand()%data.arg[3];

    char str[512] = {0};
    sprintf(str,"<d timestamp=\"%lf\"> <type>%d</type> <time>0</time> <x>%d</x> <y>%d</y> <color>%s</color> <text>%s</text> </d>\n</i>\n",
            data.index,data.arg[0],x,y,data.list.at(0).toLatin1().data(),data.list.at(1).toUtf8().data());
    qint64 ret = file->write(str,strlen(str));
    file->flush();

    DanMuData *d = new DanMuData;
    memcpy(d->arg,&data.arg,sizeof(d->arg));
    d->list = data.list;
    d->index = data.index;
    d->next = NULL;
    insert(d);
    return ret;
}


void DanMuKu::update(double time,int step)
{
    DanMuData *p = head->next;
    while(1) {
        if(!p)
            break;
        //还没到时间的弹幕就不更新
        if(p->arg[0] == 1 && p->index < time) {
            p->arg[1] += step;
        }
        p = p->next;
    }
}

void DanMuKu::del(double timestamp)
{
    DanMuData *n = head->next;
    DanMuData *p = head;
    while(1) {
        if(!n || n->index > timestamp)
            break;
        if(n->index <= timestamp) {
            p->next = n->next;
            delete n;
            n = p->next;
            continue;
        }
        p = n;
        n = n->next;
    }
}

void DanMuKu::clear()
{
    DanMuData *n = head->next;
    DanMuData *p = NULL;
    while(1) {
        if(!n)
           break;
        else {
            p = n;
            n = n->next;
            delete p;
        }
    }
    head->next = NULL;
}

void DanMuKu::show()
{
    DanMuData *p = head->next;
    while(1) {
        if(!p)
            break;
        printf("%lf:type = %d,time = %d,x=%d,y=%d\n",
               p->index,p->arg[0],p->arg[1],p->arg[2],p->arg[3]);
        fflush(stdout);
        foreach(QString str,p->list) {
            qDebug() << str;
        }
        p = p->next;
    }
}
