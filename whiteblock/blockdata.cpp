#include "blockdata.h"

BlockData::BlockData()
{
    head = NULL;
    tail = NULL;
}

BlockData::~BlockData()
{
    clear();
}

void BlockData::init(BData **d, int x, int y, int width, int height)
{
    BData *n = *d;
    n->x = x;
    n->y = y;
    n->width = width;
    n->height = height;
    n->next = NULL;
}

void BlockData::insert(BData *d)
{
    if(!tail) {
        head = d;
        tail = d;
        return;
    }
    tail->next = d;
    tail = tail->next;
}

bool BlockData::remove(int x,int y)
{
    if(!head)
        return false;

    if(x < head->x || head->x + head->width < x
            || y < head->y || head->y + head->height < y)
        return false;

    BData *n = head;
    //头尾相同
    if(n == tail) {
        head = tail = NULL;
    } else {
        head = head->next;
    }
    delete n;
    return true;
}

bool BlockData::remove(int x)
{
    if(!head)
        return false;

    if(x < head->x || head->x + head->width < x)
        return false;

    BData *n = head;
    //头尾相同
    if(n == tail) {
        head = tail = NULL;
    } else {
        head = head->next;
    }
    delete n;
    return true;
}

void BlockData::updata(int step)
{
    BData *n = head;
    while(n) {
        n->y += step;
        n = n->next;
    }
}

bool BlockData::judge(int y)
{
    if(head && head->y >= y)
        return false;
    return true;
}

void BlockData::clear()
{
    BData *n = head;
    while(n) {
        head = head->next;
        delete n;
        n = head;
    }
    tail = 0;
}

void BlockData::show()
{
    BData *n = head;
    while(n) {
        printf("%d,%d,%d,%d\n",n->x,n->y,n->width,n->height);
        n = n->next;
    }
    printf("====================\n");
    fflush(stdout);
}
