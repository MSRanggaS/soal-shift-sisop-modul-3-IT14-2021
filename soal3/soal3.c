#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>

int file_count = 0;

struct args {
  char *buffer;
};

char *getFileName(char *fName, char buff[]) {
  char *token = strtok(fName, "/");
  while (token != NULL) {
    sprintf(buff, "%s", token);
    token = strtok(NULL, "/");
  }
}

void listdir(const char *name, char buff[][1337], int iter)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(name);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, name);
            strcat(path, "/");
            strcat(path, dp->d_name);
            DIR * temp = opendir(path);
            if(!temp){
                sprintf(buff[iter], "%s/%s", name, dp->d_name);
                iter++;
            }
            listdir(path, buff, iter);
        }  
    }

    closedir(dir);
}

void count(const char *name)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(name);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {

            // Construct new path from our base path
            strcpy(path, name);
            strcat(path, "/");
            strcat(path, dp->d_name);
            count(path);
            file_count++;
        }
    }

    closedir(dir);
}

char *getExtension(char *fName, char buff[]) {
  char buffFileName[1337];
  char *token = strtok(fName, "/");
  while (token != NULL) {
    sprintf(buffFileName, "%s", token);
    token = strtok(NULL, "/");
  }
  int count = 0;
  token = strtok(buffFileName, ".");
  while(token != NULL) {
    count++;
    sprintf(buff, "%s", token);
    token = strtok(NULL, ".");
  }
  if (count <= 1) {
    strcpy(buff, "unknown");
  }

  return buff;
}

void dirChecking(char buff[]) {
  DIR *dr = opendir(buff);
  if (ENOENT == errno) {
    mkdir(buff, 0775);
    closedir(dr);
  }
}

void *move(void* arg) {
  char buffExt[100];
  char buffFileName[1337];
  char buffFrom[1337];
  char buffTo[2774];
  char cwd[1337];
  getcwd(cwd, sizeof(cwd));
  strcpy(buffFrom, (char *) arg);

  if (access(buffFrom, F_OK) == -1) {
    // printf("File %s tidak ada\n", buffFrom);
    pthread_exit(0);
  }
  DIR* dir = opendir(buffFrom);
  if (dir) {
    // printf("file %s berupa folder\n", buffFrom);
    pthread_exit(0);
  }
  closedir(dir);

  getFileName(buffFrom, buffFileName);
  strcpy(buffFrom, (char *) arg);

  getExtension(buffFrom, buffExt);
  for (int i = 0; i < sizeof(buffExt); i++) {
    buffExt[i] = tolower(buffExt[i]);
  }
  strcpy(buffFrom, (char *) arg);

  dirChecking(buffExt);

  sprintf(buffTo, "%s/%s/%s", cwd, buffExt, buffFileName);
  if (rename(buffFrom, buffTo)<0){
      printf("%s %s\n",buffFrom,buffTo);
  }

  pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("Butuh argumen tambahan -f / -d / *\n");
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1], "-f") != 0 && strcmp(argv[1], "*") != 0 && strcmp(argv[1], "-d")) {
        printf("Argumen kurang benar\n");
        exit(EXIT_FAILURE);
    }

    if (strcmp(argv[1], "-f") == 0) {
        if (argc <= 2) {
        printf("Argumen kurang benar\n");
        exit(EXIT_FAILURE);
        }

        pthread_t tid[argc-2];
        for (int i = 2; i < argc; i++) {
        pthread_create(&tid[i-2], NULL, &move, (void *)argv[i]);
        }
        for (int i = 2; i < argc; i++) {
        pthread_join(tid[i-2], NULL);
        }
        exit(0);
    }

    char *directory;
    if (strcmp(argv[1], "*") == 0) {
        if (argc != 2) {
        printf("Argumen kurang benar\n");
        exit(EXIT_FAILURE);
        }
        char buff[1337];
        getcwd(buff, sizeof(buff));
        directory = buff;
    }

    if (strcmp(argv[1], "-d") == 0) {
        if (argc != 3) {
        printf("Argumen kurang benar\n");
        exit(EXIT_FAILURE);
        }
        DIR* dir = opendir(argv[2]);
        if (dir) {
            directory = argv[2];
        } else {
            printf("Directory tidak ada\n");
            exit(EXIT_FAILURE);
        }
        closedir(dir);
    }

    count(directory);

    pthread_t tid[file_count];
    char buff[file_count][1337];
    int iter = 0;

    listdir(directory, buff, iter);

    for (int i = 0; i < file_count; i++) {
        char  *test = (char*)buff[i];
        // printf("%s\n", test);
        pthread_create(&tid[i], NULL, &move, (void *)test);
    }

    for (int i = 0; i < file_count; i++) {
        pthread_join(tid[i], NULL);
    }

}
