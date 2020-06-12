/* Exercise 5-6 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE);\
                          } while (0)

int main()
{
    int fd1, fd2, fd3;
    char file[] = "tfile";

    fd1 = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); /* rw- --- --- */
    fd2 = dup(fd1);
    fd3 = open(file, O_RDWR);

    if (write(fd1, "Hello,", 6) == -1)
        errExit("write");
    if (write(fd2, "world", 6) == -1)  /* at the end there is a 0 */
        errExit("write");
    if (lseek(fd2, 0, SEEK_SET) == -1)
        errExit("lseek");
    if (write(fd1, "HELLO,", 6) == -1)
        errExit("write");
    if (write(fd3, "Gidday", 6) == -1)
        errExit("write");

    if (close(fd1) == -1)
        errExit("close");
    if (close(fd2) == -1)
        errExit("close");
    if (close(fd3) == -1)
        errExit("close");

    return 0;
}
