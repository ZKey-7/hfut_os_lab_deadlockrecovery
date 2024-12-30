#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deadlock_recovery.h"
// 函数声明
void read_file(const char *filename, SystemStatus *Graph);
void write_file(const char *filename, SystemStatus *Graph);
void print_Graph(SystemStatus *Graph);
#endif 