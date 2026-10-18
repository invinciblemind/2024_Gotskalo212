#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#define N 1024

int main(int argc, char **argv)
{
    char buf[N];
    int fd1, fd2;
    ssize_t n;
    struct stat s;
    if (strcmp(argv[1], argv[2]) == 0)
        return 0;
    if ((fd1=open(argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "ошибка при открытии файла\n");
        return 1;
    }
    stat(argv[1], &s);
    if ((fd2=creat(argv[2], s.st_mode)) == -1) {
        fprintf(stderr, "ошибка при создании файла\n");
        return 1;
    }
    while ((n=read(fd1, buf, N)) > 0)
        write(fd2, buf, n);
    return 0;
}
