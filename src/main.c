#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include <lemipc.h>
#include <types.h>

int main(int argc, char *argv[])
{
    int             fd;
    char*           shmpath;
    t_game_state    *shmp;

    if (argc != 2) {
        fprintf(stderr, "Usage: lemipc <team>\n");
        exit(EXIT_FAILURE);
    }
    
    int team_number = atoi(argv[1]);
    shmpath = "lemipc";

    fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    
    if (fd == -1 && errno == EEXIST) { 
        printf("errno == EEXIST\n");
    if ((shmp = mmap(
        NULL,
        sysconf(_SC_PAGESIZE),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    )) == MAP_FAILED) err(EXIT_FAILURE, "mmap");

        printf("second player: %d\n", shmp->b_resource_created);
        if (sem_post(&shmp->init) == -1)
            err(EXIT_FAILURE, "sem_post");
    } else if(fd == -1) {
        shm_unlink(shmpath);
        err(EXIT_FAILURE, "shm_open");
    }

    // first player
    if (ftruncate(fd, sizeof(void)) == -1)
        err(EXIT_FAILURE, "ftruncate");

    if ((shmp = mmap(
        NULL,
        sysconf(_SC_PAGESIZE),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    )) == MAP_FAILED) err(EXIT_FAILURE, "mmap");

    if (shmp->b_resource_created == 0) {
        // first player, initialize shared resources
        printf("first player\n");

        shmp = &(t_game_state) {.board={0} };
        
        if(sem_init(&shmp->play, 1, 0) == -1)
            err(EXIT_FAILURE, "sem_init-init");

        if (sem_wait(&shmp->play) == -1)
            err(EXIT_FAILURE, "sem_wait");
        
        printf("sem_unlocked\n");
    }

    for (;;) {
        if (sem_wait(&shmp->play) == -1)
            err(EXIT_FAILURE, "sem_wait");
        // this player turn
        // check board. If only 1 team the game is over
        // if not move to closest enemy
        sem_post(&shmp->play); // move to next player
    }

    shm_unlink(shmpath);
    munmap(shmp, sysconf(_SC_PAGESIZE));
    exit(EXIT_SUCCESS);
}
