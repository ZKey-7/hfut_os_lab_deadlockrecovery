#include <stdio.h>
#include "queue.h"
#include "deadlock_recovery.h"

//初始化队列
void init_queue(queue *q) {
    q->l = 0;
    q->r = 0;
    q->size = 0;
}
//判断队列是否为空
int is_empty(queue *q) {
    return q->l == q->r;
}
//判断队列是否已满
int is_full(queue *q) {
    return q->size == MAX;
}

//出队
SystemStatus poll(queue *q) {
    if(is_empty(q)){    
        printf("队列为空\n");
        SystemStatus empty;
        init_system_status(&empty);
        return empty;
    }
    SystemStatus res = q->Queue[q->l];
    if(q->l != MAX - 1){
        (q->l)++;
    }
    else{
        q->l = 0;
    }
    q->size--;
    return res;
}
//入队
void offer(queue *q, SystemStatus value) {
    if(is_full(q)){
        printf("队列已满");
        return;
    }
    q->Queue[q->r] = value;
    if(q->r != MAX - 1){
        (q->r)++;
    }
    else{
        q->r = 0;
    }
    q->size++;
}

//取队头元素
SystemStatus front(queue *q) {
    if(is_empty(q)){
        printf("队列为空");
        SystemStatus empty;
        init_system_status(&empty);  
        return empty;
    }
    return q->Queue[q->l];
}