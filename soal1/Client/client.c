#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char choice[8];
    char username[12];
    char password[50];
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    printf("\n----------WELCOME----------");
    printf("\n1. Register");
    printf("\n2. Login");
    printf("\nChoose : ");
    scanf("%s", choice);
    if(strcmp(choice, "register") == 0 || strcmp(choice, "Register") == 0 || strcmp(choice, "REGISTER") == 0)
    {
        send(sock, choice, strlen(choice), 0);
        printf("\nRegister");
        printf("\nUsername : ");
        scanf("%s", username);
        send(sock, username, strlen(username), 0);
        printf("Password : ");
        scanf("%s", password);
        send(sock, password, strlen(password), 0);
        printf("Register Success\n");
    }
    else if(strcmp(choice, "login") == 0 || strcmp(choice, "Login") == 0 || strcmp(choice, "LOGIN") == 0 )
    {
        send(sock, choice, strlen(choice), 0);
        printf("\nLogin");
        printf("\nUsername : ");
        scanf("%s", username);
        send(sock, username, strlen(username), 0);
        printf("Password : ");
        scanf("%s", password);
        send(sock, password, strlen(password), 0);
        memset(buffer, 0, 1024);
        valread = read(sock, buffer, 1024);
        printf("%s\n", buffer);
        if(strcmp(buffer, "Login Success") == 0)
        {
            printf("lanjutt\n");
        }
        else if(strcmp(buffer, "Login Failed") == 0)
        {
            printf("gagall\n");
        }
        else
        {
            printf("Error\n");
            return 0;
        }
    }
    else if(strcmp(choice, "exit") == 0 || strcmp(choice, "Exit") == 0 || strcmp(choice, "EXIT") == 0)
    {
        send(sock, choice, strlen(choice), 0);
        return 0;
    }
    else
    {
        printf("Your Choice Not Found\n");
    }
    // send(sock , hello , strlen(hello) , 0 );
    // printf("Hello message sent\n");
    // valread = read( sock , buffer, 1024);
    // printf("%s\n",buffer );
    // return 0;
    
}
