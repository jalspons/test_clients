#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(*arr)

// Get timestamp of milliseconds since epoch
uint64_t get_timestamp_millis(void);

// Pause execution for `millis` milliseconds time
void sleep_ms(uint64_t millis);

#endif  // UTIL_H_
