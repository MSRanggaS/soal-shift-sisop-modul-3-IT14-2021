#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <pthread.h> 
#include <unistd.h>

int row = 0, n, i, j, k, w, temp[24];
int matb[4][6];
struct tes{
    int t;
    int u;
    int v;
};
void input(int matb[4][6]) {
    printf("Input matriks B 4x6\n");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 6; j++) {
            scanf("%d", &matb[i][j]);
        } 
    }printf("\n");
}
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

int main() {
	key_t key = 6970;
    int *value;
    int cell;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

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
