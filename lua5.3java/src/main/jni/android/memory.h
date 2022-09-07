//
// Created by CrazyWu on 9/4/2022.
//

#ifndef ANDROIDLUA_MEMORY_H
#define ANDROIDLUA_MEMORY_H

int memory_read(int pid, void *lpAddress, int size);
int memory_write(int pid, void *lpAddress, int size, long data);

int memory_search();

#endif //ANDROIDLUA_MEMORY_H
