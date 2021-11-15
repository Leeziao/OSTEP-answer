/**
 * Note: 1 microsecond = 10-6 second
 */
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOOP 20
#define USEC2SEC 1000000

int main(int argc, char **argv)
{
    struct timeval old_tv, new_tv;
    int retval, i, LOOP;
    char buf;

    LOOP = (argc == 1) ? (DEFAULT_LOOP) : (atoi(argv[1]));

    int fd = open(argv[0], O_RDONLY);
    read(fd, &buf, 0);

    retval = gettimeofday(&old_tv, NULL);
    if (retval != 0)
    {
        printf("Error occur\n");
        exit(1);
    }

    for (i = 0; i < LOOP; i++)
    {
        read(fd, &buf, 0);
    }

    retval = gettimeofday(&new_tv, NULL);
    if (retval != 0)
    {
        printf("Error occur\n");
        exit(1);
    }

    printf("Before: sec=%lx, usec=%lx\n", old_tv.tv_sec, old_tv.tv_usec);
    printf("After:  sec=%lx, usec=%lx\n", new_tv.tv_sec, new_tv.tv_usec);
    printf("LOOP=%10d, usec=%10ld, ONE=%10lf usec\n", LOOP, USEC2SEC * new_tv.tv_sec + new_tv.tv_usec - old_tv.tv_usec - USEC2SEC * old_tv.tv_sec,
           (double)(USEC2SEC * new_tv.tv_sec + new_tv.tv_usec - old_tv.tv_usec - USEC2SEC * old_tv.tv_sec) / (double)(LOOP));

    return 0;
}
