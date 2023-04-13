#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    uid_t ruid, euid, suid;
    // setuid(0);
    getresuid(&ruid, &euid, &suid);
    // printf("ruid = %d, euid = %d, suid = %d\n", ruid,euid, suid);
    // printf("ruid = %s, ", getpwuid(ruid)->pw_name);
    // printf("euid = %s, ", getpwuid(euid)->pw_name);
    // printf("suid = %s\n", getpwuid(suid)->pw_name);
    // system("chown root test.txt");
    // if(!strcmp(getpwuid(ruid)->pw_name,"root")){
        char account[20]="";
        char password[20]="";
        char repassword[20]="";

        FILE* file= fopen("./db-userinfo.txt", "a+");
        printf("%s","輸入帳號：");
        scanf("%s",account);
        // printf("%s","輸入密碼：");
        // scanf("%s",password);
        // printf("%s","再輸入密碼一次："); 
        // scanf("%s",repassword);
        // while(strcmp(password,repassword)!=0){
        //     printf("%s","密碼前後不一樣"); 
        //     printf("%s","再輸入密碼一次：");
        //     password==""; 
        //     scanf("%s",repassword);
        // }
        char buf[1000]="";
        fread(buf,1000,1,file);
        // strcat(account,":");
        // strcat(account,password);
        // strcat(account,"\n");
        // fprintf(file,account,account);
        char *loc=strstr(buf,account);
       if(loc!=NULL){
        printf("%s",loc);
       }


        // printf("%s",account);
        // printf("%s",password);

        
     
}