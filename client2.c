#include <stdio.h>

#include "datacollection.h"
#include "util.h"

enum output3_control_state { START = 0, LOW = 1, HIGH = 2 };

#define OUTPUT_3_THRESHOLD_VAL 3.0

void output3_control_callback(struct controlparameters *params, float value) {
  switch (params->state) {
    case START:
      // High state
      if (value >= OUTPUT_3_THRESHOLD_VAL) {
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_FREQUENCY, 1000);
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_AMPLITUDE, 8000);
        params->state = HIGH;
      } else {  // Low state
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_FREQUENCY, 2000);
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_AMPLITUDE, 4000);
        params->state = LOW;
      }
      break;
    case LOW:
      // Rising event
      if (value >= OUTPUT_3_THRESHOLD_VAL) {
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_FREQUENCY, 1000);
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_AMPLITUDE, 8000);
        params->state = HIGH;
      }
      break;
    case HIGH:
      // Falling event
      if (value < OUTPUT_3_THRESHOLD_VAL) {
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_FREQUENCY, 2000);
        ctrl_write_property(params->ctrl, 1, CTRL_PROP_AMPLITUDE, 4000);
        params->state = LOW;
      }
      break;
  }
}

int main(int argc, char **argv) {
  // Init controller
  struct controller ctrl = {
      .port = 4000,
      .fd = -1,
  };
  // TODO: error handling
  ctrl_init(&ctrl);

  // Define data connections
  struct controlparameters output3_controls = {
      .ctrl = &ctrl,
      .control_callback = output3_control_callback,
      .state = START,
  };
  struct dataconnection comms[] = {
      {.id = 1, .port = 4001, .fd = -1},
      {.id = 2, .port = 4002, .fd = -1},
      {.id = 3, .port = 4003, .fd = -1, .controls = &output3_controls},
  };

  // Start running datacollection
  dc_run_periodic_updates(comms, ARRAY_SIZE(comms), 20);

  // TODO: error handling
  ctrl_cleanup(&ctrl);
  return 0;
}
