#include "controller.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

static void ctrl_close(struct controller *ctrl) {
  if (ctrl == NULL) return;

  close(ctrl->fd);
  ctrl->fd = -1;
}

static int ctrl_open(struct controller *ctrl) {
  if (ctrl == NULL) return 0;

  // Don't connect, if socket already connected
  if (ctrl->fd > 0) {
    return 0;
  }

  int ret;
  struct sockaddr_in serv_addr = {0};

  if ((ctrl->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(ctrl->port);

  if ((ret = connect(ctrl->fd, (struct sockaddr *)&serv_addr,
                     sizeof(serv_addr))) < 0) {
    ctrl_close(ctrl);
    return -1;
  }

  return 0;
}

static void ctrl_write_data(struct controller *ctrl, void *data, int len) {
  write(ctrl->fd, data, len);
}

int ctrl_init(struct controller *ctrl) {
  ctrl_open(ctrl);
  return 0;
}

int ctrl_cleanup(struct controller *ctrl) {
  ctrl_close(ctrl);
  return 0;
}

enum ctrl_operation { CTRL_OP_READ = 1, CTRL_OP_WRITE = 2 };

struct __attribute__((packed)) ctrl_read_message {
  uint16_t operation;
  uint16_t object;
  uint16_t property;
};

void ctrl_read_property(struct controller *ctrl, uint16_t object,
                        uint16_t property) {
  struct ctrl_read_message msg = {
      .operation = htons(CTRL_OP_READ),
      .object = htons(object),
      .property = htons(property),
  };
  ctrl_write_data(ctrl, (void *)&msg, sizeof(msg));
}

struct __attribute__((packed)) ctrl_write_message {
  uint16_t operation;
  uint16_t object;
  uint16_t property;
  uint16_t value;
};

void ctrl_write_property(struct controller *ctrl, uint16_t object,
                         uint16_t property, uint16_t value) {
  struct ctrl_write_message msg = {
      .operation = htons(CTRL_OP_WRITE),
      .property = htons(property),
      .object = htons(object),
      .value = htons(value),
  };
  ctrl_write_data(ctrl, (void *)&msg, sizeof(msg));
}
