#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <stdio.h>

struct BData{
    int x;
    int y;
    int width;
    int height;
    BData *next;
};

class BlockData
{
public:
    BlockData();
    ~BlockData();
    void init(BData **d,int x=0,int y=0,int width=0,int height=0);   //初始化
    void insert(BData *d);          //插入数据
    bool remove(int x,int y);       //删除数据
    bool remove(int x);             //删除数据
    void updata(int step);          //更新数据
    bool judge(int y);              //判断数据
    void clear();                   //清空数据
    BData* get(){ return head;}     //读取数据
    void show();                    //显示数据
private:
    BData *head;
    BData *tail;
};

#endif // BLOCKDATA_H
