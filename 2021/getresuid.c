#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv) {
    uid_t ruid, euid, suid;
    // setuid(0);
    getresuid(&ruid, &euid, &suid);
    printf("ruid = %d, euid = %d, suid = %d\n", ruid,euid, suid);
    printf("ruid = %s, ", getpwuid(ruid)->pw_name);
    printf("euid = %s, ", getpwuid(euid)->pw_name);
    printf("suid = %s\n", getpwuid(suid)->pw_name);
    // system("chown root test.txt");
    chown("./test.txt",euid,euid);
}