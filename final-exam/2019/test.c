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
///color///
#define NONE "\033[m"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"
#define RED_BOLD "\x1b[;31;1m"
#define BLU_BOLD "\x1b[;34;1m"
#define YEL_BOLD "\x1b[;33;1m"
#define GRN_BOLD "\x1b[;32;1m"
#define CYAN_BOLD_ITALIC "\x1b[;36;1;3m"
#define RESET "\x1b[0;m"

/*
全域變數，放解析過後的使用者指令（字串陣列）
*/
char* cmdArr[10][256];
char* argVect[256];
char* cmdString[256];
int cmdParaNumber[256];
int cmdNumber=0;
int cmdIdx=0;

//下列三個變數作為main和signal handler溝通用
sigjmp_buf jumpBuf;
volatile sig_atomic_t hasChild = 0;
pid_t childPid;
const long long nspersec = 1000000000;

long long timespec2sec(struct timespec ts) {
    long long ns = ts.tv_nsec;
    ns += ts.tv_sec * nspersec;
    return ns;
    //return (double)ns/1000000000.0;
}

double timeval2sec(struct timeval input) {
    long long us = input.tv_sec*1000000;
    us += input.tv_usec;
    //printf("%ldsec, %ld us\n", input.tv_sec, input.tv_usec);
    return (double)us/1000000.0;
}

/*signal handler專門用來處理ctr-c*/
void ctrC_handler(int sigNumber) {
    if (hasChild) {
        kill(childPid, sigNumber);
        hasChild = 0;
    } else {
        /*確認main function並不是剛好在處理字串，這裡使用一個隱含的同步方法*/
        /*藉由確認是否argVect[0]（即執行檔）是否為NULL保證main function不是在處理字串*/
        /*主程式的控制迴圈必須在一開始的地方將argVect[0]設為NULL*/
        if (argVect[0] == NULL) {
            ungetc('\n', stdin);ungetc('c', stdin);ungetc('^', stdin);
            siglongjmp(jumpBuf, 1);
        } else {
            /*極少發生，剛好在處理字串，忽略這個signal，印出訊息提示一下*/
            fprintf(stderr, "info, 處理字串時使用者按下ctr-c\n");
        }
    }
}


char *ltrim(char *s){ 
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
void parseStringAndSplit(char* str) {
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        cmdArr[cmdIdx][idx++] = retPtr;
        retPtr=strtok(NULL, " \n");
        cmdParaNumber[cmdIdx]++;
    }
    cmdArr[cmdIdx++][idx]=NULL;

}
void parseStringByPipe(char* str) {
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, "|\n");
    while(retPtr != NULL) {
        cmdString[idx++] = retPtr;
        retPtr=strtok(NULL, "|\n");
        cmdNumber++;
    }
    cmdString[idx]=NULL;
}
/*
parseString：將使用者傳進的命令轉換成字串陣列
str：使用者傳進的命令
cmd：回傳執行檔
*/
void parseString(char* str, char** cmd) {
    int idx=0;
    char* retPtr;
    //printf("%s\n", str);
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        //printf("token =%s\n", retPtr);
        //if(strlen(retPtr)==0) continue;
        argVect[idx++] = retPtr;
        if (idx==1)
            *cmd = retPtr;
        retPtr=strtok(NULL, " \n");
    }
    argVect[idx]=NULL;
}

/*
請參考myshell2.c的註解
*/

int main (int argc, char** argv) {
    memset(cmdArr,'\0',sizeof(cmdArr));
    memset(cmdParaNumber,'\0',sizeof(cmdParaNumber));
    memset(cmdString,'\0',sizeof(cmdString));
    char cmdLine[4096];
    char hostName[256];
    char cwd[256];
    char* exeName;
    int pid, pos, wstatus;
    struct rusage resUsage;     //資源使用率
    struct timespec statTime, endTime;
    /*底下程式碼註冊signal的處理方式*/
    signal(SIGINT, ctrC_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    /*無窮迴圈直到使用者輸入exit*/
    while(1) {

        char* showPath;
        char* loginName;
        int homeLen = 0;
        //設定化hasChild, argVect[0]，避免發生race condtion
        hasChild = 0;
        argVect[0]=NULL;
        //抓取主機名稱、用戶名稱
        cmdNumber=0;
        loginName = getlogin();
        gethostname(hostName, 256);
        /*
        底下程式碼製造要顯示的路徑字串，
        會參考"home"將"home"路徑取代為~
        */
        getcwd(cwd, 256);
        pos=strspn(getenv("HOME"), cwd);
        homeLen = strlen(getenv("HOME"));
        //printf("cwd=%s, home=%s, pos=%d, prompt=%s", cwd, getenv("HOME"), pos, &cwd[pos]);
        if(pos>=homeLen) {
            cwd[pos-1]='~';
            showPath=&cwd[pos-1];
        }
        else
            showPath=cwd;   
        /*
        底下程式碼負責印出提示符號
        
        */
        printf(LIGHT_GREEN"%s@%s:", loginName, hostName);
        printf(BLU_BOLD"%s>> " NONE, showPath);
        //printf();
        //設定返回地點，如果使用者按下ctr-c會從sigsetjmp的下一行開始執行
        sigsetjmp(jumpBuf, 1);
        /*
        接收使用者命令，除了cd, exit以外，其他指令呼叫對應的執行檔
        */
        fgets(cmdLine, 4096, stdin);
        //printf("cmdLine = %s\n",cmdLine);
        
        if (strlen(cmdLine)>1){
            parseStringByPipe(cmdLine);//判斷長度是否大於1，判斷「使用者無聊按下enter鍵」
            printf("%d",cmdNumber);
            for(int i=0;i<cmdNumber;i++){
                parseStringAndSplit(trim(cmdString[i]));
            }
        }else{
            continue;
        }



        // for(int i=0;i<cmdNumber;i++){
        //     for(int j=0;j<cmdParaNumber[i];j++){
        //         printf("%s",cmdArr[i][j]);
        //     }
        //         printf("\n");
        // }

        if (cmdNumber==1&&strcmp(cmdArr[0][0], "^c") == 0) {   //使用者按下control-c，^c是由signal handler放入
            //printf("ctr-c \n");
            printf("\n");
            continue;
        }

        if (cmdNumber==1&&strcmp(cmdArr[0][0], "exit") == 0)   //內建指令exit
            break;
        if (cmdNumber==1&&strcmp(cmdArr[0][0], "cd") == 0) {
               //內建指令cd
            if (strcmp(argVect[1], "~")==0)
                chdir(getenv("HOME"));
            else
                chdir(argVect[1]);
            continue;
        }
        if (cmdNumber==1&&strcmp(cmdArr[0][0], "stderr") == 0) {
         
            
        }else{
            
        }
        clock_gettime(CLOCK_MONOTONIC, &statTime);
        pid = fork();   //除了exit, cd，其餘為外部指令
        printf("pid: %d\n",pid);
        if (pid == 0) {
            int pipefd[2];
            int ret, wstat;
            pipe(pipefd);
            int pid1 = fork();   //產生第一個child
            if (pid1==0) {
                close(1);   //關閉stdout
                dup(pipefd[1]); //將pipefd[1]複製到stdout
                close(pipefd[1]);   //將沒用到的關閉
                close(pipefd[0]);   //將沒用到的關閉
                execvp(cmdArr[0][0], cmdArr[0]);//執行ls，ls會將東西藉由stdout輸出到pipefd[1]
            }else{    
                int pid2 = fork();//產生第二個child
                if (pid2==0) {
                    close(0);   //關閉stdin
                    dup(pipefd[0]); //將pipefd[0]複製到stdin
                    close(pipefd[1]);   //將沒用到的關閉
                    close(pipefd[0]);   //將沒用到的關閉
                    execvp(cmdArr[1][0],cmdArr[1]);   //執行wc，wc將透過stdin從pipefd[0]讀入資料
                }
            }   
            close(pipefd[0]); 
            close(pipefd[1]);
            printf("wait for pid1: %d\n",wait(&wstat));//for pid1
            printf("wait for pid2: %d\n",wait(&wstat));//for pid2
            exit(0);
        } else {
            /*
            parent(myShell)等待child完成命令
            完成命令後，parent將child的結束回傳值印出來
            */
            childPid = pid;/*通知singal handler，如果使用者按下ctr-c時，要處理這個child*/
            hasChild = 1;/*通知singal handler，正在處理child*/
            printf("wait for pid: %d\n",wait(&wstatus));//for pid
            //  printf("wait for pid: %d\n",wait3(&wstatus, 0, &resUsage));//for pid2       ;
            clock_gettime(CLOCK_MONOTONIC, &endTime);
            //wait(&wstatus);
            //int ret=getrusage(RUSAGE_CHILDREN, &resUsage);
            //printf("ret = %d\n", ret);
            double sysTime = timeval2sec(resUsage.ru_stime);
            double usrTime = timeval2sec(resUsage.ru_utime);
            //printf("%ld\n", endTime.tv_nsec);
            //printf("%ld\n", statTime.tv_nsec);
            printf("\n");
            long long elapse = timespec2sec(endTime)-timespec2sec(statTime);
            printf(RED"經過時間:                       "YELLOW"%lld.%llds\n",elapse/nspersec, elapse%nspersec);
            printf(RED"CPU花在執行程式的時間:	         "YELLOW"%fs\n"
                   RED"CPU於usr mode執行此程式所花的時間："YELLOW"%fs\n"
                   RED"CPU於krl mode執行此程式所花的時間："YELLOW"%fs\n", sysTime+usrTime , usrTime, sysTime);
            printf(RED"Page fault，但沒有造成I/O：      "YELLOW"%ld\n", resUsage.ru_minflt);
            printf(RED"Page fault，並且觸發I/O:        "YELLOW"%ld\n", resUsage.ru_majflt);
            printf(RED"自願性的context switch：        "YELLOW"%ld\n", resUsage.ru_nvcsw);
            printf(RED"非自願性的context switch:       "YELLOW"%ld\n", resUsage.ru_nivcsw);
            printf(RED "return value of " YELLOW "%s" RED " is " YELLOW "%d\n", 
                exeName, WEXITSTATUS(wstatus));
            //printf("isSignaled? %d\n", WIFSIGNALED(wstatus));
            if (WIFSIGNALED(wstatus))
                printf(RED"the child process was terminated by a signal "YELLOW"%d"RED
                    ", named " YELLOW "%s.\n",  WTERMSIG(wstatus), strsignal(WTERMSIG(wstatus)));
            printf(NONE);
        }
    }
}