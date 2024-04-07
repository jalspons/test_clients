#include <stdio.h>

#include "datacollection.h"
#include "state.h"
#include "util.h"

int main(int argc, char **argv) {
  // Initialize state for program
  state_init();

  // Define data connections
  struct dataconnection comms[] = {{.id = 1, .port = 4001, .fd = -1},
                                   {.id = 2, .port = 4002, .fd = -1},
                                   {.id = 3, .port = 4003, .fd = -1}};
  // Start running datacollection
  dc_run_periodic_updates(comms, ARRAY_SIZE(comms), 100);

  // Cleaning
  state_cleanup();
  return 0;
}
