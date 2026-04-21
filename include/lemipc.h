#ifndef LEMIPC_H
#define LEMIPC_H

#include <sys/msg.h>
#include <err.h>
#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define BUF_SIZE 1024   /* Maximum size for exchanged string */

struct shmbuf {
    sem_t  sem1;            /* POSIX unnamed semaphore */
    sem_t  sem2;            /* POSIX unnamed semaphore */
    size_t cnt;             /* Number of bytes used in 'buf' */
    char   buf[BUF_SIZE];   /* Data being transferred */
};


/*
ERRORS
       EACCES Permission to shm_unlink() the shared memory object was
              denied.

       EACCES Permission was denied to shm_open() name in the specified
              mode, or O_TRUNC was specified and the caller does not have
              write permission on the object.

       EEXIST Both O_CREAT and O_EXCL were specified to shm_open() and
              the shared memory object specified by name already exists.

       EINVAL The name argument to shm_open() was invalid.

       EMFILE The per-process limit on the number of open file
              descriptors has been reached.

       ENAMETOOLONG
              The length of name exceeds PATH_MAX.

       ENFILE The system-wide limit on the total number of open files has
              been reached.

       ENOENT An attempt was made to shm_open() a name that did not
              exist, and O_CREAT was not specified.

       ENOENT An attempt was to made to shm_unlink() a name that does not
              exist.
*/

#endif