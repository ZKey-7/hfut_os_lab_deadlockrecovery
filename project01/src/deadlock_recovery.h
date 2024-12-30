// deadlock_recovery.h
#ifndef DEADLOCK_RECOVERY_H
#define DEADLOCK_RECOVERY_H
#include <stdio.h>
#include <time.h>
#define MAX_RESOURCE 100
#define MAX_PROCESS 100
#define MAX_AVAILABLE_RESOURCE 100

typedef struct {
    // 进程相关字段
    int pid;                   // 进程ID
    int res_holding;        // 持有的资源数量
    int res_request;   // 请求的资源数量
    int state;               // 进程状态(0:终止, 1:运行中)
    int weight;             // 进程权重(用于死锁恢复决策)
} Process;

typedef struct {
    //最大资源数量max
    int max[MAX_RESOURCE];
    //可用资源数组available
    int available[MAX_RESOURCE];
    //进程数量num
    int num;
    //资源数量
    int resource_num;
    //进程数组processes
    Process processes[MAX_PROCESS];
    //资源分配二维数组allocation
    int allocation[MAX_PROCESS][MAX_RESOURCE];
    //资源申请二维数组request
    int request[MAX_PROCESS][MAX_RESOURCE];
    //保存杀死进程的路径数组kill_path
    int kill_path[MAX_PROCESS];
    //kill_path的长度
    int kill_path_length;

} SystemStatus;

//打印系统状态
void print_system_status(SystemStatus *Graph);

//计算空闲资源
void calculate_free_resource(SystemStatus *Graph);

//检测进程是否全部满足请求
int check_request(SystemStatus *Graph);

//计算进程资源
void calculate_process_resource(SystemStatus *Graph);

//计算进程权重  
void calculate_weight(SystemStatus *Graph);

//通过权重排序进程
void sort_process_by_weight(SystemStatus *Graph);

//状态拷贝
SystemStatus copy_system_status(SystemStatus Graph);
//杀死进程后，得到新状态
SystemStatus kill_process_change(SystemStatus *Graph, int index);

// 初始化系统状态
void init_system_status(SystemStatus *Graph);

//资源分配图初始化
void init_resource_allocation(SystemStatus *Graph);

// 检测死锁
int detect_deadlock(SystemStatus *Graph);



// 通过广度优先搜索杀掉进程
void kill_process_bfs(SystemStatus *Graph);
//dfs遍历
void dfs_traversal(SystemStatus *Graph, int total_weight, SystemStatus *best_path, int *max_value);
// 通过深度优先搜索杀掉进程
void kill_process_dfs(SystemStatus *Graph);

// 随机生成死锁状态
void generate_deadlock_state(SystemStatus *Graph, int process_num, int resource_num, int available_resource);
#endif // DEADLOCK_RECOVERY_H;