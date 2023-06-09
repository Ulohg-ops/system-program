/*執行方法：
./myShell
>> ls -R /
ctr-c
*/
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <setjmp.h>
#include <sys/resource.h> 

int main(){
    int fd[2], fd2[2];
    pid_t pid, pid2;
    char str[100];

    if(pipe(fd) != 0){
        fprintf(stderr, "Error pipe.\n");
        return 1;
    }
    if(pipe(fd2) != 0){
        fprintf(stderr, "Error pipe.\n");
        return 1;
    }

    do{
        pid = fork();
    }while(pid == -1 && errno == EAGAIN);

    if(pid == -1){
        perror("fork()");
        exit(1);
    }

    if(pid == 0){
        pid2 = fork();
        if(pid2 == 0){
            // fprintf(stderr, "1 - Grand Child\n");
            dup2(fd[1], STDOUT_FILENO); // 1);
            close(fd[0]);
            close(fd[1]);
            close(fd2[1]);
            close(fd2[0]);
            execlp("cat", "cat", "/etc/passwd", NULL);
            perror("cat error");
        }else{
            // wait(&pid2);
            // fprintf(stderr, "2 - Child\n");
            dup2(fd[0], STDIN_FILENO); //0);
            dup2(fd2[1], STDOUT_FILENO); // 1);
            close(fd[0]);
            close(fd[1]);
            close(fd2[1]);
            close(fd2[0]);
            execlp("grep", "grep", "1000", NULL);
            perror("grep failed");
        }
        return 1;
    }else{
        // wait(&pid);
        // fprintf(stderr, "3 - Parent\n");
        dup2(fd2[0], STDIN_FILENO); //0);
        close(fd[0]);
        close(fd[1]);
        close(fd2[1]);
        close(fd2[0]);
        execlp("cut", "cut", "-d:", "-f1", NULL);
        perror("cut failed");
        return 1;
    }
    return 0;
}
