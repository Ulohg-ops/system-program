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

char* cmdParaArr[10][256];//紀錄每個指令有幾個參數
char* argVect[256];
char *cmdArr[256];//使用者輸入的指令
int cmdParaNumber[10];//指令參數數量
int cmdNumber=0;
int cmdIdx=0;

void parseString(char* str, char** cmd) {
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        argVect[idx++] = retPtr;
        if (idx==1)
            *cmd = retPtr;
        retPtr=strtok(NULL, " \n");
    }
    argVect[idx]=NULL;
}
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
void parseStringAndSplit(char* str) {
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, " \n");
    while(retPtr != NULL) {
        cmdParaArr[cmdIdx][idx++] = retPtr;
        retPtr=strtok(NULL, " \n");
        cmdParaNumber[cmdIdx]++;
    }
    cmdParaArr[cmdIdx++][idx]=NULL;

}
void parseStringByPipe(char* str) {
    cmdNumber=0;
    int idx=0;
    char* retPtr;
    retPtr=strtok(str, "|\n");
    while(retPtr != NULL) {
        cmdArr[idx++] = retPtr;
        retPtr=strtok(NULL, "|\n");
        cmdNumber++;
    }
    cmdArr[idx]=NULL;
}
int main (int argc, char** argv) {
    char cmdLine[4096];
    fgets(cmdLine, 4096, stdin);
    parseStringByPipe(cmdLine);
    for(int i=0;i<cmdNumber;i++){
        parseStringAndSplit(trim(cmdArr[i]));
    }
    for(int i=0;i<3;i++){
        printf("%d\n",cmdParaNumber[i]);
    }
    for(int i=0;i<cmdNumber;i++){
        for(int j=0;j<cmdParaNumber[i];j++){
            printf("%s\n",cmdParaArr[i][j]);
        }
            printf("-----------\n");
    }
}