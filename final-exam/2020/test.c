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

int childPid;
void ctrC_handler(int sigNumber) {
        kill(childPid, 9);

}


int main(){
    pid_t pid, pid2;
    int wstat=0;
    childPid = fork();
    if(childPid == 0){
        while(1){
            printf("%d\n",45);
        }
    }else{
       sleep(3);
       wait(&wstat);
    }
    return 0;
}
