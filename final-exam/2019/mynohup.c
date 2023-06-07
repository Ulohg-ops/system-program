#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>


int cmdSize=0;
char* cmdArr[256];

char *ltrim(char *s)
{
   while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}
void sighandler(int signum){
}

void parseString(char* str) {
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        cmdArr[idx++] = retPtr;
        retPtr=strtok(NULL, " \n");
        cmdSize++;
    }
    cmdArr[idx]=NULL;
}

int main(int argc, char **argv) {
    // signal(SIGHUP,SIG_IGN);
    // freopen("dsa.out", "w+", stdout);  
    // for(int i=0;i<10;i++){
    //     sleep(1);
    //     printf("%d",i);
    // }
    // execlp("ls","ls",NULL);

    int pid1;
    int wstat;
    if(argc>=2){
        if(strcmp(argv[1],"--wait")==0){
            signal(SIGHUP,SIG_IGN);
        }
    }
    
    pid1=fork();
    if(pid1==0){
        int mypid=getpid();
        char fileName[256];
        char pidStirng[256];
        // strcat(fileName,"mynohup.");
        sprintf(fileName, "mynohup.%d.out", mypid);
        // strcat(fileName,pidStirng); 
        // strcat(fileName,".out");
        // printf("%s",fileName);
        freopen(fileName, "w+", stdout);  
        sleep(10);
        signal(SIGHUP,SIG_IGN);
        execlp("ls","ls",NULL);
    }else{
        wait(&wstat);
    }
}
