/* Exercise 4-1 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static void usageErr(const char *cmd) {
    fprintf(stderr, "Usage: %s [-a] filename\n", cmd);
    exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
    int opt, flag;
    char filename[NAME_MAX+1] = "";

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
        usageErr(argv[0]);

    opterr = 0;    /* getopt() does not print an error message */
    if ((opt = getopt(argc, argv, ":a:")) != -1) {
        switch (opt) {
        case 'a':
            if (optind == 2 && argc == 3) { /* to prevent user from inputing the error case like './tee -afile1 file2' */
                fprintf(stderr, "Did you mean\n %s -a%s or\n %s -a %s\n", argv[0], optarg, argv[0], argv[2]);
                exit(EXIT_FAILURE);
            }
            flag = O_APPEND;
            strcpy(filename, optarg);
            break;
        case '?':
            fprintf(stderr, "Unknown option: -%c\n", (char)optopt);
            usageErr(argv[0]);
        case ':':
            fprintf(stderr, "Missing option argument\n");
            usageErr(argv[0]);
        }
    } else { /* don't specify -a option */
        if (argc == 3) /* too many arguments */
            usageErr(argv[0]);
        flag = O_TRUNC;
        strcpy(filename, argv[optind]);
    }

    int fd;
    ssize_t n;
    char buf[4096];
    fd = open(filename, O_WRONLY | O_CREAT | flag, 0644);
    if (fd < 0) {
        perror(filename);
        exit(EXIT_FAILURE);
    }

    while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (n < 0) {
            perror("read stdin");
            exit(EXIT_FAILURE);
        }
        if (write(STDOUT_FILENO, buf, n) < 0) {
            perror("write stdout");
            exit(EXIT_FAILURE);
        }
        if (write(fd, buf, n) < 0) {
            perror("write file");
            exit(EXIT_FAILURE);
        }
    }

    close(fd);

    return 0;
}
