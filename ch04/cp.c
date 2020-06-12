/* Exercise 4-2 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define THRESHOLD 2
typedef enum{false=0, true=1} bool;

static void usageErr(const char *cmd) {
    fprintf(stderr, "Usage: %s SOURCE DEST\n", cmd);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int fd1, fd2;
    ssize_t n;
    char buf[4096], buf2[4096] = "";
    struct stat filestat;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr(argv[0]);

    fd1 = open(argv[1], O_RDONLY);
    if (fd1 < 0) {
        perror(argv[1]);
        exit(EXIT_FAILURE);
    }
    
    fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd2 < 0) {
        perror(argv[2]);
        exit(EXIT_FAILURE);
    }

    if (fstat(fd1, &filestat)) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }
    
    if (filestat.st_blocks * 512 < filestat.st_size) { /* sparse file */
        int null_count = 0, data_count = 0;
        bool hole = false;

        printf("%s is a sparse file.\n", argv[1]);
        while ((n = read(fd1, buf, sizeof(buf))) > 0) {
            if (n < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            int i = 0, j = 0;
            for (; i < n; ++i) {
                if (buf[i]) {       /* Non-zero */
                    if (hole) {
                        hole = false;
                        lseek(fd2, null_count, SEEK_CUR);
                        null_count = 0;
                    }
                    if (null_count) {
                        data_count += null_count;
                        null_count = 0;
                    }
                    ++data_count;
                } else {           /* zero */
                    if (!hole && null_count > THRESHOLD) {  /*If the number of zeros exceeds the threshold,
                                                              then it is assumed to be a hole*/
                        hole = true;
                        if (data_count) {
                            write(fd2, buf+i-1-THRESHOLD-data_count, data_count);
                            data_count = 0;
                        }
                    }
                    ++null_count;
                }
            }
            if (data_count) {
                write(fd2, buf+n-data_count, data_count);
                data_count = 0;
            }
        }
    } else {
        printf("%s is not a sparse file.\n", argv[1]);
        while ((n = read(fd1, buf, sizeof(buf))) > 0) {
            if (n < 0) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (write(fd2, buf, n) < 0) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }
    }

    close(fd1);
    close(fd2);

    return 0;
}
