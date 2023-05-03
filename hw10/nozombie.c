/*
usage: ./NoZombie 10000
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

void manyChild(int num)
{
    int i, pid;
    for (int i = 0; i < num; i++)
    {
        pid = fork();
        if (pid == 0){
            sleep(i);
            exit(0);
        }
        if (pid > 0){
            continue;
        }
    }
}
int main(int argc, char **argv)
{
    int pid, num = 100;
    pid = fork();
    if (pid == 0){
        manyChild(num);
        exit(0);
    }
    else if (pid > 0){
        waitpid(pid, NULL, 0);
    }
    getchar();
}