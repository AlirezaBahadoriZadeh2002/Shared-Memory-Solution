#include<stdio.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024
#define SHM_KEY 0x1234


int main(int argc, char *argv[]) {
    int shmid = shmget (SHM_KEY, BUF_SIZE, 0644| IPC_CREAT);
    if (shmid == -1){
        perror("Cannot create shared memory!");
        return 1;
    }

    // Attach to the segment to get a pointer to it.
    char* buf = shmat(shmid, NULL, 0);
    if (buf == (void *) -1) {
        perror("Cannot attach shared memory attach");
        return 1;
    }
    for (int numtimes = 0; numtimes < 30; numtimes++) {
        printf("Producer Process: Read => %s \n", buf);
        sleep (1);
    }
    if (shmdt (buf) == -1) {
        perror("Cannot detach shared memory!");
        return 1;
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        perror("Cannot remove shared memory!");
        return 1;
    }
    printf("Producer Process: Completed.\n");
    return 0;    
}
