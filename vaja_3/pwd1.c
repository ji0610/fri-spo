#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

int main(void) {

    struct stat buf_child, buf_parent, st;
    DIR *dp;
    struct dirent *dirp;

    char path[1024] = "";
    char tmp[1024];

    if (stat(".", &buf_child) == -1) {
        perror("stat .");
        return 1;
    }

    if (chdir("..") == -1) {
        perror("chdir ..");
        return 1;
    }

    if (stat(".", &buf_parent) == -1) {
        perror("stat ..");
        return 1;
    }

    while (!(buf_child.st_ino == buf_parent.st_ino && buf_child.st_dev == buf_parent.st_dev)) {

        dp = opendir(".");
        if (dp == NULL) {
            perror("opendir");
            return 1;
        }

        while ((dirp = readdir(dp)) != NULL) {

            if (lstat(dirp->d_name, &st) == -1)
                continue;

            if (st.st_ino == buf_child.st_ino && st.st_dev == buf_child.st_dev) {
                if (snprintf(tmp, sizeof(tmp), "/%s%s", dirp->d_name, path) >= (int)sizeof(tmp)) {
                    fprintf(stderr, "path too long\n");
                    closedir(dp);
                    return 1;
                }
                strcpy(path, tmp);
                break;
            }
        }

        if (closedir(dp) == -1) {
            perror("closedir");
            return 1;
        }

        buf_child = buf_parent;

        if (chdir("..") == -1) {
            perror("chdir ..");
            return 1;
        }

        if (stat(".", &buf_parent) == -1) {
            perror("stat ..");
            return 1;
        }

    }

    if (printf("%s\n", path[0] ? path : "/") < 0) {
        perror("printf");
        return 1;
    }

    return 0;
}
