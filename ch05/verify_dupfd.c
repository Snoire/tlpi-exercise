/* Exercise 5-5 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "dup_fcntl.h"

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);\
                          } while (0)

int main()
{
    int fd0, fd1, fd2=12;
    /* verify that duplicated file descriptors share a file offset value */
    fd0 = open("tfile", O_RDWR | O_CREAT, 0644);
    if (fd0 == -1)
        errExit("open");

    if ((fd1 = dup_f(fd0)) == -1)
        errExit("dup_f");
    if ((dup2_f(fd0, fd2)) == -1)
        errExit("dup_f");

    if (write(fd0, "test", 4) == -1)
        errExit("write");

    off_t curoff0, curoff1, curoff2;
    
    if ((curoff0 = lseek(fd0, 0, SEEK_CUR)) == -1)
        errExit("lseek");
    if ((curoff1 = lseek(fd1, 0, SEEK_CUR)) == -1)
        errExit("lseek");
    if ((curoff2 = lseek(fd2, 0, SEEK_CUR)) == -1)
        errExit("lseek");

    if (curoff0 == curoff1 && curoff1 == curoff2)
        printf("duplicated file descriptors share a file offset value\n");

    /* verify that duplicated file descriptors share open file status flags */
    int flags0, flags1, flags2, accessMode;

    if ((flags0 = fcntl(fd0, F_GETFL)) == -1)
        errExit("fcntl");
    if ((flags1 = fcntl(fd1, F_GETFL)) == -1)
        errExit("fcntl");
    if ((flags2 = fcntl(fd2, F_GETFL)) == -1)
        errExit("fcntl");

    if (flags0 == flags1 && flags1== flags2)
        printf("duplicated file descriptors share open file status flags\n");

    accessMode = flags0 & O_ACCMODE;
    if (accessMode == O_RDWR)
        printf("read/write\n");

    return 0;
}
