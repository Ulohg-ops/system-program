#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>


//word_buf 用來存取單字 並設單字長度<=80
//若讀取遇到 0x20 代表一個單字結尾
//若遇到換行符號就換行
//若此行的總字數 + 新增加的單字 >80 
// 先換行之後再新增單字

int main(int argc, char **argv)
{
    FILE *input = fopen(argv[1], "r");
    FILE *output = fopen(argv[2], "w");

    int num = atoi(argv[3]);
    if (num == 0){ //unbuffered
        printf("unbuffered\n");
        assert(setvbuf(input, NULL, _IONBF, 0)==0);
        assert(setvbuf(output, NULL, _IONBF, 0)==0);
    }
    else if (num == -1){ //linebuffered
        printf("linebuffered\n");
        assert(setvbuf(input, NULL, _IOLBF, 0)==0);
        assert(setvbuf(output, NULL, _IOLBF, 0)==0);
    }else{ //fullybuffered
        printf("fullybuffered\n"); 
        int buff_size = num;
        assert(setvbuf(input, NULL, _IOFBF, buff_size) == 0);
        assert(setvbuf(output, NULL, _IOFBF, buff_size) == 0);
    }
    char word_buf[82];
    int buflen = 0;
    int linePos = 0; //already write in the line
    int total = 0;
    while (1)
    {
        int inputInt = getc(input);
        if (inputInt == EOF)
            break;
        unsigned char inputChar = (unsigned char)inputInt;
        if (inputChar == '\n') {
            fprintf(output, "\n");
            linePos=0;
        }
        // printf("inputChar:%c linePos:%d,buflen:%d\n",inputChar,linePos,buflen);
        if (inputChar == ' ')
        {
            if (linePos + buflen > 80)
            {
                fprintf(output, "\n");
                total += linePos;
                linePos = 0;
            }
            word_buf[buflen] = ' ';
            word_buf[buflen+1] = '\0';
            fprintf(output, "%s", word_buf);
            linePos += buflen+1;
            buflen = 0;
            continue;
        }
        word_buf[buflen++] = inputChar;
    }
    return 0;
}
