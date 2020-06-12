/* Exercise 5-3 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);\
                          } while (0)


int main(int argc, char *argv[])
{
    int fd, flag;
    long val;
    char *endptr, *str;
    off_t off;

    if (argc < 3 || argc > 4 || strcmp(argv[1], "--help") == 0) {
        fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    flag = O_RDWR | O_CREAT;
    if (argc == 3)
        flag |= O_APPEND;

    fd = open(argv[1], flag, 0644);
    if (fd == -1)
        errExit("open");

    /* convert argv[2] to a long integer */
    str = argv[2];
    errno = 0;
    val = strtol(str, &endptr, 10);

    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
            || (errno != 0 && val == 0)) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endptr == str) {
        fprintf(stderr, "No digits were found\n");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < val; ++i) {
        if (argc == 4)    /* a third command-line argument (x) is supplied */
            if (lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");
        if (write(fd, "x", 1) < 0)
            errExit("write");
    }

    return 0;
}
