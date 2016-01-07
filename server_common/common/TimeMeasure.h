#ifndef TIMEMEASURE_H
#define TIMEMEASURE_H
#include <stdint.h>

namespace server {
namespace xlinux {

int init_cpu_freq();
uint64_t get_cpu_cycle();
uint64_t get_usec_interval(uint64_t start, uint64_t stop);
uint32_t get_msec_interval(uint64_t start, uint64_t stop);
uint64_t get_tick_interval(uint64_t start, uint64_t stop);

}
}
#endif
