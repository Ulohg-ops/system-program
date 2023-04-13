#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 100 //定义一个常量，表示最大文本行长度

int main() {
    char filename[] = "db-userinfo.txt"; //指定存储帐号密码的文本文件
    char line[MAX_LENGTH]; //用来存储从文本文件中读取的一行文本
    char username[MAX_LENGTH]; //存储输入的用户名
    char password[MAX_LENGTH]; //存储输入的密码
    char *found_password; //用来存储找到的密码，如果没有找到则为NULL
    char *token; //指向用户名和密码的分隔符“:”

    //提示用户输入用户名
    printf("请输入用户名：");
    fgets(username, MAX_LENGTH, stdin);
    username[strcspn(username, "\n")] = 0; //去掉输入字符串末尾的换行符

    //提示用户输入密码
    printf("请输入密码：");
    fgets(password, MAX_LENGTH, stdin);
    password[strcspn(password, "\n")] = 0; //去掉输入字符串末尾的换行符

    //打开文本文件
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("无法打开文件 %s\n", filename);
        exit(EXIT_FAILURE);
    }

    //逐行读取文本文件中的内容
    while (fgets(line, MAX_LENGTH, fp) != NULL) {
        //在行末去掉换行符
        line[strcspn(line, "\n")] = 0;

        //查找用户名和密码的分隔符“:”
        token = strtok(line, ":");//返回被分解的第一个子字符串
        if (token == NULL) {
            continue;
        }
        //比较输入的用户名和当前行中的用户名
        if (strcmp(username, token) == 0) {
            //找到了匹配的用户名，获取密码并进行比较
            token = strtok(NULL, ":");
            if (token != NULL) {
                found_password = token;
                if (strcmp(password, found_password) == 0) {
                    //用户名和密码都匹配，输出成功信息并退出程序
                    printf("登录成功！\n");
                    fclose(fp);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }

    //未找到匹配的用户名和密码，输出失败信息并退出程序
    printf("登录失败！\n");
    fclose(fp);
    exit(EXIT_FAILURE);
}

//https://shengyu7697.github.io/cpp-strtok/ 