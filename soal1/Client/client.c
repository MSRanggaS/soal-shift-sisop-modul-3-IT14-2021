#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0;
    struct sockaddr_in serv_addr;

    // msg for server
    char input[1024];

    // msg from server
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
  // jika gagal menyambungkan ke server manapun maka tampil "Connection Failed"
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }


    // code here

    // login/register
    do {
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("login/register: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // id
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("id: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // pwd
        memset(buffer, 0, sizeof(buffer));
        read(sock, buffer, sizeof(buffer));
        printf("password: ");
        scanf("%s", input);
        send(sock, input, strlen(input), 0);
        memset(input, 0, sizeof(input));

        // success or fail
        read(sock, buffer, 1024);
        printf("%s\n", buffer);
    } while (strcmp(buffer, "login success") != 0);

    printf("\n");

    // logged in
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        printf("command: ");
        scanf("\n");
        scanf("%[^\n]%*c", input);
        if (strncmp(input, "add", 3) == 0) {
            send(sock, input, sizeof(input), 0);
            
            // publisher
            memset(input, 0, sizeof(input));
            printf("Publisher: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // year
            memset(input, 0, sizeof(input));
            printf("Tahun publikasi: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // filepath
            memset(input, 0, sizeof(input));
            printf("Filepath: ");
            scanf("%s", input);
            send(sock, input, sizeof(input), 0);

            // send file
            char file_length[1024];
            char *file_content = (char *)malloc(sizeof(char) * 65536);

            memset(file_length, 0, sizeof(file_length));

            FILE *fptr;
            fptr = fopen(input, "r");

            fseek(fptr, 0, SEEK_END);
            long fsize = ftell(fptr);
            rewind(fptr);

            fread(file_content, 1, fsize, fptr);

            fclose(fptr);

            // send file size
            sprintf(file_length, "%ld", fsize);
            send(sock, file_length, sizeof(file_length), 0);
            sleep(1);

            // send file content
            for (long i = 0; i < fsize; i += 1024) {
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                send(sock, buffer, sizeof(buffer), 0);
            }
        } else if (strncmp(input, "download", 8) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);

            if (strcmp(buffer, "found") == 0) {
                // receive file
                char file_content[65535];
                char file_length[1024];
                long fsize;
                char file_to_save[50];
                sprintf(file_to_save, "%.*s", strlen(input) - 9, input + 9);

                memset(file_content, 0, sizeof(file_content));
                memset(file_length, 0, sizeof(file_length));

                // receive file size
                read(sock, file_length, 1024);
                fsize = strtol(file_length, NULL, 0);

                // receive file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    read(sock, buffer, 1024);
                    strcat(file_content, buffer);
                }

                FILE *fptr;
                fptr = fopen(file_to_save, "w");

                fprintf(fptr, "%s", file_content);

                fclose(fptr);
            } else {
                printf("file not found\n");
            }
        } else if (strncmp(input, "delete", 6) == 0) {
            send(sock, input, sizeof(input), 0);
        } else if (strncmp(input, "see", 3) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
        } else if (strncmp(input, "find", 4) == 0) {
            send(sock, input, sizeof(input), 0);
            read(sock, buffer, 1024);
            printf("%s\n", buffer);
        } else {
            send(sock, input, sizeof(input), 0);
            break;
        }

        memset(input, 0, sizeof(input));
        printf("\n");
    }

    return 0;
}
