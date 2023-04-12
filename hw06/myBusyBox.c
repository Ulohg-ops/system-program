	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char cmd[100];

    if( strcmp(argv[0], "./cp")==0 ){
        sprintf(cmd, "cp");
        for(int i=1;i<argc;i++){
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
        system(cmd);
    }

    else if( strcmp(argv[0], "./ls")==0 ){
        sprintf(cmd, "ls");
        for(int i=1;i<argc;i++){
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
        system(cmd);
    }

    else if( strcmp(argv[0], "./cat")==0 ){
        sprintf(cmd, "cat");
        for(int i=1;i<argc;i++){
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
        system(cmd);
    }

    else if( strcmp(argv[0], "./chown")==0 ){
        sprintf(cmd, "chown");
        for(int i=1;i<argc;i++){
            strcat(cmd, " ");
            strcat(cmd, argv[i]);
        }
        system(cmd);
    }

    return 0;
}