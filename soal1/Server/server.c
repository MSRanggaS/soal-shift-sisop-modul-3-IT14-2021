#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <math.h>

//mendefinisikan port server
#define PORT 8080
#define SO_REUSEPORT 15

int n;

//membuat struct user yg terdiri dari nama user dan password
struct user {
    char name[50];
    char pwd[50];
};

//membuat struct file yang terdiri nama file dan path filenya
struct file {
    char name[50];
    char path[50];
    char publisher[50];
    char ext[20];
    char year[5];
};

char *file_ext(char *file) {
    char *p = strchr(file, '.');
    if (p == NULL) return "exe";
    if (p - file == 6) {
        p = strchr(p+1, '.');
    }

    char *ext;
    ext = (char *)malloc(sizeof(char*) * 50);
    sprintf(ext, "%.*s", strlen(file) - (p - file + 1), p + 1);
    for (size_t i = 0; i < strlen(ext); ++i) {
        *(ext + i) = tolower(*(ext + i));
    }
    return ext;
}

// untuk melihat file yang ada telah dibuat
char *see(struct file files[], int n) {
    char *buf;
    buf = (char *)malloc(sizeof(char) * 1024);
    memset(buf, 0, sizeof(buf));

    /*
        Nama: 
        Publisher:
        Tahun publishing:
        Ekstensi File : 
        Filepath : 
    */

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
        
        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
    }

    return buf;
}

// untuk mencari user yang ada
int find_user(char *name, char *pwd) {
    // printf("%s:%s %d:%d\n", name, pwd, strlen(name), strlen(pwd));
    int n = 0, mode = 0;

    struct user users[50];

    for (int i = 0; i < 50; ++i) {
        memset(users[i].name, 0, sizeof(users[i].name));
        memset(users[i].pwd, 0, sizeof(users[i].pwd));
    }

    FILE *fptr;
    //membuka dan membaca di file akun.txt
    fptr = fopen("akun.txt", "r");

    fseek(fptr, 0, SEEK_END);
    long fsize = ftell(fptr);
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == ':') {
            mode ^= 1;
            continue;
        }
        if (str[i] == '\n' || str[i] == ' ') {
            mode ^= 1;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(users[n].name, "%s%c", users[n].name, str[i]);
        } else {
            sprintf(users[n].pwd, "%s%c", users[n].pwd, str[i]);
        }
    }

    fclose(fptr);

    for (int i = 0; i < n; ++i) {
        // printf("%s:%s %d:%d\n", users[i].name, users[i].pwd, strlen(users[i].name), strlen(users[i].pwd));
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].pwd, pwd) == 0) {
            return 1;
        }
    }
    
    return 0;
}

//fungsi register
void reg(char *name, char *pwd) {
    FILE *fptr;

    //membuat file dengan nama akun.txt
    fptr = fopen("akun.txt", "a");

    // isinya id sama password yg terdaftar
    fprintf(fptr, "%s:%s\n", name, pwd);
    
    fclose(fptr);
}

//membuat struct files yang akan menjadi databasenya
struct file *parse_files() {
    n = 0;
    int mode = 0;

    struct file *files;
    files = (struct file *)malloc(sizeof(struct file) * 50);

    for (int i = 0; i < 5; ++i) {
        //data yang akan diisikan berupa nama file, path filenya, nama publisher dan tahun publishnya
        memset(files[i].ext, 0, sizeof(files[i].ext));
        memset(files[i].name, 0, sizeof(files[i].name));
        memset(files[i].path, 0, sizeof(files[i].path));
        memset(files[i].publisher, 0, sizeof(files[i].publisher));
        memset(files[i].year, 0, sizeof(files[i].year));
    }

    FILE *fptr;
    //membuka file.tsv untuk menambahkan data di atas
    fptr = fopen("files.tsv", "r");

    //untuk membaca file  mulai dari offset yang diinginkan
    fseek(fptr, 0, SEEK_END); // offset dipindah relatif terhadap offset terakhir file.
    long fsize = ftell(fptr); // mengembalikan nilai saat ini dari posisi identifier.
    rewind(fptr);

    char *str = (char *)malloc(sizeof(char) * (fsize + 1));
    //membaca kalimat dalam sebuah FILE yang sudah dibuka di file.tsv
    fread(str, 1, fsize, fptr);

    for (int i = 0; i <= fsize; ++i) {
        if (str[i] == '\t') {
            mode += 1;
            continue;
        }
        if (str[i] == '\n') {
            mode = 0;
            ++n;
            continue;
        }

        if (mode == 0) {
            sprintf(files[n].path, "%s%c", files[n].path, str[i]);
        } else if (mode == 1) {
            sprintf(files[n].publisher, "%s%c", files[n].publisher, str[i]);
        } else {
            sprintf(files[n].year, "%s%c", files[n].year, str[i]);
        }
    }

    for (int i = 0; i < n; ++i) {
        sprintf(files[i].ext, "%s", file_ext(files[i].path));
        char *p = strstr(files[i].path, files[i].ext);
        if (p == NULL) {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        } else {
            sprintf(files[i].name, "%.*s", strlen(files[i].path) - 6, files[i].path + 6);
        }
    }

    fclose(fptr);

    return files;
}

//fungsi delete untuk menghapus file yang sudah ditambahkan sebelumnya
void delete(char *file) {
    struct file *files = parse_files();
    struct file *new_files = (struct file *)malloc(sizeof(struct file) * 50);
    int j = 0;
    for (int i = 0; i < n; ++i) {
        if (strcmp(files[i].name, file) == 0) {
            continue;
        }

        sprintf(new_files[j].ext, "%s", files[i].ext);
        sprintf(new_files[j].name, "%s", files[i].name);
        sprintf(new_files[j].path, "%s", files[i].path);
        sprintf(new_files[j].publisher, "%s", files[i].publisher);
        sprintf(new_files[j].year, "%s", files[i].year);

        ++j;
    }

    if (j == n) return;

    //nama file sebelum dihapus
    char old_name[50];
    sprintf(old_name, "FILES/%s", file);

    //nama file setelah dihapus
    char new_name[50];
    sprintf(new_name, "FILES/old-%s", file);

    //jika gagal merename nama file yang akan dihapus maka tampilkan "error renaming (nama file sebelum dihapus) to (nama file sesudah dihapus)"
    if (rename(old_name, new_name) < 0) {
        printf("error renaming %s to %s\n", old_name, new_name);
    }

    FILE *fptr;
    
    //membuka database file.tsv kemudian menuliskan atau mengedit yang ada di sana
    fptr = fopen("files.tsv", "w");

    for (int i = 0; i < j; ++i) {
        fprintf(fptr, "%s\t%s\t%s\n", new_files[i].path, new_files[i].publisher, new_files[i].year);
        // printf("%s\t%s\t%s\n", new_files[i].path, new_files[i].publisher, new_files[i].year);
    }

    fclose(fptr);
}

int main(int argc, char const *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // msg for client
    char *login_or_register = "login/register: ";
    char *id = "id: ";
    char *pwd = "password: ";
    char *login_success = "login success";
    char *login_fail = "login failed";
    char *register_success = "register success";
    char *file_pattern_not_found = "file pattern doesn't match";

    // msg from client
    char buffer[1024] = {0};
    char user_id[1024];
    char user_pwd[1024];
      
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

    // 
    mkdir("FILES", 0755);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // code here
        while (1) {
            send(new_socket, login_or_register, strlen(login_or_register), 0);
            read(new_socket, buffer, 1024);
            printf("user %s\n", buffer);

            memset(user_id, 0, sizeof(user_id));
            memset(user_pwd, 0, sizeof(user_pwd));

            if (strcmp(buffer, "register") == 0) {
                // id
                send(new_socket, id, strlen(id), 0);
                read(new_socket, user_id, 1024);
                printf("id: %s\n", user_id);
                
                // pwd
                send(new_socket, pwd, strlen(pwd), 0);
                read(new_socket, user_pwd, 1024);
                printf("password: %s\n", user_pwd);

                reg(user_id, user_pwd);
                send(new_socket, register_success, strlen(register_success), 0);
            } else if (strcmp(buffer, "login") == 0) {
                // id
                send(new_socket, id, strlen(id), 0);
                read(new_socket, user_id, 1024);
                printf("id: %s\n", user_id);
                
                // pwd
                send(new_socket, pwd, strlen(pwd), 0);
                read(new_socket, user_pwd, 1024);
                printf("password: %s\n", user_pwd);

                if (find_user(user_id, user_pwd)) {
                    send(new_socket, login_success, strlen(login_success), 0);
                    printf("%s\n", login_success);
                    break;
                } else {
                    send(new_socket, login_fail, strlen(login_fail), 0);
                    printf("%s\n", login_fail);
                }
            }

            memset(buffer, 0, sizeof(buffer));
            sleep(2);
        }
        printf("\n");

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            read(new_socket, buffer, 1024);

            if (strncmp(buffer, "add", 3) == 0) {
                printf("user add\n");
                char publisher[1024];
                char year[1024];
                char path[1024];
                char filepath[1024];

                read(new_socket, publisher, 1024);
                printf("Publisher: %s\n", publisher);

                read(new_socket, year, 1024);
                printf("Tahun Publikasi: %s\n", year);

                read(new_socket, path, 1024);
                printf("Filepath: %s\n", path);


                sprintf(filepath, "FILES/%s", path);
                // printf("%s\n", filepath);

                // tsv
                FILE *fptr;

                fptr = fopen("files.tsv", "a");
                fprintf(fptr, "%s\t%s\t%s\n", filepath, publisher, year);
                fclose(fptr);

                // log
                fptr = fopen("running.log", "a");
                fprintf(fptr, "Tambah : %s (%s:%s)\n", filepath, user_id, user_pwd);
                fclose(fptr);

                // receive file
                char file_content[65535];
                char file_length[1024];
                unsigned long fsize;

                memset(file_content, 0, sizeof(file_content));
                memset(file_length, 0, sizeof(file_length));

                // receive file size
                read(new_socket, file_length, 1024);
                fsize = strtol(file_length, NULL, 0);

                // receive file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    read(new_socket, buffer, 1024);
                    strcat(file_content, buffer);
                }

                // printf("%s\n", file_content);

                fptr = fopen(filepath, "w");
                fprintf(fptr, "%s", file_content);
                fclose(fptr);
            } else if (strncmp(buffer, "download", 8) == 0) {
                printf("user download\n");

                char file_to_download[50];
                sprintf(file_to_download, "%.*s", strlen(buffer) - 9, buffer + 9);

                // file status
                char *file_exist = "found";
                char *file_not_exist = "not found";

                // find file
                int found = 0;
                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    if (strcmp(files[i].name, file_to_download) == 0) {
                        found = 1;
                        break;
                    }
                }

                if (found) {
                    send(new_socket, file_exist, sizeof(file_exist), 0);
                } else {
                    send(new_socket, file_not_exist, sizeof(file_not_exist), 0);
                    continue;
                }

                sleep(1);
                // send file
                char file_length[1024];
                char *file_content = (char *)malloc(sizeof(char) * 65536);
                char file_to_open[1024];

                memset(file_length, 0, sizeof(file_length));
                memset(file_to_open, 0, sizeof(file_to_open));
                
                sprintf(file_to_open, "FILES/%s", file_to_download);

                FILE *fptr;
                fptr = fopen(file_to_open, "r");

                fseek(fptr, 0, SEEK_END);
                long fsize = ftell(fptr);
                rewind(fptr);

                fread(file_content, 1, fsize, fptr);

                fclose(fptr);

                // send file size
                sprintf(file_length, "%ld", fsize);
                send(new_socket, file_length, sizeof(file_length), 0);
                sleep(1);

                // send file content
                for (long i = 0; i < fsize; i += 1024) {
                    memset(buffer, 0, sizeof(buffer));
                    sprintf(buffer, "%.*s", fsize < 1024 ? fsize : abs(fsize - i) < 1024 ? abs(fsize - 1) : 1024, file_content + i);
                    send(new_socket, buffer, sizeof(buffer), 0);
                }

            } else if (strncmp(buffer, "delete", 6) == 0) {
                printf("user delete\n");

                char file_to_delete[50];
                sprintf(file_to_delete, "%.*s", strlen(buffer) - 7, buffer + 7);
                delete(file_to_delete);

                FILE *fptr;
                fptr = fopen("running.log", "a");

                fprintf(fptr, "Hapus : %s (%s:%s)\n", file_to_delete, user_id, user_pwd);

                fclose(fptr);

                memset(file_to_delete, 0, sizeof(file_to_delete));
            } else if (strncmp(buffer, "see", 3) == 0) {
                printf("user see\n");

                struct file *files = parse_files();
                char *files_tsv = (char *)malloc(sizeof(char) * 1024);

                sprintf(files_tsv, "%s", see(files, n));
                send(new_socket, files_tsv, strlen(files_tsv), 0);
            } else if (strncmp(buffer, "find", 4) == 0) {
                printf("user find\n");

                char file_to_find[50];
                sprintf(file_to_find, "%.*s", strlen(buffer) - 5, buffer + 5);

                char *buf = (char *)malloc(sizeof(char) * 1024);
                memset(buf, 0, sizeof(buf));

                struct file *files = parse_files();
                for (int i = 0; i < n; ++i) {
                    char *p;
                    p = strstr(files[i].name, file_to_find);
                    if (p) {
                        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, files[i].year, files[i].ext, files[i].path);
                    }
                }

                if (strlen(buf) == 0) {
                    send(new_socket, file_pattern_not_found, strlen(file_pattern_not_found), 0);
                } else {
                    send(new_socket, buf, strlen(buf), 0);
                }
            } else {
                break;
            }
            printf("\n");
        }

        close(new_socket);
    }

    return 0;
}
