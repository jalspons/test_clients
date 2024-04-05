#ifndef DATACOLLECTION_H_
#define DATACOLLECTION_H_

#include <stdint.h>

struct dataconnection {
  //-------
  // Config
  //-------
  // Unique id
  uint8_t id;
  // Connection port
  short port;
  //-------------
  // Runtime data
  //-------------
  int fd;
  // Latest value
  char value[20];
};

void datacollect_run_periodic_updates(struct dataconnection *connections,
                                      int connsize, int periodms);

#endif  // DATACOLLECTION_H_
