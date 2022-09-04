//
// Created by CrazyWu on 9/4/2022.
//
#include "unistd.h"
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <lapi.h>
#include <dirent.h>
#include <errno.h>

#include "memory.h"
#include "androidlib.h"

int memory_read(int pid, void *lpAddress, int size)
{
    char processpath[100];
    sprintf(processpath, "/proc/%d/mem", pid);
    LOGD("open path is %s", processpath);
    int fd = open(processpath, O_RDONLY);
    LOGD("fd is %d", fd);
    lseek64(fd, (uintptr_t)lpAddress, SEEK_SET);
    unsigned char *buffer = (unsigned char *)malloc(size);
    int bread = read(fd, buffer, size);
    if (bread == -1)
    {
        LOGD("ReadProcessMemory fail\n");
        return -1;
    }
    close(fd);
    LOGD("ReadProcessMemory success, data is %d\n", buffer);
    LOGD("2");
    return *buffer;
}

uintptr_t safe_ptrace(int request, pid_t pid, void * addr, void * data)
{
#ifdef TRACEPTRACE
    // debug_log("ATTACH_TO_ACCESS_MEMORY=%d\n", ATTACH_TO_ACCESS_MEMORY);
  if (threadname)
    debug_log("%s: ptrace called (%s(%x), %d, %p, %p)\n",threadname, PTraceToString(request),request, pid, addr, data);
  else
    debug_log("ptrace called (%s(%x), %d, %p, %p)\n",PTraceToString(request),request, pid, addr, data);


#endif
    uintptr_t result;
    errno = 0;
    result = ptrace(request, pid, addr, data);
    if(errno != 0)
    {
        LOGD("ptrace error(%s (%d))!\n",strerror(errno), errno);
    }
    return result;
}

static int ptrace_attach_andwait(int pid)
//call this for quick attach/detach purposes. returns <0 on error, else the attached tid (usually just pid)
{
    if (safe_ptrace(PTRACE_ATTACH, pid,0,0)==0)
    {
        int status;
        while (1)
        {
            pid=waitpid(-1, &status,0);
            if (WIFSTOPPED(status))
            {
                if (WSTOPSIG(status)==SIGSTOP)
                    return pid; //proper stop

                //not a sigstop
                LOGD("ptrace_attach_andwait:Received stop with signal %d instead of %d\n", WSTOPSIG(status), SIGSTOP);
                safe_ptrace(PTRACE_CONT, pid, 0, WSTOPSIG(status));
                continue;
            }

            if (WIFCONTINUED(status))
            {
                LOGD("ptrace_attach_andwait:It already continued?\n");
                continue;
            }

            if (WIFEXITED(status))
            {
                LOGD("ptrace_attach_andwait:Target terminated with code %d\n", WEXITSTATUS(status));
                return -2; //target exit
            }

            if (WIFSIGNALED(status))
            {
                LOGD("trace_attach_andwait:Target received a ");

                if (WTERMSIG(status))
                    LOGD("terminate signal");

                if (WCOREDUMP(status))
                    LOGD("core Dump");

                LOGD("\n");
                return -3;
            }


            LOGD("ptrace_attach_andwait: Unexpected status: %x\n", status);
            return -4;
        }

    }
    else
    {
        LOGD("ptrace_attach_andwait: ptrace attach failed\n");
        return -1; //ptrace attach failed
    }

}

typedef struct
{
    unsigned long long baseAddress;
    void *next_ptr;
    int moduleSize;
    char *moduleName;

} ModuleListEntry, *PModuleListEntry;

PModuleListEntry get_process_map(int pid)
{
    int max = 64;
    char mapfile[255];
    FILE *f = NULL;
    snprintf(mapfile, 255, "/proc/%d/maps", pid);

    f = fopen(mapfile, "r");

    if (f)
    {
        char s[512];
        memset(s, 0, 512);
        PModuleListEntry _ml = NULL;
        PModuleListEntry ret = NULL;
        while (fgets(s, 511, f)) //read a line into s
        {
            char *currentModule;
            unsigned long long start, stop;
            char memoryrange[64], protectionstring[32], modulepath[511];
            uint32_t magic;

            modulepath[0] = '\0';
            memset(modulepath, 0, 511);

            sscanf(s, "%llx-%llx %s %*s %*s %*s %[^\t\n]\n", &start, &stop, protectionstring, modulepath);

            if (strchr(protectionstring, 's'))
                continue;
            int i;
            if (strcmp(modulepath, "[heap]") == 0) //not static enough to mark as a 'module'
                continue;

            // printf("%s\n", modulepath);

            if (strcmp(modulepath, "[vdso]") != 0) //temporary patch as to not rename vdso, because it is treated differently by the ce symbol loader
            {
                for (i = 0; modulepath[i]; i++) //strip square brackets from the name (conflicts with pointer notations)
                {
                    if ((modulepath[i] == '[') || (modulepath[i] == ']'))
                        modulepath[i] = '_';
                }
            }

//            i = ReadProcessMemory(pid, (void *)start, &magic, 4);
            if (i == 0)
            {
                // printf("%s is unreadable(%llx)\n", modulepath, start);
                continue; //unreadable
            }

            PModuleListEntry ml = (PModuleListEntry)malloc(sizeof(ModuleListEntry));
            if (ret == NULL)
            {
                ret = ml;
            }
            else
            {
                _ml->next_ptr = ml;
                _ml = ml;
            }
            ml->baseAddress = start;
            ml->moduleSize = stop - start;
            ml->moduleName = strdup(modulepath);
            ml->next_ptr = NULL;

            _ml = ml;
        }
        return ret;
    }
    return NULL;
}

//mem方式读取内存
//pid 目标进程pid
//lpAddress 目标进程内存地址
//buffer 读入数据的缓冲区
//size 读入数据大小
int ReadProcessMemory(int pid, void *lpAddress, void *buffer, int size)
{
    char processpath[100];
    sprintf(processpath, "/proc/%d/mem", pid);
    int fd = open(processpath, O_RDONLY);

    lseek64(fd, (uintptr_t)lpAddress, SEEK_SET);

    int bread = read(fd, buffer, size);

    if (bread == -1)
    {
        LOGD("function ReadProcessMemory fail\n");
        bread = 0;
    }
    close(fd);
    return bread;
}


static int system_memory_read(lua_State *L) {
    LOGD("memory read called");
    FILE *fp;
    char path[1035];

    fp = popen("id -u", "r");
    while (fgets(path, sizeof(path), fp) != NULL) {
        LOGD("C ROOT?: %s\r\n", path);
    }
    pclose(fp);

    // ptace attach the process
    int pid = 3073;
    unsigned long lpAddress = 0x10000000;
    int size = 100;
    unsigned char *buffer = (unsigned char *)malloc(size);
    ReadProcessMemory(pid, lpAddress, buffer, size);

    pid_t p = pid;
//    if (ptrace_attach_andwait(p) < 0) {
//        LOGD("attach process failed");
//        return -2;
//    }

    char* mem_path = "/proc/";
    char process_path[100];
    sprintf(process_path, "/proc/%d/mem", pid);
    LOGD("process_path is %s", process_path);
    errno = 0;
    int fd = open(process_path, O_RDONLY);
    LOGD("fd is %d", fd);
    if (fd == -1) {
        LOGD("open error(%s (%d))!\n",strerror(errno), errno);
        return -1;
    }

    int list = read(fd, buffer, size);
    LOGD("list is %d", list);

    read_proc_dir("/proc");

    return 0;


    lseek64(fd, (uintptr_t)lpAddress, SEEK_SET);

    int bread = read(fd, buffer, size);

    if (bread == -1)
    {
        LOGD("ReadProcessMemory fail\n");
        bread = 0;
    }
    close(fd);
    LOGD("data is : %s", buffer);

    return bread;
    return 0;
}


int read_proc_dir(char * path) {
    DIR * p_dir;
    struct dirent * entry;
    size_t len;
    char * sub_path;
    p_dir = opendir(path);
    if (p_dir == NULL)
    {
        printf("Can not open %s/n", path);
        return -1;
    }
    while((entry = readdir(p_dir)) != NULL)
    {
//        len = strlen(path) + strlen(entry->d_name) + 3;
        LOGD("file: %s", entry->d_name);
    }
    closedir(p_dir);
}