#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <pwd.h>

int main() {
        char *time;
        DIR* dir;
        struct dirent* ent;
        char* curDir = "./";   
        char cwd[1000];
        struct stat buf;
        getcwd(cwd, sizeof(cwd));
        strcat(cwd,"/lazybox");
        printf("Current working dir: %s\n", cwd);
        stat(cwd, &buf);
        time=ctime(&buf.st_atime);
        time[strlen(time)-1] = 0;
        printf("%s\n",getpwuid(buf.st_uid)->pw_name);
        printf("%s\n",time);
        return 0;
}
