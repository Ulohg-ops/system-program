/*
usage:
1st terminal 
$ ./zombie 10000

2nd terminal
$ ps -a
*/
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
 int num = 10, pid;
 char* cmd[] = {"ls", "-alh","/",NULL};
    for (int i = 0; i < num; i++)
    {
        pid = vfork();
        if (pid == 0){
            execvp("ls",cmd);
        }
        if (pid > 0){
            continue;
        }
    }
    if (pid != 0)
        getchar();
}