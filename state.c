#include "state.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static struct program_state state = {0};

static void signal_handler(int signal) {
  switch (signal) {
    case SIGINT:
    case SIGTERM:
      state.running = 0;
      break;
  }
}

void state_init(void) {
  state.running = 1;

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
}

void state_cleanup(void) { state.running = 0; }

struct program_state *state_get(void) { return &state; }
