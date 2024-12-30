// 功能：本文件实现文件的读入和写入
// 函数定义写在file.h中
#include "file.h"

void print_Graph(SystemStatus *Graph)
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
    printf("avilable: ");
    for (int i = 0; i < Graph->resource_num; i++)
    {
        printf("%d", Graph->available[i]);
        printf(" ");
    }
    printf("\n");
}
// 增加
void read_file(const char *filename, SystemStatus *Graph)
{

    init_resource_allocation(Graph);
    // 从文件中读取资源分配图，并存入Graph中
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        return;
    }
    // 读取进程数量
    fscanf(fp, "%d", &Graph->num);
    char line[1024];
    fgetc(fp); // 读取换行符

    // 读取进程运行状态
    fgets(line, sizeof(line), fp);
    for (int i = 0; i < Graph->num; i++)
    {
        Graph->processes[i].state = line[i] - '0';
    }

    // 读取资源类型
    fgets(line, sizeof(line), fp);
    char *token = strtok(line, ",\n");
    int resource_count = 0;
    while (token != NULL && resource_count < MAX_RESOURCE)
    {
        Graph->max[resource_count++] = atoi(token);
        token = strtok(NULL, ",\n");
    }
    Graph->resource_num = resource_count;

    // 读取进程请求的资源数量
    fgets(line, sizeof(line), fp);
    token = strtok(line, ",\n"); // 作用是分割字符串  如100,010,000,001 分割后为100 010 000 001
    int i = 0;
    while (token != NULL && i < Graph->num)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {
            Graph->request[i][j] = token[j] - '0'; // 将字符转换为整数
        }
        i++;
        token = strtok(NULL, ",\n");
    }

    // 读取进程持有的资源数量
    fgets(line, sizeof(line), fp);
    token = strtok(line, ",\n");
    i = 0;
    while (token != NULL && i < Graph->num)
    {
        for (int j = 0; j < Graph->resource_num; j++)
        {                                             // 假设每个进程的资源持有数量是3个
            Graph->allocation[i][j] = token[j] - '0'; // 将字符转换为整数
        }
        i++;
        token = strtok(NULL, ",\n");
    }
    // 计算空闲资源
    calculate_free_resource(Graph);
    // 计算进程资源,申请资源数/请求资源数z
    calculate_process_resource(Graph);
    // 计算进程权重
    calculate_weight(Graph);
    print_Graph(Graph);
    fclose(fp);
    printf("文件读取成功!\n");
}

void write_file(const char *filename, SystemStatus *Graph)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL)
    {
        printf("无法打开文件 %s\n", filename);
        return;
    }

    // 写入进程数量
    fprintf(fp, "%d\n", Graph->num);

    // 写入进程运行状态
    for (int i = 0; i < Graph->num; i++)
    {
        fprintf(fp, "%d", Graph->processes[i].state);
    }
    fprintf(fp, "\n");

    // 写入资源数量
    int resource_count = 0;
    while (Graph->max[resource_count] > 0 && resource_count < MAX_RESOURCE)
    {
        if (resource_count > 0)
        {
            fprintf(fp, ",");
        }
        fprintf(fp, "%d", Graph->max[resource_count]);
        resource_count++;
    }
    Graph->resource_num = resource_count;
    fprintf(fp, "\n");

    // 写入进程请求资源数量
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < resource_count; j++)
        {
            fprintf(fp, "%d", Graph->request[i][j]);
        }
        if (i < Graph->num - 1)
        {
            fprintf(fp, ",");
        }
    }
    fprintf(fp, "\n");

    // 写入进程持有的资源数量
    for (int i = 0; i < Graph->num; i++)
    {
        for (int j = 0; j < resource_count; j++)
        {
            fprintf(fp, "%d", Graph->allocation[i][j]);
        }
        if (i < Graph->num - 1)
        {
            fprintf(fp, ",");
        }
    }
    // fprintf(fp, "\n");

    fclose(fp);
    printf("文件写入成功!\n");
}
