#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <stdint.h>

struct controller {
  short port;
  int fd;
};

enum ctrl_property_ids {
  CTRL_PROP_AMPLITUDE = 0xaa,
  CTRL_PROP_FREQUENCY = 0xff,
};

/// @brief Initialize controller
/// @param ctrl controller data structure
/// @return 0 on success
int ctrl_init(struct controller *ctrl);

/// @brief Clean controller
/// @param ctrl controller data structure
/// @return 0 on success
int ctrl_cleanup(struct controller *ctrl);

/// @brief Read output property value
/// @param ctrl controller data structure
/// @param object output object id
/// @param property output property id
void ctrl_read_property(struct controller *ctrl, uint16_t object,
                        uint16_t property);

/// @brief Write new output property value
/// @param ctrl controller data structure
/// @param object output object id
/// @param property output property id
/// @param value new value
void ctrl_write_property(struct controller *ctrl, uint16_t object,
                         uint16_t property, uint16_t value);

#endif  // CONTROLLER_H_
