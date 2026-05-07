#include <signals.h>
#include <utils.h>

void handle_sigint(int sig) {
    int fd = shm_open("lemipc", O_RDWR, 0600);

    if (fd == -1 && errno != EEXIST) return;

    t_game_state *shmp = mmap(
            NULL,
            sysconf(_SC_PAGESIZE),
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            fd,
            0
            );
    if (!t_game_state)
        err(EXIT_FAILURE, "mmap");
        
    if (sem_wait(&shmp->play) == -1)
        err(EXIT_FAILURE, "sem_wait");

       
    // add logic to use semaphore && check if last player && cleanup
}
