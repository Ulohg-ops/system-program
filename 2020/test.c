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


int main(int argc, char const *argv[])
{
    char* s="./";
    char pathname[512];
    strcpy(pathname,s);
    printf("%s\n",pathname);
    /* code */

    char* b="";    
    strcpy(pathname,b);
    printf("%s\n",pathname);
    
    
    
    
    return 0;
}
