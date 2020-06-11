#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


static void usageErr(const char *cmd) {
    fprintf(stderr, "Usage: %s sparsefile\n", cmd);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int fd, curr = 0;
    int readnum = 0;
    char buf[4096] = "";
    struct stat filestat;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr(argv[0]);

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }

    if (fstat(fd, &filestat)) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    curr = lseek(fd, 0, SEEK_CUR);
    printf("current offset is %d\n", curr);

    curr = lseek(fd, 0, SEEK_HOLE);
    printf("after seek the hole(0), current offset is %d\n\n", curr);

    curr = lseek(fd, 50, SEEK_HOLE);
    printf("after seek the hole(50), current offset is %d\n\n", curr);

    readnum = read(fd, buf, sizeof(buf));
    printf("read %d bytes, buf is %s\n", readnum, buf);

    curr = lseek(fd, 0, SEEK_DATA);
    printf("after seek the data(0), current offset is %d\n\n", curr);

    curr = lseek(fd, 50, SEEK_DATA);
    printf("after seek the data(50), current offset is %d\n\n", curr);

    curr = lseek(fd, 8190, SEEK_DATA);
    printf("after seek the data(8190), current offset is %d\n\n", curr);

    curr = lseek(fd, 8194, SEEK_DATA);
    printf("after seek the data(8194), current offset is %d\n\n", curr);

    curr = lseek(fd, 10001, SEEK_DATA);
    printf("after seek the data(10001), current offset is %d\n\n", curr);

    readnum = read(fd, buf, sizeof(buf));
    printf("read %d bytes, buf is %s\n", readnum, buf);
    
//    printf("dont't support Seeking file data and holes\n");
    printf("Seeking file data and holes doesn't work properly\n");
    close(fd);

    return 0;
}
