#define _POSIX_C_SOURCE 199309L

#include "util.h"

#include <stddef.h>
#include <sys/time.h>
#include <time.h>

uint64_t get_timestamp_millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void sleep_ms(uint64_t millis) {
  struct timespec ts;
  ts.tv_sec = millis / 1000;
  ts.tv_nsec = (millis % 1000) * 1000000;
  nanosleep(&ts, NULL);
}
