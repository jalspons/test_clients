#ifndef DATACOLLECTION_H_
#define DATACOLLECTION_H_

#include <stdint.h>

#include "controller.h"

struct controlparameters {
  // controller object
  struct controller *ctrl;
  // current state of control
  // Control logic defines states
  uint8_t state;
  // Control logic method
  void (*control_callback)(struct controlparameters *params, float value);
};

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
  // Control interface to react for value changes
  struct controlparameters *controls;
};

/// @brief Run data collection routine for collecting data from outputs.
/// @param connections Data connection structures
/// @param connsize Number of data connections
/// @param periodms Sampling period length in milliseconds
void dc_run_periodic_updates(struct dataconnection *connections, int connsize,
                             int periodms);

#endif  // DATACOLLECTION_H_
