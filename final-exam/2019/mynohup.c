#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

void sighandler(int signum){
}
int main(int argc, char **argv) {
    FILE *fp;
    int pipefd[2];
    int ret, wstat, pid1, pid2;
    pipe(pipefd);
    pid1 = fork();   //產生第一個child
    if (pid1==0) {
        close(1);   //關閉stdout
        dup(pipefd[1]); //將pipefd[1]複製到stdout
        close(pipefd[1]);   //將沒用到的關閉
        close(pipefd[0]);   //將沒用到的關閉
        execlp("ls", "ls",NULL);   //執行ls，ls會將東西藉由stdout輸出到pipefd[1]
    }
    if (pid1>0) {    
        pid2 = fork();//產生第二個child
        if (pid2==0) {
            char tem[20000];
            read(pipefd[0],tem,20000);
            close(0);   //關閉stdin
            dup(pipefd[0]); //將pipefd[0]複製到stdin
            close(pipefd[1]);   //將沒用到的關閉
            close(pipefd[0]);   //將沒用到的關閉
            fp=fopen("test.txt","a+");
            fwrite(tem,1,20000,fp);
            // execlp("wc","wc", NULL);   //執行wc，wc將透過stdin從pipefd[0]讀入資料
        }
    }
    //parent一定要記得關掉pipe不然wc不會結束（因為沒有接到EOF）
    close(pipefd[0]); close(pipefd[1]);
    printf("child %d\n",wait(&wstat));
    printf("child %d\n",wait(&wstat));
}

  // char info[5][100];
      //   char appendString[100];
      //   int word = 0;
      //   int j = 0;
      //   time_t curtime;
      //   time(&curtime);    
      //   fgets(buf, 100,input);
      //   for (int i = 0; i < strlen(buf); i++) {
      //       if (buf[i] == ' ') {
      //           info[word][j] = '\0';
      //           word++;
      //           j = 0;
      //       } else {
      //           info[word][j] = buf[i];
      //           j++;
      //       }
      //   }

