/* Exercise 5-2 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);\
                          } while (0)


int main(int argc, char *argv[])
{
    int fd;
    off_t off;

    if (argc != 2 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    fd = open(argv[1], O_RDWR | O_APPEND);
    if (fd == -1)
        errExit("open");

    if (lseek(fd, 0, SEEK_SET) == -1) /* seeks to the beginning of the file */
        errExit("lseek");

    if (write(fd, "test", 4) == -1)
        errExit("write");

    if ((off = lseek(fd, 0, SEEK_CUR)) == -1)
        errExit("lseek");

    printf("offset is %d\n", off);
    
    if (off > 4)
        printf("The previous lseek operation did not take effect.\n"
                "If the O_APPEND file status flag is set on the open file description,\n"
                "then a write() always moves the file offset to the end of the file,\n"
                "regardless of the use of lseek().\n");

    return 0;
}
