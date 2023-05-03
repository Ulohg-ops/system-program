#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/file.h>

int main() {
     char userDir[100];
     uid_t ruid, euid, suid;
    getresuid(&ruid, &euid, &suid);
    strcat(userDir,"/home/");
    strcat(userDir,getpwuid(ruid)->pw_name);
    strcat(userDir,"/sys.log");
    // printf("%s\n",userDir);
 
    while(1){
        FILE* input= fopen("/proc/meminfo", "r");
        FILE* output= fopen(userDir, "a+");
        int fd = open(userDir, O_RDWR, S_IRUSR| S_IWUSR);
        time_t curtime;
        time(&curtime);    
        char memTotal[100];
        char memFree[100];
        fgets(memTotal, 100,input);
        fgets(memFree, 100,input);
        float memTotalGiga=0;
        float freeMemTotalGiga=0;
        char appendString[100];
        flock(fd,LOCK_EX);

       int word=0;
       int j=0;
        char info[12][100];
        for(int i = 0; i < strlen(memTotal); i++) {
            if (memTotal[i] == ' ') {
                info[word][j] = '\0';
                word++;
                j = 0;
            } else {
                info[word][j] = memTotal[i];
                j++;
            }
        }//7 is the number located
        for(int i=0;i<strlen(info[7]);i++){
            memTotalGiga*=10;
            memTotalGiga+=info[7][i]-48;
        }//char to int 

        float res=memTotalGiga;
        res/=1024;//mb
        res/=1024;//gb
        //  printf("%.2f\n",res);
//-------------------------------------------------
        word=0;
        j=0;
         char info2[12][100];
        for(int i = 0; i < strlen(memFree); i++) {
            if (memFree[i] == ' ') {
                info2[word][j] = '\0';
                word++;
                j = 0;
            } else {
                info2[word][j] = memFree[i];
                j++;
            }
        }//8 is the number located
        
        for(int i=0;i<strlen(info2[8]);i++){
            freeMemTotalGiga*=10;
            freeMemTotalGiga+=info2[8][i]-48;
        }//char to int 

        float res2=freeMemTotalGiga;
        res2/=1024;//mb
        res2/=1024;//gb
        strcat(appendString,ctime(&curtime));
        appendString[(int)strlen(ctime(&curtime))-1]='\0';
        strcat(appendString,", memTotal ");

        fprintf(output, "%s", appendString);
        fprintf(output, "%.2f", res);
        fprintf(output, "%s", "GB, MemFree ");
        fprintf(output, "%.2f", res2);
        fprintf(output, "%s\n", "GB");
        flock(fd,LOCK_UN);
         sleep(1);
        fclose(input);
        fclose(output);
    }  
        
        
}
