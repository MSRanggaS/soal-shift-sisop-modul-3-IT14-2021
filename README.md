# soal-shift-sisop-modul-3-IT14-2021

Repository Sebagai Laporan Resmi Soal Shift Modul 3 Praktikum Sistem Operasi 2021
Disusun oleh :

- Romandhika Rijal I (0531180000048)
- M. Shaladin Rangga (05311940000029)
- Moh. Ibadul Haqqi (05311940000037)

---
# Daftar Isi

---
* [Soal 1]

---
* [Soal 2]

---
* [Soal 3]

---
# Soal 1
**Deskripsi:**\
Pada saat client tersambung dengan server, terdapat dua pilihan pertama, yaitu register dan login. Jika memilih register, client akan diminta input id dan passwordnya untuk dikirimkan ke server. User juga dapat melakukan login. Login berhasil jika id dan password yang dikirim dari aplikasi client sesuai dengan list akun yang ada didalam aplikasi server. Sistem ini juga dapat menerima multi-connections. Koneksi terhitung ketika aplikasi client tersambung dengan server. Jika terdapat 2 koneksi atau lebih maka harus menunggu sampai client pertama keluar untuk bisa melakukan login dan mengakses aplikasinya. Keverk menginginkan lokasi penyimpanan id dan password pada file bernama akun.txt

**Pembahasan:**
## Client
```c
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
```

## Server
```c
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
```

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
 
## soal 3

```c
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
```
- pertama kita masukkan library yang akan digunakan nantinya

```c

```
