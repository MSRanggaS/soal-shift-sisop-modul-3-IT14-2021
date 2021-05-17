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
- Disini kami membuat shared memory untuk `matriks3` sesuai dengan template pembuatan shared memory yang ada pada modul, karena nanti `matriks3` akan digunakan untuk acuan dari soal 4.b

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
- aaaa

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
