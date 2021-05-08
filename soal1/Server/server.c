#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <asm-generic/socket.h>
#define PORT 8080

int server_fd, new_socket, new_socket2, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[1024] = {0};
FILE *listakun;
pthread_t tid[2];
pthread_mutex_t lock;	
struct Akun Acc;

struct Akun
{
    char username[12];
    char password[50];
};

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char *hello = "Server Connected";
    send(new_socket , hello , strlen(hello) , 0 );
    memset(buffer, 0, 1024);
    valread = read(new_socket, buffer, 1024);
    if(strcmp(buffer, "register") == 0 || strcmp(buffer, "Register") == 0 || strcmp(buffer, "REGISTER") == 0)
    {
        //read from client
        memset(buffer, 0, 1024);
        valread = read(new_socket, buffer, 1024);
        strcpy(Acc.username, buffer);
        memset(buffer, 0, 1024);
        valread = read(new_socket, buffer, 1024);
        strcpy(Acc.password, buffer);
        //append data
        listakun  = fopen ("akun.txt", "a");
        fprintf(listakun,"%s:%s\n",Acc.username,Acc.password);
        fclose(listakun);
        printf("Register Success\n");
        //read data
        listakun = fopen("akun.txt","r");
        while(fscanf(listakun,"%s:%s",Acc.username,Acc.password)>0)
        {
            printf("%s:%s\n",Acc.username,Acc.password);
        }
        fclose(listakun);
    }
    else if(strcmp(buffer, "login") == 0 || strcmp(buffer, "Login") == 0 || strcmp(buffer, "LOGIN") == 0)
    {
        //read from client
        char user[12];
        char pass[50];
        memset(buffer, 0, 1024);
        valread = read(new_socket, buffer, 1024);
        strcpy(user, buffer);
        memset(buffer, 0, 1024);
        valread = read(new_socket, buffer, 1024);
        strcpy(pass, buffer);
        memset(buffer, 0, 1024);
        //read data
        listakun = fopen("akun.txt","r");
        while(fscanf(listakun,"%s","%s",Acc.username,Acc.password)>0)
        {
            if(strcmp(user, Acc.username) == 0 && strcmp(pass, Acc.password) == 0)
            {
                fclose(listakun);
                printf("\nAuth Success\n");
                char *success = "Login Success";
                send(new_socket, success, strlen(success), 0);
            }
        }
        fclose(listakun);
        printf("\nAuth Failed\n");
        char *failed = "Login Failed";
        send(new_socket , failed , strlen(failed) , 0 );
    }
    
}
