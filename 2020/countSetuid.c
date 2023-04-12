#define _DEFAULT_SOURCE
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <stddef.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <assert.h>
//底下這個.h定義了最長的path name長度
#include <linux/limits.h>
void printDir(char* pathName){
    DIR* dirp=opendir(pathName);
    struct dirent* ent=readdir(dirp);
    
    struct stat buf;
    char pathname[512];
    char *curDir="./";
    int perm;
    
    while(ent!=NULL){
        // printf("%s %s ","before:",pathname);
        strcpy(pathname,curDir);
        strcat(pathname,ent->d_name);
        // printf("%s %s ","after:",pathname);
        printf("%s\n",pathname);
        stat(pathname,&buf);
        perm=(buf.st_mode&S_ISUID);
        printf("%o\n",perm);
        if(strcmp(ent->d_name, ".")==0 || strcmp(ent->d_name,"..")==0){
            ent=readdir(dirp);
            continue;
        }
        
        // printf("%s\n",ent->d_name);
        if(ent->d_type==DT_DIR){
            char * newPathName=malloc(sizeof(PATH_MAX)); 
            sprintf(newPathName, "%s/%s", pathName,ent->d_name);
            printDir(newPathName);
        }
        ent=readdir(dirp);
    }
    closedir(dirp);
}

int main(int argc, char **argv)
{  
    printDir(argv[1]);
    /* code */
    return 0;
}

