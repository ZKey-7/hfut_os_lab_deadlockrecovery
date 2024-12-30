#include <stdio.h>
#include "deadlock_recovery.h"
#define MAX 100
typedef struct {
    //数组
    SystemStatus Queue[MAX];
    //队列头
    int l;
    //队列尾
    int r;
    //队列大小
    int size;
} queue;
//初始化队列
void init_queue(queue *q);  
//判断队列是否为空
int is_empty(queue *q);
//判断队列是否已满
int is_full(queue *q);
//出队
SystemStatus poll(queue *q);
//入队
void offer(queue *q, SystemStatus value);
//取队头元素
SystemStatus front(queue *q);


