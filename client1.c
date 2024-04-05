#include "datacollection.h"
#include "util.h"

int main(int argc, char **argv) {
  // Define data connections
  struct dataconnection comms[] = {{.id = 1, .port = 4001},
                                   {.id = 2, .port = 4002},
                                   {.id = 3, .port = 4003}};
  // Start running datacollection
  datacollect_run_periodic_updates(comms, ARRAY_SIZE(comms), 100);
  return 0;
}