

#include <stdio.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


long long MinFileSize, MaxFileSize;

void GoThroughFiles(char *dirName, int *cntOfFiles, FILE *file) {
    DIR *currDir;
    struct dirent *dp;
    struct stat buf;

    char *dirDown;
    int cntFilesInDir = 0;

  
    if ((currDir = opendir(dirName)) == NULL) {
        perror("Couldn't open the directory");
        return;
    }

    
    while ((dp = readdir(currDir)) != NULL) {
        
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            dirDown = (char*) calloc(strlen(dirName)+1+ strlen(dp->d_name), sizeof(char));
            strcpy(dirDown, dirName);
            strcat(dirDown, "/");
            strcat(dirDown, dp->d_name);
            GoThroughFiles(dirDown, cntOfFiles, file);
        }
      
        else if (dp->d_type == DT_REG) {
            
            char *filePath = (char*) calloc(strlen(dirName) + 1 + strlen(dp->d_name), sizeof(char));
            strcpy(filePath, dirName);
            strcat(filePath, "/");
            strcat(filePath, dp->d_name);

            cntFilesInDir++;

          
            if (stat(filePath, &buf) == 0) {
                if ((buf.st_size>=MinFileSize) && (buf.st_size<=MaxFileSize )){
                    
                    fprintf(file, "%s       %s         %ld\n", filePath,dp->d_name,buf.st_size); 
                }
            }
        }
    }

    
    *cntOfFiles += cntFilesInDir;

    if (closedir(currDir) == -1) {
        perror("Couldn't close directory");
    }

    return;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr ,"First argument - name of directory\nSecond argument - min size of file\nThird argument - max size of file\nFourth argument - name of the result file\n");
        return -1;
    }

   
    int fileHandler = open(argv[4], O_WRONLY | O_CREAT, 438);
    if (fileHandler < 0) {
        perror("Couldn't open the out file");
        return -1;
    }

    
    errno = 0;
    FILE *file = fdopen(fileHandler, "w");
    if (errno != 0) {
        perror("Couldn't open the out file");
        return -1;
    }

    int cnt = 0;
    char *ptr;
    MinFileSize = strtol(argv[2], &ptr, 10);
    MaxFileSize = strtol(argv[3], &ptr, 10);

    GoThroughFiles(argv[1], &cnt, file);
    printf("Amount of files: %d\n", cnt);
    return 0;
}