#include <stdio.h>
#include <windows.h>
#include "file.h"

//用户界面
void user_interface(SystemStatus* Graph) {
    int choice;
    char filename[256];
  
    while(1) {
        printf("\n=== DeadLock Detection and Recovery ===\n");
        printf("1. 读取文件\n");
        printf("2. 写入文件\n");
        printf("3. 测试文件读写\n");
        printf("4. 检测死锁\n");
        printf("5. DFS方式解除死锁的代价最小算法\n");
        printf("6. BFS方式解除死锁的代价最小算法\n");
        printf("7. 随机生成死锁状态\n");
        printf("8. 退出\n");
        printf("请输入您的选择 (1-8): ");
        fflush(stdout);
        
        if(scanf("%d", &choice) != 1) {
            printf("输入错误!\n");
            fflush(stdout);
            // 清除输入缓冲区
            while(getchar() != '\n');
            continue;
        }
        
        switch(choice) {
            case 1:
                printf("请输入要读取的文件名：");
                scanf("%s", filename);
                read_file(filename, Graph);
                break;
            case 2:
                
                    int write_choice;
                    printf("\n=== 写入文件方式 ===\n");
                    printf("1. 手动输入状态写入\n");
                    printf("2. 自动写入当前状态\n");
                    printf("请选择写入方式 (1-2): ");
                    scanf("%d", &write_choice);
                    
                    printf("请输入要写入的文件名：");
                    scanf("%s", filename);
                    
            switch(write_choice) {
                case 1:  // 手动创建
                printf("请输入要写入的文件名：");
                scanf("%s", filename);
                //请用户输入相关数据
                printf("请输入进程数量 (1-%d): ", MAX_PROCESS);
                scanf("%d", &Graph->num);
                while(Graph->num <= 0 || Graph->num > MAX_PROCESS) {
                    printf("进程数量无效,请重新输入 (1-%d): ", MAX_PROCESS);
                    scanf("%d", &Graph->num);
                }

                printf("请输入每个进程的状态(0表示终止,1表示运行)\n");
                for(int i = 0; i < Graph->num; i++) {
                    printf("进程 %d 的状态: ", i);
                    scanf("%d", &Graph->processes[i].state);
                    while(Graph->processes[i].state != 0 && Graph->processes[i].state != 1) {
                        printf("状态无效,请重新输入进程 %d 的状态(0或1): ", i);
                        scanf("%d", &Graph->processes[i].state);
                    }
                }

                printf("请输入资源种类数量 (1-%d): ", MAX_RESOURCE);
                int resource_types;
                scanf("%d", &resource_types);
                while(resource_types <= 0 || resource_types > MAX_RESOURCE) {
                    printf("资源种类数量无效,请重新输入 (1-%d): ", MAX_RESOURCE);
                    scanf("%d", &resource_types);
                }

                printf("请输入每种资源的最大数量:\n");
                for(int i = 0; i < resource_types; i++) {
                    printf("资源 %d 的数量: ", i);
                    scanf("%d", &Graph->max[i]);
                    while(Graph->max[i] <= 0) {
                        printf("资源数量必须为正数,请重新输入资源 %d 的数量: ", i);
                        scanf("%d", &Graph->max[i]);
                    }
                }

                printf("请输入每个进程请求的资源数量:\n");
                for(int i = 0; i < Graph->num; i++) {
                    for(int j = 0; j < resource_types; j++) {
                        printf("进程 %d 请求资源 %d 的数量: ", i, j);
                        scanf("%d", &Graph->request[i][j]);
                        while(Graph->request[i][j] < 0 || Graph->request[i][j] > Graph->max[j]) {
                            printf("请求数量无效,请重新输入(0-%d): ", Graph->max[j]);
                            scanf("%d", &Graph->request[i][j]);
                        }
                    }
                }

                printf("请输入每个进程已分配的资源数量:\n");
                for(int i = 0; i < Graph->num; i++) {
                    for(int j = 0; j < resource_types; j++) {
                        printf("进程 %d 已分配资源 %d 的数量: ", i, j);
                        scanf("%d", &Graph->allocation[i][j]);
                        while(Graph->allocation[i][j] < 0 || Graph->allocation[i][j] > Graph->max[j]) {
                            printf("分配数量无效,请重新输入(0-%d): ", Graph->max[j]);
                            scanf("%d", &Graph->allocation[i][j]);
                        }
                    }
                }
                            break;
                            
                        case 2:  // 自动写入
                            write_file(filename, Graph);
                            break;
                            
                        default:
                            printf("无效的选择\n");
                            break;
                    
                }
                break;
            case 3:
                // 测试文件读写
                printf("\n=== 测试文件读写 ===\n");
                // 1. 先读取DATA1.txt
                printf("读取DATA1.txt...\n");
                read_file("DATA1.txt", Graph);
                
                // 2. 将读取的数据写入新文件test_output.txt
                printf("\n将数据写入test_output.txt...\n");
                write_file("test_output.txt", Graph);
                
                // 3. 读取DATA2.txt
                printf("\n读取DATA2.txt...\n");
                read_file("DATA2.txt", Graph);
                
                // 4. 将数据写入另一个新文件test_output2.txt
                printf("\n将数据写入test_output2.txt...\n");
                write_file("test_output2.txt", Graph);
                
                printf("\n测试完成！请检查test_output.txt和test_output2.txt文件\n");
                break;
            case 4:
                 int result = detect_deadlock(Graph);
                 if(result == 0) {
                    printf("系统无死锁\n");
                 } else {
                    printf("系统有死锁\n");
                 }
                break;
            case 5:
                 kill_process_dfs(Graph);
                break;
            case 6:
             kill_process_bfs(Graph);
                break;
            case 7:
                 {
                     int process_num, resource_num, available_resource;
                     printf("请输入进程数量 (1-%d): ", 6);
                     scanf("%d", &process_num);
                     printf("请输入资源类型数量 (1-%d): ", MAX_RESOURCE);
                     scanf("%d", &resource_num);
                     printf("请输入资源量最大值: (1-%d)", 9);
                     scanf("%d", &available_resource);
                     generate_deadlock_state(Graph, process_num, resource_num, available_resource);
                 }
                 break;
            case 8:
                 return;
            default:
                printf("无效的选择，请重新输入\n");
        }
    }
}

SystemStatus Graph;
int main() {
    // 设置控制台输出为 UTF-8
    SetConsoleOutputCP(CP_UTF8);

    printf("程序启动...\n");
    fflush(stdout);


    init_system_status(&Graph); // 初始化Graph结构体

    user_interface(&Graph);  // 调用用户界面

    printf("\n程序结束，按回车键退出...");
    fflush(stdout);
    getchar();  // 删除可能的换行符
    getchar();  // 等待用户按回车

    return 0;
} 