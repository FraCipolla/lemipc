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
    struct shmbuf   *shmp;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s /shm-path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    int team_number = atoi(argv[1]);
    shmpath = "lemipc";

    /* Create shared memory object and set its size to the size
       of our structure.  */

    fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (fd == -1 && errno == EEXIST) {
        // we are not the first player, resources already exists

    } else if (fd == -1) {
        err(EXIT_FAILURE, "shm_open");
    }

    // first player
    if (ftruncate(fd, sizeof(struct shmbuf)) == -1)
        err(EXIT_FAILURE, "ftruncate");

    /* Map the object into the caller's address space.  */

    if (shmp = mmap(
        NULL,
        sysconf(_SC_PAGESIZE),
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        0
    ) == MAP_FAILED) err(EXIT_FAILURE, "mmap");

    // create share resources
    shmp = &(t_game_state) {
        .start=0,
        .board=&(t_board) { .width=256, .height=256, .players=NULL},
        .n_players=1,
        .n_teams=1,
        .teams=&(t_team){.team_idx=team_number, .team_size=1,.players=NULL,.next=NULL}
    };
    

    /* Initialize semaphores as process-shared, with value 0.  */

    if (sem_init(&shmp->sem1, 1, 0) == -1)
        err(EXIT_FAILURE, "sem_init-sem1");
    if (sem_init(&shmp->sem2, 1, 0) == -1)
        err(EXIT_FAILURE, "sem_init-sem2");

    /* Wait for 'sem1' to be posted by peer before touching
       shared memory.  */

    if (sem_wait(&shmp->sem1) == -1)
        err(EXIT_FAILURE, "sem_wait");

    /* Convert data in shared memory into upper case.  */

    for (size_t j = 0; j < shmp->cnt; j++)
        shmp->buf[j] = toupper((unsigned char) shmp->buf[j]);

    /* Post 'sem2' to tell the peer that it can now
       access the modified data in shared memory.  */

    if (sem_post(&shmp->sem2) == -1)
        err(EXIT_FAILURE, "sem_post");

    /* Unlink the shared memory object.  Even if the peer process
       is still using the object, this is okay.  The object will
       be removed only after all open references are closed.  */

    shm_unlink(shmpath);

    exit(EXIT_SUCCESS);
}