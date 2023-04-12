/**
DT_BLK      This is a block device. 「b」
DT_CHR      This is a character device. 「c」
DT_DIR      This is a directory. 「d」
DT_FIFO     This is a named pipe (FIFO). 「f」
DT_LNK      This is a symbolic link. 「l」
DT_REG      This is a regular file. 「-」
DT_SOCK     This is a UNIX domain socket. 「s」
DT_UNKNOWN  The file type could not be determined. 「U」
*/


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

//檔案型別應該不會超過100個
int filetype[100];
char fileSymbol[100];

//準備統計目錄中到底有多少種檔案型別，如果有該型別在filetype[X]上設定為1，該型別的代表字放在fileSymbol
void initFileTypoe() {
    for (int i=0; i< 99; i++) {
        filetype[i] = -1;
    }
    /*同學自己補下去*/
    fileSymbol[DT_BLK]='b';
    fileSymbol[DT_CHR]='c';//2
    fileSymbol[DT_DIR]='d';
    fileSymbol[DT_FIFO]='f';//1
    fileSymbol[DT_LNK]='l';
    fileSymbol[DT_REG]='-';
    fileSymbol[DT_SOCK]='s';
    fileSymbol[DT_UNKNOWN]='U';

}

//回傳某個檔案的大小
int readSize(char* pathname) {
    struct stat buf;
    // printf("readSize:%s\n", pathname);
    assert(stat(pathname, &buf)==0);
    return buf.st_size;

}
long myCountDir(char* path) {
    setuid(0);
    long size = 0; 
    //打開該目錄
    DIR* dirp = opendir(path);
    struct dirent* ent = readdir(dirp);
    while (ent != NULL) {
        if (strcmp(ent->d_name, "." )==0 || strcmp(ent->d_name, ".." )==0) {
            ent = readdir(dirp);
            continue;
        }
        //設定有這種檔案型別
        filetype[ent->d_type] = 1;
        //製造『路徑/名』
        //如果使用者的輸入是「/」怎麼辦？，例如：「//home」會發生錯誤嗎？
        char pathname[PATH_MAX]="";
        strcat(pathname, path);
        strcat(pathname, "/");
        strcat(pathname, ent->d_name);
        printf("%s\n", pathname);
        //如果是目錄
       if (ent->d_type == DT_DIR) {
             myCountDir(pathname);  
        }
        ent = readdir(dirp);
    }
    closedir(dirp);
    return size;
}

int main(int argc, char** argv) {
    int totalFileSize=0;
    initFileTypoe();
    totalFileSize=myCountDir(argv[1]);
    // char fileTypeRes[100]="";
    // printf("%s ","檔案種類：");
    // for(int i=0;i<99;i++){
    //     if(filetype[i]==1){
    //         printf("%c",fileSymbol[i]);
    //     }
    // }	
    printf("\n");
    
}
