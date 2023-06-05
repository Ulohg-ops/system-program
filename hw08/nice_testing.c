#define _GNU_SOURCE
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>


int cpp;

void alarmHandler(int signo) {
    if(cpp>0){
        printf("Parent's cpp = %d\n", cpp);
    }else{
        printf("Child's cpp = %d\n", -cpp);
    }
    exit(0);
}

int main(int argc, char** argv){
     cpu_set_t mask; //CPU 核的集合
    CPU_ZERO(&mask);
    CPU_SET(1,&mask); //先做好參數設定,綁在第一顆處理器
    sched_setaffinity(0, sizeof(mask), &mask);
    int childPid = fork(); //產生二個行程
    if (childPid > 0) {//養我們的雙親
        nice(5); //修改 parent 的 nice 值 為 5
    } 
    else {
    //child 不用特別處理
    }
    // fork()會有以下三種回傳值：
    // -1 ： 發生錯誤
    // 0 ： 代表為子程序
    // 大於 0 ： 代表為父程序, 其回傳值為子程序的 ProcessID

    //底下的程式碼無論 child 是否大於 0 都會執行
    //設定鬧鐘(SIGALRM)叫的時候, 『作業系統』呼叫alarmHandler
    signal(SIGALRM, alarmHandler);
    //把鬧鐘的時間定在 1 秒鐘以後
    alarm(1);
    //不斷地 cpp++
    if(childPid>0){
        while (1) {
            cpp++;
        }
    }else{
        while(1){
            cpp--;
        }
        
    }
}
