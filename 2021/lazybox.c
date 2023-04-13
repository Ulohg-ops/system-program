#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main() {
    while(1) {
        char command[4096];
        printf("lazybox$");
        fgets(command, 4096, stdin);
        char p[2][1000];
        if (strcmp("exit\n",command)==0){
            printf("%s\n","good bye");
            exit(0);
        }else if (command[0] == 'c' && command[1] == 'd') {
            int word = 0;
            int j = 0;
            for (int i = 0; i < strlen(command); i++) {
                if (command[i] == ' ') {
                    p[word][j] = '\0';
                    word++;
                    j = 0;
                } else {
                    p[word][j] = command[i];
                    j++;
                }
            }
            p[word][j]='\0';
            printf("%s",p[1]);
            printf("%d",strcmp("/",p[1]));//比較string是否相等 相等回傳0
            
            chdir(p[1]); 
            //獲取一個字串 之後切割放在陣列中 然後把陣列第2格的字丟到fun當中 我卡在字串處理
            //ex:輸入cd / 丟"/"  到chdir() 中
            //正常來說chdir("/") 可以成功執行 但是我切割之後就不行  
            //我不知道哪裡有問題 chatgpt也不會 ＝ ＝
            //how to try
            //就一樣是 "/"

        }else if(command[0] == 'l' && command[1] == 'n'){
            symlink("~/Desktop/system-program", "~/Desktop/system-program/2021/ewq.out");
        }
        else if(strcmp(command,"sakai\n")==0){
            printf("%s\n","sadkodsa");
        }else {
        system(command);
        }
    }
}

