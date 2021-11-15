# Mechanism: Limited Direct Execution

1. Use `gettimeofday()` to measure syscall duration [code](syscall_duration.c).

```bash
$ ./syscall_duration.out
Before: sec=6191c599, usec=79e2e
After:  sec=6191c599, usec=7a4bd
LOOP=     10000, usec=      1679, ONE=  0.167900 usec
```

2. Use `rdtsc` to measure syscall duration [code](syscall_duration2.c).

> https://en.wikipedia.org/wiki/Time_Stamp_Counter

```bash
$ ./syscall_duration2.out 10000
Before: ticks=e284fa576f6
After:  ticks=e284fe171fc
LOOP=     10000, total ticks=   3930886, ONE=393.088600 ticks
```

Run `cat /proc/cpuinfo | grep "cpu MHz" | sort | uniq` and the frequency of cpu is 2370 MHz. Hence one syscall spends
$$\frac{393}{2370\times 10^6}\times 10^{-6}=0.1658~\mu s$$
which is very similar to the first method.

3. Measure context switch duration [code](ctx_duration.c).

> http://lmbench.sourceforge.net/
>
> https://man7.org/linux/man-pages/man2/sched_setaffinity.2.html

One pipe operation loop of write and read requires 2 context switch, hence the result needs to be devided by 2.

```bash
$ ./ctx_duration.out 10000
fd: [4]->[3]
fd: [6]->[5]
Before: sec=61920541, usec=e42ab
After:  sec=61920541, usec=e54b5
LOOP=     10000, usec=      4618, ONE=  0.461800 usec
Total Context Switch=     20000, ONE=  0.230900 usec
```
