#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main() {
    char userDir[100];
     uid_t ruid, euid, suid;
    getresuid(&ruid, &euid, &suid);
    strcat(userDir,"/home/");
    strcat(userDir,getpwuid(ruid)->pw_name);
    strcat(userDir,"/sys.log");
    // printf("%s",userDir);
    while(1){
        FILE* input= fopen("/proc/loadavg", "r");
        FILE* output= fopen(userDir, "a+");
        char buf[100];
        char info[5][100];
        char appendString[100];
        int word = 0;
        int j = 0;
        time_t curtime;
        time(&curtime);    
        fgets(buf, 100,input);
        for (int i = 0; i < strlen(buf); i++) {
            if (buf[i] == ' ') {
                info[word][j] = '\0';
                word++;
                j = 0;
            } else {
                info[word][j] = buf[i];
                j++;
            }
        }
        strcat(appendString,ctime(&curtime));
        appendString[(int)strlen(ctime(&curtime))-1]='\0';
        strcat(appendString,", avgrunnable ");
        for(int i=0;i<3;i++){
            strcat(appendString,info[i]);    
            strcat(appendString," ");        
        }       
        strcat(appendString,", total_tasks ");        

        char total_tasks[100];        
        int flag=0;
        int r=0;
        for (int i = 0; i < strlen(info[3]); i++) {
            if(flag==1){
                total_tasks[r]=info[3][i];
                r++;
            }
            if(info[3][i]=='/'){
                flag=1;
            }
        }
        total_tasks[r]='\0';
        strcat(appendString,total_tasks);        
        strcat(appendString,"\n");        
        // printf("%s\n",appendString);
        fprintf(output, "%s", appendString);
        fclose(input);
        fclose(output);
        sleep(1);
    }    
}

