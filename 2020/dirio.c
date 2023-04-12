#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char const *argv[])
{
    FILE *output=fopen("./dirio.tmp","w+");
    int totalWriteSize=atoi(argv[1]);
    int bufSize=atoi(argv[2]);
    char *buf=(char*)malloc(sizeof(bufSize));
    setvbuf(output,buf,_IOFBF,bufSize);
    int j=1;
    for(int i=0;i<totalWriteSize;i++){
        if(j%2==1){
            char* c="1";
            fwrite(c,1,1,output);
        }else{
            char* c=" ";
            fwrite(c,1,1,output);
        }
        j++;
    }
    return 0;
}
