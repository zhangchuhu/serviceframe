#include "TimeMeasure.h"
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#define rdtsc(low,high) __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))


namespace
{
    double cpu_freq = 1; /*MHz*/
    double cpu_freq_magnification = 1; /*MHz*/
}

namespace server
{
namespace xlinux
{
uint64_t get_cpu_cycle()
{
    union cpu_cycle
    {
            struct t_i32
            {
                    uint32_t l;
                    uint32_t h;
            } i32;
            uint64_t t;
    } c;

    rdtsc( c.i32.l, c.i32.h );

    return c.t;
}

uint64_t get_usec_interval(uint64_t start, uint64_t stop)
{
    if (stop < start)
        return 0;
    return (uint64_t) ((stop - start) / cpu_freq);
}

uint32_t get_msec_interval(uint64_t start, uint64_t stop)
{
    if (stop < start)
        return 0;
    return (uint32_t) ((stop - start) / cpu_freq_magnification);
}

uint64_t get_tick_interval(uint64_t start, uint64_t stop)
{
    if (stop < start)
        return 0;
    return (stop - start);
}

int init_cpu_freq()
{
    FILE * fp;
    char * str;
    const char * cmd;
    int ratio = 1;

    str = (char*) malloc(1024);

    fp = popen("cat /proc/cpuinfo | grep -m 1 \"model name\"", "r");
    fgets(str, 1024, fp);
    fclose(fp);

    if (strstr(str, "AMD"))
    {
        cmd
                = "cat /proc/cpuinfo | grep -m 1 \"cpu MHz\" | sed -e \'s/.*:[^0-9]//\'";
    }
    else
    {
        cmd
                = "cat /proc/cpuinfo | grep -m 1 \"model name\" | sed -e \"s/^.*@ //g\" -e \"s/GHz//g\"";
        ratio = 1000;
    }

    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        return -1;
    }
    else
    {
        fgets(str, 1024, fp);
        cpu_freq = atof(str) * ratio;
        cpu_freq_magnification = cpu_freq * 1000;
        fclose(fp);
    }

    free(str);

    return 0;
}

}
}
