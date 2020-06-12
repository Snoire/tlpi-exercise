/* Exercise 5-4 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>


int dup_f(int oldfd)
{
    int newfd;
    newfd = fcntl(oldfd, F_DUPFD, 0);

    return newfd;
}

int dup2_f(int oldfd, int newfd)
{
    if (fcntl(oldfd, F_GETFL) == -1) {  /* oldfd is invalid */
        errno = EBADF;
        return -1;
    }

    if (newfd == oldfd)   /* newfd has the same value as oldfd */
        return newfd;

    if (fcntl(newfd, F_GETFL) == -1) {
        if (errno != EBADF) {
            fprintf(stderr, "unknown error..\n");
            return -1;
        }
    } else     /* newfd was previously opened */
        if (close(newfd) == -1)
            return -1;

    return fcntl(oldfd, F_DUPFD, newfd);
}

