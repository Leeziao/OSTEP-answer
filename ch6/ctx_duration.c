#define _GNU_SOURCE

#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sched.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOOP    20
#define USEC2SEC        1000000
#define BYTESENT        0 

void Gettimeofday(struct timeval *tv, struct timezone *tz)
{
    int reval = gettimeofday(tv, tz);
    if (0 != reval)
    {
        printf("Error occur\n");
        exit(1);
    }
}

int Read(int fd, char *buf, int count)
{
    int reval = read(fd, buf, count);
    if (reval < 0)
    {
        printf("Read Error\n");
        exit(1);
    }
    return reval;
}

int Write(int fd, const char *buf, int count)
{
    int reval = write(fd, buf, count);
    if (reval < 0)
    {
        printf("Write Error\n");
        exit(1);
    }
    return reval;
}

int main(int argc, char **argv)
{
    struct timeval old_tv, new_tv;
    int i, LOOP, fd_one2two[2], fd_two2one[2];
    char buf = 'a';
    cpu_set_t set;

    CPU_ZERO(&set);
    CPU_SET(0, &set);

    LOOP = (argc == 1) ? (DEFAULT_LOOP) : (atoi(argv[1]));
    pipe(fd_one2two);
    pipe(fd_two2one);

    printf("fd: [%d]->[%d]\n", fd_one2two[1], fd_one2two[0]);
    printf("fd: [%d]->[%d]\n", fd_two2one[1], fd_two2one[0]);

    if (0 == fork())
    {
        // Child
        sched_setaffinity(0, sizeof(cpu_set_t), &set);
        close(fd_one2two[1]);
        close(fd_two2one[0]);

        for (i = 0; i < LOOP; i++)
        {
            Read(fd_one2two[0], &buf, BYTESENT);
            Write(fd_two2one[1], &buf, BYTESENT);
        }
        exit(0);
    }

    sched_setaffinity(0, sizeof(cpu_set_t), &set);
    close(fd_one2two[0]);
    close(fd_two2one[1]);
    sleep(1);

    Gettimeofday(&old_tv, NULL);
    for (i = 0; i < LOOP; i++)
    {
        Write(fd_one2two[1], &buf, BYTESENT);
        Read(fd_two2one[0], &buf, BYTESENT);
    }
    Gettimeofday(&new_tv, NULL);

    printf("Before: sec=%lx, usec=%lx\n", old_tv.tv_sec, old_tv.tv_usec);
    printf("After:  sec=%lx, usec=%lx\n", new_tv.tv_sec, new_tv.tv_usec);
    printf("LOOP=%10d, usec=%10ld, ONE=%10lf usec\n", LOOP, USEC2SEC * new_tv.tv_sec + new_tv.tv_usec - old_tv.tv_usec - USEC2SEC * old_tv.tv_sec,
           (double)(USEC2SEC * new_tv.tv_sec + new_tv.tv_usec - old_tv.tv_usec - USEC2SEC * old_tv.tv_sec) / (double)(LOOP));
    printf("Total Context Switch=%10d, ONE=%10lf usec\n", LOOP*2,
           (double)(USEC2SEC * new_tv.tv_sec + new_tv.tv_usec - old_tv.tv_usec - USEC2SEC * old_tv.tv_sec) / (double)(LOOP * 2));

    return 0;
}
