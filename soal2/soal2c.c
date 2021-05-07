#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
 
int main() {
    pid_t child_id;
    int status;
    int fp1[2];
    int fp2[2];
    char output[1000];
    
    if (pipe(fp1)==-1){
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    
    if (pipe(fp2)==-1){
        fprintf(stderr, "Pipe Failed" );
        return 1;
    }
    
    child_id = fork();
        if (child_id < 0) {
        exit(EXIT_FAILURE);
    }
    
    if (child_id == 0) {
        close(fp1[0]);
        dup2(fp1[1], STDOUT_FILENO);
        char *argv[] = {"ps","aux", NULL};
        execv("/bin/ps", argv);
    } else {
        // this is parent
        while ((wait(&status)) > 0);
        child_id = fork();
        if (child_id < 0) {
            exit(EXIT_FAILURE);
        }
        if (child_id == 0){
            close(fp1[1]);
            dup2(fp1[0], STDIN_FILENO);
            close(fp2[0]);
            dup2(fp2[1], STDOUT_FILENO);
            char *argv[] = {"sort", "-nrk", "3,3", NULL};
            execv("/usr/bin/sort", argv);
        }else{
            close(fp2[1]);
            close(fp1[1]);
            while ((wait(&status)) > 0);
            dup2(fp2[0], STDIN_FILENO);
            char *argv[] = {"head", "-5", NULL};
            execv("/usr/bin/head", argv);
        }
    }
}
