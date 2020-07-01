/*************************************
 *  实现思路:
 *  1. 每秒更新一次数据 读取当前一秒内的所有弹幕
 *      PS：用double类型去更新太不准了 而且更新太频繁还是不要吧
 *  2. 每帧视频更新弹幕的显示坐标 达到滚动的效果
 *      PS: 滚动步进可以根据实际显示宽度自行计算
 *  3. 删除 第 n 秒前的弹幕数据
 *      PS: n 值可以自行确定
 *************************************/


#ifndef DANMUKU_H
#define DANMUKU_H

#include <QFile>
#include <QXmlStreamReader>
#include <QMultiHash>

struct DanMuData{
    double index;     //时间戳索引
    int arg[4];
//    int type;       //弹幕类型 固定位置:滚动 0:1
//    int time;       //滚动弹幕实时位置
//    int x;          //X轴
//    int y;          //Y轴
    QStringList list;
//    int color;      //颜色 rgb值
//    char text[256]; //内容
    DanMuData *next;
};

class  DanMuKu
{
public:
    DanMuKu();
    ~DanMuKu();
    void open();                            //打开文件
    void init(DanMuData **data);            //初始化结构体
    void read(int timestamp);               //读取数据
    void insert(DanMuData *data);           //插入数据
    int insertFile(DanMuData data);         //插入数据
    void update(double time,int step=1);    //更新数据
    void del(double timestamp);             //删除数据
    void clear();                           //清空数据
    void show();                            //打印数据
    DanMuData* get(){ return head->next;}   //获取数据
private:
    QFile *file;                            //弹幕文件
    DanMuData *head;                        //数据
};

#endif // DANMUKU_H
