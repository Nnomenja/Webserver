#include <iostream>
#include <ctime>
#include <stdint.h>

#include "./utils.hpp"  

uint64_t get_time_ms(void) 
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ((uint64_t)ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

bool verify_deadline_ms(const uint64_t &start_time_ms,const uint64_t timeout_ms) 
{
    uint64_t now_ms = get_time_ms();
    return ((now_ms - start_time_ms) > timeout_ms);
}
