#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define N 5 /* число философов*/

int mas_phils[N];
int semid;
struct sembuf sops[3];

void kill_philosophers() {
    int i;
    signal(SIGINT, SIG_DFL);
    for(i = 0; i <= N - 1; i++) {
        kill(mas_phils[i], SIGINT);
    }
}

void think(int i, int sleep_time) {
    printf("%d (%d) - started thinking...\n", i, sleep_time);
    fflush(stdout);
    sleep(sleep_time);
    printf("%d - stopped thinking\n", i);
    fflush(stdout);
}

void sit(int i) {
    sops[0].sem_num = (i + N - 1) % N;
    sops[0].sem_op = -1;
    sops[0].sem_flg = 0;
    sops[1].sem_num = i;
    sops[1].sem_op = -2;
    sops[1].sem_flg = 0;
    sops[2].sem_num = (i + 1) % N;
    sops[2].sem_op = -1;
    sops[2].sem_flg = 0;
    semop(semid, sops, 3);
}

void eat(int i) {
    printf("      %d - started nyam-nyam...\n", i);
    fflush(stdout);
    sleep(10);
    printf("      %d - finished nyam-nyam\n", i);
    fflush(stdout);
}

void wake(int i) {
    sops[0].sem_num = (i + N - 1) % N;
    sops[0].sem_op = 1;
    sops[0].sem_flg = 0;
    sops[1].sem_num = i;
    sops[1].sem_op = 2;
    sops[1].sem_flg = 0;
    sops[2].sem_num = (i + 1) % N;
    sops[2].sem_op = 1;
    sops[2].sem_flg = 0;
    semop(semid, sops, 3);
}

void philosopher(int i, int sleep_time) /* i - номер философа от 0 до 4*/
{
    while(1) {
        think(i, sleep_time); /*философ гуляет, думает, размышляет*/
        sit(i);
        eat(i); // ест спагетти, ням-ням
        wake(i);
    }
}

int main()
{
    int i, pid, q;
    key_t key;
    key = ftok("/Users/maxbig/f.txt", 's');
    semid = semget(key, N, 0666 | IPC_CREAT);
    for(i = 0; i <= N - 1; i++) {
        semctl(semid, i, SETVAL, (int) 2);
    }
    
    signal(SIGINT, kill_philosophers);
    for(i = 0; i <= N - 1; i++) {
        q = 3 + rand() % 6;
        if ((pid = fork()) == 0) {
            signal(SIGINT, SIG_DFL);
            philosopher(i, q);
        }
        else {
            mas_phils[i] = pid;
        }
    }
    for(i = 0; i <= N - 1; i++) {
        wait(NULL);
    }
    return 0;
}
