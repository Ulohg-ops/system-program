#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

char* argVect[256];

/*
 parseString：將使用者傳進的命令轉換成字串陣列
 str：使用者傳進的命令
 cmd：回傳執行檔
 */
void parseString(char* str, char** cmd) {
    int idx=0;
    char* retPtr;
    //printf("%s\n", str);
    //這裡使用了strrtok，這個函數可以將字串切割成小字串
    //第二個參數決定「切割的依據」，在此為換行符號「\n」與空白符號「 」
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        //printf("token =%s\n", retPtr);
        //if(strlen(retPtr)==0) continue;
        argVect[idx++] = retPtr;
        if (idx==1) {
            *cmd = retPtr;
            //printf("para = %s\n", *cmd);
        }
        retPtr=strtok(NULL, " \n");
    }
    //最後一個字串指標設定為NULL，這樣才知道「已經到字串陣列的結尾」
    argVect[idx]=NULL;
}

int main(int argc, char **argv) {
     char cmdLine[4096];
     char* exeName;
     fgets(cmdLine, 4096, stdin);
     parseString(cmdLine,&exeName);
    for(int i=0;i<3;i++){
        printf("%s\n",argVect[i]);
    }

}

