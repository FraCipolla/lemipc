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

        shmp = &(t_game_state) {
            .b_resource_created=0,
            .board=&(t_board) { .width=256, .height=256, .players=NULL},
            .n_players=1,
            .n_teams=1,
            .teams=&(t_team){.team_idx=team_number, .team_size=1,.players=NULL,.next=NULL}
        };
        
        shmp->b_resource_created = 1;
        
        if(sem_init(&shmp->init, 1, 0) == -1)
            err(EXIT_FAILURE, "sem_init-init");

        if (sem_wait(&shmp->init) == -1)
            err(EXIT_FAILURE, "sem_wait");
        
        printf("sem_unlocked\n");
    }
    /* Initialize semaphores as process-shared, with value 0.  */

    // if (sem_init(&shmp->sem1, 1, 0) == -1)
    //     err(EXIT_FAILURE, "sem_init-sem1");
    // if (sem_init(&shmp->sem2, 1, 0) == -1)
    //     err(EXIT_FAILURE, "sem_init-sem2");

    // /* Wait for 'sem1' to be posted by peer before touching
    //    shared memory.  */

    // if (sem_wait(&shmp->sem1) == -1)
    //     err(EXIT_FAILURE, "sem_wait");

    // /* Convert data in shared memory into upper case.  */

    // for (size_t j = 0; j < shmp->cnt; j++)
    //     shmp->buf[j] = toupper((unsigned char) shmp->buf[j]);

    // /* Post 'sem2' to tell the peer that it can now
    //    access the modified data in shared memory.  */

    // if (sem_post(&shmp->sem2) == -1)
    //     err(EXIT_FAILURE, "sem_post");

    /* Unlink the shared memory object.  Even if the peer process
       is still using the object, this is okay.  The object will
       be removed only after all open references are closed.  */

    shm_unlink(shmpath);
    munmap(shmp, sysconf(_SC_PAGESIZE));
    exit(EXIT_SUCCESS);
}
