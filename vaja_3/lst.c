#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>


struct name_time {
    char name[256];
    time_t time;
};

int compare (const void *a, const void *b) {
    const struct name_time *aa = a;
    const struct name_time *bb = b;

    if (aa->time > bb->time) return -1;
    if (aa->time < bb->time) return 1;
    return strcmp(aa->name, bb->name);
}


void main(int argc, char *argv[]) {


    DIR* dp;
    struct stat buf;

    int reverse = 0;

    if (argc == 2 && strcmp(argv[1], "-r") == 0) {
        reverse = 1;
    } else if (argc != 1) {
        fprintf(stderr, "Uporaba: %s (-r)\n", argv[0]);
        exit(1);
    }

    
    if ((dp=opendir(".")) == NULL){
        perror(".");
        exit(1);
    }

    struct dirent *dirp;

    int num_of_files = 0;

    while((dirp = readdir(dp)) != NULL) {
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) {
            num_of_files++;  
        }            
    }

    struct name_time *list_of_files = malloc(num_of_files * sizeof(struct name_time));

    rewinddir(dp);

    int i = 0;

    while((dirp = readdir(dp)) != NULL) {
        
        if (strcmp(dirp->d_name, ".") != 0 && strcmp(dirp->d_name, "..") != 0) { 
            
            if (lstat(dirp->d_name, &buf) < 0) {
                printf("lstat error.\n");
                exit(1);
            }
            
            strcpy(list_of_files[i].name, dirp->d_name);
            list_of_files[i].time = buf.st_mtime;
            i++;
        }            
    }

    qsort(list_of_files, i, sizeof(struct name_time), compare);
    
    if (reverse) {
        for (int j = i-1; j >= 0; j--) {
        printf("%s ", list_of_files[j].name);
        }
    } else {
        for (int j = 0; j < i; j++) {
        printf("%s ", list_of_files[j].name);
        }
    }
    printf("\n");

    //printf("st: %d\n", num_of_files);

    free(list_of_files);
    closedir(dp);
}