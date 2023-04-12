#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main() {
        char command[4096];
        // printf("lazybox$");
        fgets(command, 4096, stdin);
        char p[2][1000];
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
        chdir(p[1]); // 使用第二個子字串作為要切換的目錄名稱
        return 0;
}
