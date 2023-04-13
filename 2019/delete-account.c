#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100 // 最大帐户数

typedef struct {
    char username[50];
    char password[50];
} Account;

int main() {
    char username[50];
    char filename[] = "accounts.txt";
    Account accounts[MAX_ACCOUNTS];
    int num_accounts = 0;
    int i, j;

    // 读取accounts.txt文件中的所有帐户
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return 1;
    }
    while (fscanf(fp, "%49[^:]:%49[^\n]\n", accounts[num_accounts].username, accounts[num_accounts].password) == 2) {
        num_accounts++;
    }
    fclose(fp);

    // 获取要删除的帐户名
    printf("Enter the username of the account to delete: ");
    scanf("%49s", username);

    // 在内存中删除指定帐户
    for (i = 0; i < num_accounts; i++) {
        if (strcmp(accounts[i].username, username) == 0) {
            // 找到要删除的帐户，将后面的帐户依次往前移动
            for (j = i + 1; j < num_accounts; j++) {
                accounts[j - 1] = accounts[j];
            }
            num_accounts--; // 帐户数减一
            break;
        }
    }
    if (i == num_accounts) {
        // 没有找到要删除的帐户
        printf("Account not found.\n");
        return 1;
    }

    // 将更新后的帐户写回accounts.txt文件中
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Failed to open file %s\n", filename);
        return 1;
    }
    for (i = 0; i < num_accounts; i++) {
        fprintf(fp, "%s:%s\n", accounts[i].username, accounts[i].password);
    }
    fclose(fp);

    printf("Account deleted successfully.\n");

    return 0;
}
