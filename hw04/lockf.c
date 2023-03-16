#define _GNU_SOURCE
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <time.h>
#define BUF_SIZE 4096
int main(){
    int fd;
    int ret;
    int num;
    char buffer[BUF_SIZE];
    int offset=0;
    // fd = open("flock.db",O_WRONLY);
    fd = open("flock.db", O_RDWR, S_IRUSR| S_IWUSR);
	
    struct timespec n_sleep;
    n_sleep.tv_sec=0;//secondes sleep duration
    n_sleep.tv_nsec=100000000;    //nanoseconds sleep duration

    for(int i=0;i<1000;i++){
        nanosleep(&n_sleep,NULL);     
        lockf(fd,F_LOCK,0);
        lseek(fd,0,SEEK_END);
        lseek(fd,-5,SEEK_CUR);
        num=read(fd,buffer,5);
        sscanf(buffer, "%d", &offset);
        // printf("%d\n",offset);
        lseek(fd,offset,SEEK_END);
        sprintf(buffer, "%d", offset + 1);
        write(fd,buffer,5);
        lockf(fd,F_ULOCK,0);

    }
    getchar();
    return 0;
}