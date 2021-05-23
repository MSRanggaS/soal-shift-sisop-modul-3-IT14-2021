# soal-shift-sisop-modul-3-IT14-2021

Repository Sebagai Laporan Resmi Soal Shift Modul 3 Praktikum Sistem Operasi 2021
Disusun oleh :

- Romandhika Rijal I (0531180000048)
- Moch. Shaladin Rangga (05311940000029)
- Moh. Ibadul Haqqi (05311940000037)

---
# Daftar Isi

---
[Soal 1]
- [Soal no1](#soal-no1)
- [Pembahasan](#pembahasan-no1)

---
[Soal 2]
  * [Soal 2a](#soal-2a)
  * [Soal 2b](#soal-2b)  
  * [Soal 2c](#soal-2c)

---
[Soal 3]
- [soal](#soal-no3)
- [pembahasan](#pembahasan-no3)

---

# Soal no1
Keverk adalah orang yang cukup ambisius dan terkenal di angkatannya. Sebelum dia menjadi ketua departemen di HMTC, dia pernah mengerjakan suatu proyek dimana keverk tersebut meminta untuk membuat server database buku. Proyek ini diminta agar dapat digunakan oleh pemilik aplikasi dan diharapkan bantuannya dari pengguna aplikasi ini.

Di dalam proyek itu, Keverk diminta: 

a). Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt dengan format :

**akun.txt**
```
id:password
id2:password2
```

b). Sistem memiliki sebuah database yang bernama files.tsv. Isi dari files.tsv ini adalah path file saat berada di server, publisher, dan tahun publikasi. Setiap penambahan dan penghapusan file pada folder file yang bernama  FILES pada server akan memengaruhi isi dari files.tsv. Folder FILES otomatis dibuat saat server dijalankan. 

c). Tidak hanya itu, Keverk juga diminta membuat fitur agar client dapat menambah file baru ke dalam server. Direktori FILES memiliki struktur direktori di bawah ini : 

Direktori **FILES** 
```
File1.ekstensi
File2.ekstensi
```

Pertama client mengirimkan input ke server dengan struktur sebagai berikut :
Contoh Command Client :
```
add
```

Output Client Console:
```
Publisher:
Tahun Publikasi:
Filepath:
```
Kemudian, dari aplikasi client akan dimasukan data buku tersebut (perlu diingat bahwa Filepath ini merupakan path file yang akan dikirim ke server). Lalu client nanti akan melakukan pengiriman file ke aplikasi server dengan menggunakan socket. Ketika file diterima di server, maka row dari files.tsv akan bertambah sesuai dengan data terbaru yang ditambahkan.

d). Dan client dapat mendownload file yang telah ada dalam folder FILES di server, sehingga sistem harus dapat mengirim file ke client. Server harus melihat dari files.tsv untuk melakukan pengecekan apakah file tersebut valid. Jika tidak valid, maka mengirimkan pesan error balik ke client. Jika berhasil, file akan dikirim dan akan diterima ke client di folder client tersebut. 

Contoh Command client
```
download TEMPfile.pdf
```

e). Setelah itu, client juga dapat menghapus file yang tersimpan di server. Akan tetapi, Keverk takut file yang dibuang adalah file yang penting, maka file hanya akan diganti namanya menjadi ‘old-NamaFile.ekstensi’. Ketika file telah diubah namanya, maka row dari file tersebut di file.tsv akan terhapus.

Contoh Command Client:
```
delete TEMPfile.pdf
```

f). Client dapat melihat semua isi files.tsv dengan memanggil suatu perintah yang bernama see. Output dari perintah tersebut keluar dengan format. 

Contoh Command Client :
```
see
```

Contoh Format Output pada Client:
```
Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

Nama:
Publisher:
Tahun publishing:
Ekstensi File : 
Filepath : 

```

g). Aplikasi client juga dapat melakukan pencarian dengan memberikan suatu string. Hasilnya adalah semua nama file yang mengandung string tersebut. Format output seperti format output f.
Contoh Client Command:
```
find TEMP
```

h). Dikarenakan Keverk waspada dengan pertambahan dan penghapusan file di server, maka Keverk membuat suatu log untuk server yang bernama `running.log`. Contoh isi dari log ini adalah

`running.log`
```
Tambah : File1.ektensi (id:pass)
Hapus : File2.ektensi (id:pass)
```


# Pembahasan no1
## Pembahasan Source Code Client

- mendefinisikan port nya
```c
#define PORT 8080
```


- Jike pembuatan socket tidak berhasil, maka menampilkan `Socket creation error`
- jika gagal menyambungkan ke server manapun maka tampil `Connection Failed`
```c
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
```

- Client akan menginput pilihan login dan register. Setelah itu client akan menginputkan id beserta passwordnya. Data-data ini nantinya akan dikirim ke server. Jika login berhasil maka client akan mendapatkan tampilan login success. Setelah mendapatkan tampilan tersebut, client dapat menginputkan command add untuk menambahkan file, maka client akan diminta untuk memasukkan `Publisher`, `Tahun publikasi`, dan `Filepath-nya`. Kemudian client akan mengirimkan file beserta ukuran dan isinya ke server. Client juga dapat menginputkan command `download` untuk mendownload file dari server. Nantinya client akan menerima file yang didownload dari server. Jika file yang ingin didownload tidak ditemukan maka ditampilkan `"file not found"`. Client juga dapat menginputkan command delete untuk menghapus file yang diinginkan, command ini nantinya akan dikirim ke server. Selain itu, client juga dapat menginputkan command `see` dan client akan menerima data file yang tersedia pada server. Client pun juga dapat menginputkan command `find` untuk mencari file yang ingin dicari pada server, command ini nantinya akan dikirim oleh client ke server dan client nantinya akan menerima file yang dicari pada server.
```c  
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
```

## Penjelasan Source Code Server

- Mendefinisikan port server, kemudian membuat struct untuk user yang terdiri dari nama atau id user dan password user. Setelah itu membuat struct untuk files yang berisi nama file dan path filenya sebagai berikut.

```c
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
```
- Membuat perintah see dimana nantinya akan memunculkan semua file yang ada. Dimana nantinya output yang ditampilkan terdapat Nama Filesnya, Publishernya, Tahun publikasi file, dan Filepathnya.

```c
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
```

- Setelah itu membuat fungsi find_user dimana fungsi ini digunakan untuk mencari akun atau user yang sudah melakukan pendaftaran. Fungsi ini nantinya akan dijalankan saat user melakukan login

```c
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
```

- Membuat fungsi register untuk membantu user mendaftarkan akun. Fungsi ini juga akan menuliskan atau mengedit daftar nama akun dan password yang akan terdaftar pada akun.txt

```c
//fungsi register
void reg(char *name, char *pwd) {
    FILE *fptr;

    //membuat file dengan nama akun.txt
    fptr = fopen("akun.txt", "a");

    // isinya id sama password yg terdaftar
    fprintf(fptr, "%s:%s\n", name, pwd);
    
    fclose(fptr);
}
```

- Membuat struct files yang nantinya akan menjadi database pada program ini. Database ini menyimpan nama file, path file, nama publisher dan tahun publish dari file-file yang sudah ditambahkan sebelumnya

```c
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
```

- Setelah itu membuat fungsi delete dimana fungsi ini akan menghapus file yang tersimpan di server dengan mengganti namanya menjadi old-(namafile).ekstensi. Jika gagal merename nama file yang akan dihapus maka ditampilkan "error renaming (nama file sebelum dihapus) to (nama file sesudah dihapus). Setelah itu, program akan menghapus row dari file ini pada database file.tsv

```c
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
```

- Untuk di fungsi main, nantinya jika server telah tersambung dengan client maka server akan mengirimkan pilihan untuk login atau register pada client. Kemudian jika client memilih register, maka client diminta untuk menginputkan id dan password untuk akunnya. Setelah pendaftaran akun selesai, server akan mengirim pesan register success pada client. Namun, jika client memilih pilihan login, maka client diminta untuk memasukkan id dan password yang telah ia daftarkan sebelumnya. Jika berhasil masuk, server akan menampilkan login success. Kemudian server akan menerima command add dari client yang nantinya akan menambahkan file, setelah itu server akan mengirimkan format penambahan file yang akan dimasukkan oleh client sebagai berikut.
```
Publisher:
Tahun Publikasi:
Filepath:
```
Kemudian pada `file.tsv` menyimpan file yang baru ditambahkan tersebut sedangkan pada running.log akan mencatat command add, path, nama user, dan password user yang menambahkan file tersebut. Setelah itu server akan menerima file, ukuran file, dan isi dari file tersebut. Selain menerima command add, server juga menerima command download dari client. Server nantinya akan mencari file yang client ingin download kemudian mengirim file beserta ukuran dan isinya ke client. Server juga dapat menerima command delete, setelah menerima command ini server akan memanggil fungsi delete dan mencatatkan record command Hapus file pada running.log. Server pun juga dapat menerima command see dari client yang nantinya akan memanggil fungsi see yang memperlihatkan file yang ada pada server dan mengirimkannya ke client. Dan command terakhir yang dapat diterima oleh server adalah command find. Command ini nantinya akan mencari file yang dicari oleh client dan mengirimkannya ke client.

```c
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
                        sprintf(buf, "%s\nNama: %s\nPublisher: %s\nTahun publishing: %s\nEkstensi File: %s\nFilepath: %s\n", buf, files[i].name, files[i].publisher, 					files[i].year, files[i].ext, files[i].path);
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
```
Kendala:
Karena kurangnya waktu, soal nomer 1 belum sempat selesai sampai batas waktu pengumpulan.

## Soal 2a
**Deskripsi:**\
Membuat program perkalian matrix (4x3 dengan 3x6) dan menampilkan hasilnya. Matriks nantinya akan berisi angka 1-20 (tidak perlu dibuat filter angka).

**Pembahasan:**\
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
```
* `#include <stdio.h>` Library untuk fungsi input-output (e.g. printf(), sprintf())
* `#include <sys/ipc.h>` Library digunakan untuk tiga mekanisme interprocess communication (IPC)(e.g. semaphore)
* `#include <sys/shm.h>` Library untuk mendefinisikan symbolic constants structure seperti(SHM_RDONLY,SHMLBA)
* `#include <stdlib.h>` Library untuk fungsi umum (e.g. exit(), atoi())
* `#include <unistd.h>` Llibrary untuk melakukan system call kepada kernel linux (e.g. fork())

```c
int mat1[4][3];
int mat2[3][6];
int mat3[4][6];
int row = 0, i, j, k;
```
* mengidentifikasi `matriks1  4x3`, `matriks2 3x6`, `matriks3 4x6` 

```c
void input(int mat1[4][3], int mat2[3][6]) {
    printf("Input matriks pertama 4x3\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            scanf("%d", &mat1[i][j]);
        } 
    }printf("\n");
    printf("Input matriks kedua 3x6\n");
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 6; j++) {
            scanf("%d", &mat2[i][j]);
        } 
    }printf("\n");
}
```
* menjalankan program `mat1[4][3]` menggunakan fungsi for loop
* setelah `mat1` sudah di proses maka akan melanjutkan program `mat2[3][6]` memakai fungsi for loop, lalu setelah selesai for loop akan menampilkan hasil `mat2` tersebut

```c
void mult(int mat1[4][3], int mat2[3][6], int mat3[4][6]) {
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            mat3[i][j] = 0;
            for (k = 0; k < 3; k++)
                mat3[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}
```
* memproses matriks3 terlebih dahulu dengan for loop 
* Lalu hasil perkalian akan ditambahkan dan dimasukan ke `matriks3`. Pada case ini ordo matriks hasil adalah (4*5), karena ordo matriks hasil perkalian dua buah matriks adalah jumlah baris pertama dikali jumlah kolom ke dua.

```c
int main() {
	key_t key = 6970;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
```
- Disini kami membuat shared memory untuk `matriks3` sesuai dengan template pembuatan shared memory yang ada pada modul, karena nanti `matriks3` akan digunakan untuk acuan dari soal 2.b

```c
    input(mat1, mat2);
    mult(mat1, mat2, mat3);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            printf("%d ", mat3[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            value[i * 6 + j] =  mat3[i][j];
        }
    }
        sleep(15);
    printf("Selesai ngirim\n");

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
```
- input `mat1` dan `mat2`, lalu `mat3`. output dari `mat3` sendiri adalah dari perkalian `mat1` dan `mat2` yang akan menghasilkan matriks 4x6. setelah output mat3 keluar akan `sleep(15)` lalu akan keluar `selesai mengirim`

## Soal 2b
**Deskripsi:**\
Membuat program dengan menggunakan matriks output dari program sebelumnya (program soal2a.c) (Catatan!: gunakan shared memory). Kemudian matriks tersebut akan dilakukan perhitungan dengan matrix baru (input user) sebagai berikut contoh perhitungan untuk matriks yang ada. Perhitungannya adalah setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya (dari paling besar ke paling kecil)

**Pembahasan:**
```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>
```
* `#include <stdio.h>` Library untuk fungsi input-output (e.g. printf(), sprintf())
* `#include <sys/ipc.h>` Library digunakan untuk tiga mekanisme interprocess communication (IPC)(e.g. semaphore)
* `#include <sys/shm.h>` Library untuk mendefinisikan symbolic constants structure seperti(SHM_RDONLY,SHMLBA)
* `#include <stdlib.h>` Library untuk fungsi umum (e.g. exit(), atoi())
* `#include <unistd.h>` Llibrary untuk melakukan system call kepada kernel linux (e.g. fork())
* `#include <pthread.h>` Library untuk operasi thread (e.g. pthread_create(), ptrhead_exit() )

```c
int row = 0, n, i, j, k, w, temp[24];
int matb[4][6];
struct tes{
    int t;
    int u;
    int v;
};
```
- Pertama kami membuat array dan matriks dengan ordo sesuai dengan output matriks soal 2
- Mendefinisikan struct dengan atribut t, u, v  yang nanti digunakan sebagai baris dan kolom

```c
void input(int matb[4][6]) {
    printf("Input matriks B 4x6\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            scanf("%d", &matb[i][j]);
        } 
    }printf("\n");
}
```
- disini kita akan menginput sebuah matriks dengan ordo 4x6, `for()` loop yang pertama untuk ordo 4, dan `for()` loop yang kedua untuk ordo 6, lalu kita print hasilnya

```c
long factorial(int n) {  
    if (n == 0)  
        return 1;  
    else  
        return(n * factorial(n-1));  
}  
void* func(void* arg) {
    int x;
    int to = ((struct tes*)arg)->t;
    int uo = ((struct tes*)arg)->u;
    int vo = ((struct tes*)arg)->v;
    x = to * 6 + uo;
    w++;
    if(vo >= matb[to][uo]){
        temp[x]=factorial(vo)/factorial(vo-matb[to][uo]);
    }
    else if(matb[to][uo] > vo){
        temp[x]=factorial(vo);
    }
    else if((vo == 0) && (matb[to][uo] == 0)){
        temp[x] = 0;
    }
}
```
- Pendefinisian fungsi `penjumalah` dengan pembuatan `struct` yang akan menset atribut to, uo, vo nya keadalam variable t, u, v
- For() loops disini akan berjalan dengan counter atribut-atribut yang akan di increment untuk setiap baris dan kolom dari matriks.

```c
int main() {
	key_t key = 6970;
    int *value;
    int cell;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
```
- Pada `main()`, pertama-tama kami akan membuat shared memory untuk matriks, sesuai dengan template pembuatan shared memory yang ada pada modul

```c
    printf("Matriks A 4x6\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            printf("%d ", value[i * 6 + j]);
        } printf("\n");
    } printf("\n");
    input(matb);
    printf("Matriks B 4x6\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            printf("%d ", matb[i][j]);
        } printf("\n");
    } printf("\n");
    pthread_t tid[24];
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            cell = value[i * 6 + j];
            struct tes *cek = (struct tes*)malloc(sizeof(struct tes));
            cek->t = i;
            cek->u = j;
            cek->v = cell;
            pthread_create(&(tid[i * 6 + j]), NULL, &func, (void*) cek);
        } 
    } 
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            pthread_join(tid[i * 6 + j], NULL);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            printf("%d ", temp[i * 6 + j]);
        }printf("\n");
    }printf("\n");

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}
```
- pertama menjalankan matriks A yang uda ada pada penjelasan sebelumnya.
- Setelah matriks A selesai maka akan menjalankan matriks B

## soal 2c
**Deskripsi:**\
Karena takut lag dalam pengerjaannya membantu Loba, Crypto juga membuat program (soal2c.c) untuk mengecek 5 proses teratas apa saja yang memakan resource komputernya dengan command “ps aux | sort -nrk 3,3 | head -5”

**Pembahasan:**
```c
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
```
- `#include <stdio.h>` Library untuk fungsi input-output (e.g. printf(), sprintf())
- `#include <stdlib.h>` Library untuk fungsi umum (e.g. exit(), atoi())
- `#include <unistd.h>` Llibrary untuk melakukan system call kepada kernel linux (e.g. fork())
- `#include <sys/types.h>` Library tipe data khusus (e.g. pid_t)
- `#include<sys/wait.h>` Library untuk pendefinisian symbolic constants untuk penggunaan waitpid(): (e.g. WNOHANG)

```c
int main() {
    pid_t child_id;
    int status;
    int fp1[2];
    int fp2[2];
    char output[1000];
```
- mendefinisikan `int main()`

```c
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
```
- jika `pipe(fp1)` dan `pipe(fp2)` `==-1`, maka akan `Pipe Failed`

```c
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
```
- Disini akan dilakukan `fork()` dan untuk parent proccesnya, dia akan membuat copy `fp[1]` yang berfungsi sebagai `write` end dari pipe
- Selanjutnya akan dilakukan pengecekan program diatas agar tidak terjadi kendala lag pada saat menjalankannya dengan perintah `for()` pada setiap command `“ps aux | sort -nrk 3,3 | head -5”`

Kendala:
Tidak ada
 
## soal 3
Seorang mahasiswa bernama Alex sedang mengalami masa gabut. Di saat masa gabutnya, ia memikirkan untuk merapikan sejumlah file yang ada di laptopnya. Karena jumlah filenya terlalu banyak, Alex meminta saran ke Ayub. Ayub menyarankan untuk membuat sebuah program C agar file-file dapat dikategorikan. Program ini akan memindahkan file sesuai ekstensinya ke dalam folder sesuai ekstensinya yang folder hasilnya terdapat di working directory ketika program kategori tersebut dijalankan.

Contoh apabila program dijalankan:
```
# Program soal3 terletak di /home/izone/soal3
$ ./soal3 -f path/to/file1.jpg path/to/file2.c path/to/file3.zip
#Hasilnya adalah sebagai berikut
/home/izone
|-jpg
|--file1.jpg
|-c
|--file2.c
|-zip
|--file3.zip
```

a). Program menerima opsi -f seperti contoh di atas, jadi pengguna bisa menambahkan argumen file yang bisa dikategorikan sebanyak yang diinginkan oleh pengguna. 
Output yang dikeluarkan adalah seperti ini :
```
File 1 : Berhasil Dikategorikan (jika berhasil)
File 2 : Sad, gagal :( (jika gagal)
File 3 : Berhasil Dikategorikan
```

b). Program juga dapat menerima opsi -d untuk melakukan pengkategorian pada suatu directory. Namun pada opsi -d ini, user hanya bisa memasukkan input 1 directory saja, tidak seperti file yang bebas menginput file sebanyak mungkin. Contohnya adalah seperti ini:
```
$ ./soal3 -d /path/to/directory/
```
Perintah di atas akan mengkategorikan file di /path/to/directory, lalu hasilnya akan disimpan di working directory dimana program C tersebut berjalan (hasil kategori filenya bukan di /path/to/directory).
Output yang dikeluarkan adalah seperti ini :
```
Jika berhasil, print “Direktori sukses disimpan!”
Jika gagal, print “Yah, gagal disimpan :(“
```

c). Selain menerima opsi-opsi di atas, program ini menerima opsi *, contohnya ada di bawah ini:
```
$ ./soal3 \*
```

Opsi ini akan mengkategorikan seluruh file yang ada di working directory ketika menjalankan program C tersebut.

c). Semua file harus berada di dalam folder, jika terdapat file yang tidak memiliki ekstensi, file disimpan dalam folder “Unknown”. Jika file hidden, masuk folder “Hidden”.
d). Setiap 1 file yang dikategorikan dioperasikan oleh 1 thread agar bisa berjalan secara paralel sehingga proses kategori bisa berjalan lebih cepat.



- di dalam int main terdapat 3 if else
- if yang pertama mengecek apakah argv berupa `"-f"` atau tidak jika iya maka program akan mengkategorikan file dengan menjalankan function `categorize_files`
- if yang kedua mengecek apakah argv berupa `"-d"` atau tidak jika iya maka program akan melakukan pengecekan kembali apakah argc tidak sama dengan 3 atau sama, jika tidak sama maka akan menjalankan error. Dilanjutkan dengan menjalankan function categorize_dir yang akan mengkategorikan file.
- if yang terakhir mengecek apakah `argv` berupa `""` atau tidak, jika iya maka program akan megecek kembali apakah `argc` lebih dari 2, jika iya maka akan menjalankan error, namun jika tidak akan menjalankan function categorize_dir yang berfungsi untuk mengkategorikan file. Karena argv berupa `""` maka yang dikategorikan filenya adalah file yang di direktori saat ini

```c
    if (strcmp("-f", argv[1]) == 0) {
        if (argc < 3) err();

        categorize_files(argc, argv, active_dir);
    } else if (strcmp("-d", argv[1]) == 0) {
        if (argc != 3) err();
        mode = 1;

        sprintf(dir, "%s", argv[2]);

        categorize_dir(dir, active_dir, dir);

        for (int i = 0; i < current_thread; ++i) {
            pthread_join(tid[i], NULL);
        }

        if (status) {
            printf("Direktori sukses disimpan!\n");
        } else {
            printf("Yah, gagal disimpan :(\n");
        }
    } else if (strcmp("*", argv[1]) == 0) {
        if (argc > 2) err();
        mode = 1;

        getcwd(dir, 1024);

        categorize_dir(dir, active_dir, active_dir);

        for (int i = 0; i < current_thread; ++i) {
            pthread_join(tid[i], NULL);
        }

        if (status) {
            printf("Direktori sukses disimpan!\n");
        } else {
            printf("Yah, gagal disimpan :(\n");
        }
    } else {
        err();
    }

    exit(EXIT_SUCCESS);
}
```
Kendala:
Masih belum menguasai penggunaan thread sehingga sedikit kebingungan dalam melakukan hal yang diminta soal

# Dokumentasi
## 1a.
### Register

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845181307050786816/1a1.jpg">

### Login

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845181336544739338/1a2.jpg">

### akun.txt

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845181366915825704/1a3.jpg">

### Antri client

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845183211008688138/1a4.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845183247830614016/1a5.jpg">

## 1b.
### files.tsv
files.tsv ini akan terisi apabila sudah melakukan command add

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845183967111020594/1b.jpg">

## 1c.
### 5 file yang akan diupload
Berikut adalah 5 file yang akan diupload ke server beserta isi tiap file

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184393962192916/1cbuku1.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184431429910548/1cbuku2.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184431622324254/1cbuku3.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184433853693962/1cbuku4.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184421455724564/1cbuku5.jpg">

### command add

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184812393824256/1c1.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845184807248199720/1c2.jpg">

## 1d.
### command download
Pada command download ini kami memberi percobaan dengan mengedit isi file yang akan di download pada server, setelah di download maka benar terlihat bahwa isi file berubah.

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185011317080094/1dbuku3.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185006674509834/1d1.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185008356425798/1dbuku32.jpg">

## 1e.
### command delete
Bisa dilihat pada file.tsv, bahwa file yang dihapus maka di file.tsv juga terhapus 

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185287360872458/1e1.jpg">
<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185289080406026/1e2.jpg">

## 1f.
### command see

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185416046313482/1f.jpg">

## 1g.
### command find

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185437899161620/1g.jpg">

## 1h.
### running.log
Pada running.log akan merekap apa saja yang terjadi pada file di server

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845185461306785862/1h.jpg">

## 2a.
Membuat sebuah program perkalian matriks dengan shared memory.

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845197525127397376/2a.jpg">

## 2b.
Hasil perkalian matriks pada program 2a dilakukan perhitungan dimana setiap cel yang berasal dari matriks A menjadi angka untuk faktorial, lalu cel dari matriks B menjadi batas maksimal faktorialnya.

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845197541820858389/2b.jpg">

## 2c.
Menampilkan 5 proses teratas apa saja yang memakan resource komputer.

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845197561521635368/2c.jpg">

## 3a. 
File file awal yang akan dikategorikan

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845198348381585458/3ori.jpg">

### argumen -f
Mengkategorikan 2 file dari folder soal3

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845198362159349820/3a.jpg">

## 3b.
### argumen -d
Mengkategorikan folder soal3

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845198370699083786/3b.jpg">

## 3c.
### argumen *
Mengkategorikan seluruh file pada direktori tersebut, terlihat bahwa file program3 dan program3.c juga ikut dikategorikan.

<img src="https://cdn.discordapp.com/attachments/841192613917884436/845198372506959872/3c.jpg">
