#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define DEFAULT_LOOP 20

typedef union Mytime
{
    struct
    {
        uint32_t low;
        uint32_t high;
    };
    uint64_t val;
} Mytime;

void get_timestamp(Mytime *mytime)
{
    asm volatile("rdtsc"
                 : "=a"(mytime->low), "=d"(mytime->high)::);
}

int main(int argc, char **argv)
{
    Mytime old_time, new_time;
    int retval, i, LOOP;
    char buf;

    LOOP = (argc == 1) ? (DEFAULT_LOOP) : (atoi(argv[1]));

    int fd = open(argv[0], O_RDONLY);
    read(fd, &buf, 0);

    get_timestamp(&old_time);

    for (i = 0; i < LOOP; i++)
    {
        read(fd, &buf, 0);
    }

    get_timestamp(&new_time);

    printf("Before: ticks=%lx\n", old_time.val);
    printf("After:  ticks=%lx\n", new_time.val);
    printf("LOOP=%10d, total ticks=%10ld, ONE=%10lf ticks\n", LOOP, new_time.val - old_time.val,
           (double)(new_time.val - old_time.val) / (double)(LOOP));

    return 0;
}
