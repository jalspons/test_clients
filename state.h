#ifndef STATE_H_
#define STATE_H_

#include <stdatomic.h>

struct program_state {
  atomic_bool running;
};

// Prepare state for the program
void state_init(void);
// Clean program state
void state_cleanup(void);
// Get pointer to state structure
struct program_state *state_get(void);

#endif  // STATE_H_
