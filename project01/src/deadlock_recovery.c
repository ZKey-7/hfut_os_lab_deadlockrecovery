// deadlock_recovery.c
#include "deadlock_recovery.h"
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
// 打印系统状态
void print_system_status(SystemStatus *Graph)
{
    printf("进程数量: %d\n", Graph->num);
    printf("进程状态: ");
    for (int i = 0; i < Graph->num; i++)
    {
        printf("%d ", Graph->processes[i].state);
    }
    printf("\n");
    printf("资源类型: ");
    for (int i = 0; i < Graph->resource_num; i++)
    {
        printf("%d ", Graph->max[i]);
    }
    printf("\n");
    printf("进程请求的资源数量: ");
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {
            printf("%d", Graph->request[i][j]);
        }
        printf(" ");
    }
    printf("\n");
    printf("进程持有的资源数量: ");
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {
            printf("%d", Graph->allocation[i][j]);
        }
        printf(" ");
    }
    printf("\n");

    // 长度kill_path_length
    printf("杀死进程个数: %d\n", Graph->kill_path_length);
    // 打印kill_path
    printf("杀死进程序列: ");
    for (int i = 0; i < Graph->kill_path_length; i++)
    {
        printf("%d ", Graph->kill_path[i]);
    }
    printf("\n");
    // available
    printf("available: ");
    for (int i = 0; i < Graph->resource_num; i++)
    {
        printf("%d ", Graph->available[i]);
    }
    printf("\n");
}

// 计算空闲资源
// 初始化available
void calculate_free_resource(SystemStatus *Graph)
{
    // 初始化空闲资源
    for (int i = 0; i < Graph->resource_num; i++)
    {
        Graph->available[i] = Graph->max[i];
    }
    // 每种资源的空闲资源 = 资源最大值-资源资源被占用数量
    // 遍历资源占用矩阵
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {
            Graph->available[j] -= Graph->allocation[i][j];
        }
    }
}
// 检测进程是否全部满足请求
// 如果所有进程都满足请求，则返回1，否则返回0
int check_is_all_request(SystemStatus *Graph)
{
    for (int i = 0; i < Graph->num; i++)
    {
        if (Graph->processes[i].state == 1)
        {
            return 0;
        }
    }
    return 1;
}
// 资源分配图初始化
void init_resource_allocation(SystemStatus *Graph)
{
    // 初始化进程数量
    Graph->num = 0;
    Graph->kill_path_length = 0;

    // 初始化最大资源数组和可用资源数组
    for (int i = 0; i < MAX_RESOURCE; i++)
    {
        Graph->max[i] = 0;
        Graph->available[i] = 0;
    }

    // 初始化进程数组
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        Graph->processes[i].pid = i;
        Graph->processes[i].res_holding = 0;
        Graph->processes[i].res_request = 0;
        Graph->processes[i].state = 0;
        Graph->processes[i].weight = 0;
        Graph->kill_path[i] = 0;
        Graph->resource_num = 0;

        // 初始化资源分配和请求矩阵
        for (int j = 0; j < MAX_RESOURCE; j++)
        {
            Graph->allocation[i][j] = 0;
            Graph->request[i][j] = 0;
        }
    }
}

// 检测死锁 时间复杂度O(n^2)
// 返回0表示未检测到死锁，返回1表示检测到死锁
int detect_deadlock(SystemStatus *Graph)
{
    // 实现死锁检测逻辑
    if (Graph->num == 0)
    {
        return 0;
    }
    printf("----检测死锁中----\n");
    int relese_num = 0;     // 记录被满足的进程数量
    int work[MAX_RESOURCE]; // 工作向量
                            // 保存当前进程状态，初始化已经满足的进程
    int current_state[Graph->num];
    for (int i = 0; i < Graph->num; i++)
    {
        if (Graph->processes[i].state == 0)
            relese_num++;
        current_state[i] = Graph->processes[i].state;
    }
    // work = available
    for (int i = 0; i < MAX_RESOURCE; i++)
    {
        work[i] = Graph->available[i];
    }

    // 打印work
    printf("work:");
    for (int i = 0; i < Graph->resource_num; i++)
    {
        printf("%d ", work[i]);
    }
    printf("\n");
    // 循环检测进程是否满足请求
    int last_relese_num = relese_num;
    while (1)
    {
        for (int i = 0; i < Graph->num; i++)
        {
            if (Graph->processes[i].state != 0)
            {
                // 如果进程状态不为0，则检查是否满足请求
                int pid = Graph->processes[i].pid;
                int flag = 1;
                for (int j = 0; j < Graph->resource_num; j++)
                {
                    if (Graph->request[pid][j] > work[j])
                    {
                        // 如果请求大于工作，则不满足请求
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1)
                {
                    // 如果满足请求，则释放垓进程占用的资源
                    for (int j = 0; j < Graph->resource_num; j++)
                    {
                        work[j] += Graph->allocation[pid][j];
                    }
                    printf("进程%d满足请求\n", pid);
                    printf("work:");
                    for (int j = 0; j < Graph->resource_num; j++)
                    {
                        printf("%d ", work[j]);
                    }
                    printf("\n");
                    Graph->processes[i].state = 0;
                    relese_num++; // 被满足的进程数量+1
                }
            }
        }
        // if(check_is_all_request(Graph) == 1) {
        //     break;
        // }
        // 如果所有进程都被满足了，退出
        if (relese_num == Graph->num)
        {
            break;
        }
        // 如果满足的进程数量没有变化，则说明Work不能满足���意进程的需求
        if (last_relese_num == relese_num)
        {
            break;
        }
        last_relese_num = relese_num;
    }
    //     if(check_is_all_request(Graph) == 1){
    //     return 0;
    // }
    // 恢复进程状态
    for (int i = 0; i < Graph->num; i++)
    {
        Graph->processes[i].state = current_state[i];
    }
    if (relese_num == Graph->num)
    {
        printf("检测到无死锁\n");
        printf("----检测死锁结束----\n");
        return 0; // 返回0表示无死锁
    }
    printf("检测到有死锁\n");
    printf("----检测死锁结束----\n");
    return 1; // 返回1表示检测到死锁
}
// 计算进程资源
void calculate_process_resource(SystemStatus *Graph)
{
    // 计算进程资源
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {
            Graph->processes[i].res_holding += Graph->allocation[i][j];
            Graph->processes[i].res_request += Graph->request[i][j];
        }
    }
}
// 权重计算
void calculate_weight(SystemStatus *Graph)
{
    // 某个进程权重计算公式：weight = 占有资源数目 - 请求资源数目
    for (int i = 0; i < Graph->num; i++)
    {
        Graph->processes[i].weight = Graph->processes[i].res_holding - Graph->processes[i].res_request;
    }
}
// 将进程按权重从大到小排序
void sort_process_by_weight(SystemStatus *Graph)
{
    for (int i = 0; i < Graph->num - 1; i++)
    {
        int maxIndex = i;
        for (int j = i + 1; j < Graph->num; j++)
        {
            if (Graph->processes[j].weight > Graph->processes[maxIndex].weight)
            {
                maxIndex = j;
            }
        }
        if (maxIndex != i)
        {
            Process temp = Graph->processes[i];
            Graph->processes[i] = Graph->processes[maxIndex];
            Graph->processes[maxIndex] = temp;
        }
    }
    // 打印排序后的进程
    printf("排序后的进程:");
    for (int i = 0; i < Graph->num; i++)
    {
        printf("进程%d,权重:%d\n", Graph->processes[i].pid, Graph->processes[i].weight);
    }
}
// 状态拷贝
SystemStatus copy_system_status(SystemStatus Graph)
{
    SystemStatus new_status;
    init_system_status(&new_status);
    // 拷贝进程数组
    for (int i = 0; i < Graph.num; i++)
    {
        new_status.processes[i] = Graph.processes[i];
    }
    // 拷贝资源数组
    for (int i = 0; i < Graph.resource_num; i++)
    {
        new_status.available[i] = Graph.available[i];
        new_status.max[i] = Graph.max[i];
    }
    // 拷贝请求和分配矩阵
    for (int i = 0; i < Graph.num; i++)
    {
        for (int j = 0; j < Graph.resource_num; j++)
        {
            new_status.request[i][j] = Graph.request[i][j];
            new_status.allocation[i][j] = Graph.allocation[i][j];
        }
    }
    // 拷贝其他变量
    new_status.num = Graph.num;
    new_status.resource_num = Graph.resource_num;
    new_status.kill_path_length = Graph.kill_path_length;
    // 拷贝kill_path数组
    for (int i = 0; i < Graph.kill_path_length; i++)
    {
        new_status.kill_path[i] = Graph.kill_path[i];
    }
    return new_status;
}
// 杀死进程后，得到新状态
SystemStatus kill_process_change(SystemStatus *Graph, int index)
{

    // 杀死进程后，新状态变化涉及process数组和available数组,kill_path数组,kill_path_length
    SystemStatus new_status = copy_system_status(*Graph);
    new_status.processes[index].state = 0;
    // 释放进程占用的资源
    int pid = new_status.processes[index].pid;
    for (int i = 0; i < Graph->resource_num; i++)
    {
        new_status.available[i] += Graph->allocation[pid][i];
    }
    new_status.kill_path[new_status.kill_path_length] = pid;
    new_status.kill_path_length++;
    return new_status;
}
// 通过广度优先搜索BFS杀掉进程
queue q;
void kill_process_bfs(SystemStatus *Graph)
{
    // 实现通过BFS杀掉进程的逻辑
    // sort_process_by_weight(Graph);
    // 建立队列进行bfs遍历
    // 如果没有死锁，则直接返回
    if (detect_deadlock(Graph) == 0)
    {
        printf("未检测到死锁\n");
        return;
    }
    printf("开始BFS遍历\n");
    init_queue(&q);
    offer(&q, *Graph);
    while (!is_empty(&q))
    {
        // 取队头，按权重排序。此时process[0]是权重最大的进程
        SystemStatus head = poll(&q);
        sort_process_by_weight(&head);
        // 从process[0]开始遍历，如果进程状态为1，则释放进程占用的资源
        // 释放资源后，将进程状态设置为0，并将其加入队列
        for (int i = 0; i < head.num; i++)
        {
            if (head.processes[i].state == 1)
            {

                // 直接使用栈上的变量
                SystemStatus new_status = kill_process_change(&head, i);

                printf("********杀死进程%d\n", head.processes[i].pid);
                // 打印新状态
                printf("********新状态:");
                print_system_status(&new_status);
                printf("\n");
                // 打印head的值
                printf("********head的值:");
                print_system_status(&head);
                printf("\n");
                // 新状态入队
                offer(&q, new_status);
                // 死锁检测
                if (detect_deadlock(&new_status) == 0)
                {
                    printf("$未检测到死锁$\n");
                    printf("最终状态\n");
                    print_system_status(&new_status);
                    return;
                }
            }
        }
    }
}

// 辅助函数dfs遍历
// 价值 = 权重和 / 杀掉进程数量 。 即权重越大，杀掉数量越小。价值越大
void dfs_traversal(SystemStatus *Graph, int total_weight, SystemStatus *best_path, int *max_value)
{
    // 如果没有死锁，更新路径
    if (detect_deadlock(Graph) == 0)
    {
        // 当前路径的价值 > 全局最大价值，则更新最优路径
        int value = total_weight / Graph->kill_path_length;
        if (value > *max_value)
        {
            *max_value = value;
            *best_path = copy_system_status(*Graph);
            // 输出满足条件的路径
            printf("满足条件的路径:");
            print_system_status(best_path);
            printf("\n");
        }
        return;
    }
    sort_process_by_weight(Graph); // 按权重从大到小排序，目的是为了优先杀死权重大的进程。使得杀死进程顺序合理
    // 打印当前状态
    printf(".当前状态:.\n");
    print_system_status(Graph);
    printf("\n");
    // 遍历所有进程，尝试终止进程
    for (int i = 0; i < Graph->num; i++)
    {

        if (Graph->processes[i].state == 1)
        {
            // 杀死进程
            printf("********杀死进程%d\n", Graph->processes[i].pid);
            SystemStatus new_status = kill_process_change(Graph, i);
            // dfs遍历新状态
            dfs_traversal(&new_status, total_weight + Graph->processes[i].weight, best_path, max_value);
        }
    }
}
// 通过深度优先搜索杀掉进程
void kill_process_dfs(SystemStatus *Graph)
{
    // 实现通过DFS杀掉进程的逻辑

    // 如果没有死锁，则直接返回
    if (detect_deadlock(Graph) == 0)
    {
        printf("未检测到死锁\n");
        return;
    }
    else
    {
        printf("检测到死锁\n");
    }
    // 输出提示
    printf("开始DFS遍历\n");
    // 初始化最优路径
    SystemStatus best_path;
    init_system_status(&best_path);
    // 初始化最大价值
    int max_value = -(MAX_RESOURCE * 2);
    dfs_traversal(Graph, 0, &best_path, &max_value);

    // 打印最优路径
    printf("DFS最优路径:");
    print_system_status(&best_path);
    printf("\n");
}

// 初始化系统状态
void init_system_status(SystemStatus *Graph)
{
    // 初始化进程数量
    Graph->num = 0;

    // 初始化资源数组
    for (int i = 0; i < MAX_RESOURCE; i++)
    {
        Graph->max[i] = 0;
        Graph->available[i] = 0;
    }

    // 初始化进程数组
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        Graph->processes[i].pid = i;
        Graph->processes[i].state = 0;
        Graph->processes[i].res_holding = 0;
        Graph->processes[i].res_request = 0;
        Graph->processes[i].weight = 0;
        Graph->resource_num = 0;
    }

    // 初始化资源分配和请求矩阵
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        for (int j = 0; j < MAX_RESOURCE; j++)
        {
            Graph->allocation[i][j] = 0;
            Graph->request[i][j] = 0;
        }
    }

    // 初始化死锁恢复相关数组
    Graph->kill_path_length = 0;
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        Graph->kill_path[i] = 0;
    }
}
// 辅助函数：返回两个数中的较小值
int min(int a, int b)
{
    return (a < b) ? a : b;
}
void generate_deadlock_state(SystemStatus *Graph, int process_num, int resource_num, int available_resource)
{
    printf("开始随机生成死锁状态\n");
    for (int times = 0; times < 1000; times++)
    {
        srand(time(NULL)); // 初始化随机数种子，使得每次运行程序生成的随机数不同
        init_system_status(Graph);
        // 设置进程数和资源数
        Graph->num = process_num;
        Graph->resource_num = resource_num;

        // 初始化资源最大数量（每种资源1-available_resource个）
        for (int i = 0; i < resource_num; i++)
        {
            Graph->max[i] = rand() % available_resource + 1;
            // printf("资源%d的最大数量: %d\n", i, Graph->max[i]);
        }

        // 初始化进程状态（都在运行）
        for (int i = 0; i < process_num; i++)
        {
            Graph->processes[i].pid = i;
            Graph->processes[i].state = 1;
        }

        // 为了确保死锁，我们先创建一个环形等待
        for (int i = 0; i < process_num; i++)
        {
            int next = (i + 1) % process_num; // 下一个进程，当i=process_num-1时，next=0
            // 进程i持有一些进程next需要的资源
            // 0-> n -> n - 1 -> n - 2 -> ... -> 1 -> 0 循环等待条件
            for (int j = 0; j < resource_num; j++)
            {
                // 确保分配的资源总量不超过最大值
                int remaining = Graph->max[j];
                // 计算已经分配的资源总量
                for (int k = 0; k < i; k++)
                {
                    remaining -= Graph->allocation[k][j];
                }
                if (remaining > 0)
                {
                    // 随机分配0到min(remaining, available_resource/2)个资源
                    int max_alloc = min(remaining, available_resource / 2);
                    Graph->allocation[i][j] = rand() % (max_alloc + 1);
                    if (Graph->allocation[i][j] > 0)
                    {
                        // 请求1到min(remaining, available_resource/3)个资源
                        int max_request = min(remaining, available_resource / 3);
                        Graph->request[next][j] = rand() % max_request + 1;
                    }
                }
            }
        }

        // 验证是否有死锁
        if (detect_deadlock(Graph) == 1)
        {
            printf("成功生成死锁状态！\n");
            // 计算空闲资源
            calculate_free_resource(Graph);
            // 计算进程资源
            calculate_process_resource(Graph);
            // 计算进程权重
            calculate_weight(Graph);
            printf("\n生成的死锁状态：\n");
            print_system_status(Graph);
            return;
        }
    }
    printf("未能生成死锁状态，请重试\n");
    return;
}
