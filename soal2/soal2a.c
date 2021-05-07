#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>

int mat1[4][3];
int mat2[3][6];
int mat3[4][6];
int row = 0, i, j, k;
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
void mult(int mat1[4][3], int mat2[3][6], int mat3[4][6]) {
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            mat3[i][j] = 0;
            for (k = 0; k < 3; k++)
                mat3[i][j] += mat1[i][k] * mat2[k][j];
        }
    }
}
int main() {
	key_t key = 6970;
    int *value;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

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
