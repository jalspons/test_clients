#include "datacollection.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define NOREAD_VALUE "--"

static void conn_close(struct dataconnection *conn) {
  close(conn->fd);
  conn->fd = 0;
}

static int conn_open(struct dataconnection *conn) {
  // Don't connect, if socket already connected
  if (conn->fd > 0) {
    return 0;
  }

  int ret;
  struct sockaddr_in serv_addr = {0};

  if ((conn->fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(conn->port);

  if ((ret = connect(conn->fd, (struct sockaddr *)&serv_addr,
                     sizeof(serv_addr))) < 0) {
    conn_close(conn);
    return -1;
  }

  return 0;
}

static int conn_read(struct dataconnection *conn) {
  int ret;
  int total = 0;

  // Clear previous value
  memset(conn->value, 0, sizeof(conn->value));

  char buf[200] = {0};
  while ((ret = recv(conn->fd, &buf[total], sizeof(buf) - total,
                     MSG_DONTWAIT)) > 0) {
    // Try read all available data
    total += ret;
  }

  // ret == 0   -> Socket closed
  // ret == -1  -> error (allow EWOULDBLOCK)
  if (ret == 0 || (ret < 0 && errno != EWOULDBLOCK)) {
    return -1;
  }

  // Collect only most recent value
  char *sp, *ep;
  sp = buf;
  ep = strchr(sp, '\n');
  // Check if value exists
  if (ep) {
    char *ptr;
    // Find last value
    while ((ptr = strchr(&ep[1], '\n'))) {
      sp = ep;
      ep = ptr;
    }
    if (sp[0] == '\n') sp++;
    ep[0] = 0;
    // Store value
    strncpy(conn->value, sp, sizeof(conn->value) - 1);
  }

  return 0;
}

static unsigned long long get_timestamp_millis() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static void report_readings(struct dataconnection *connections, int connsize) {
  char jsonbuf[2048] = {0};
  jsonbuf[0] = '{';
  // Timestamp
  snprintf(&jsonbuf[strlen(jsonbuf)], sizeof(jsonbuf) - strlen(jsonbuf) - 1,
           "\"timestamp\":%llu,", get_timestamp_millis());
  // Output data
  for (int i = 0; i < connsize; i++) {
    snprintf(&jsonbuf[strlen(jsonbuf)], sizeof(jsonbuf) - strlen(jsonbuf) - 1,
             "\"out%d\":\"%s\",", connections[i].id,
             connections[i].value[0] ? connections[i].value : NOREAD_VALUE);
  }
  jsonbuf[strlen(jsonbuf) - 1] = '}';

  printf("%s\n", jsonbuf);
}

/// @brief Run datacollection routine
/// @param connections data connections to be used
/// @param connsize length of connections
/// @param periodms length of sampling period
void datacollect_run_periodic_updates(struct dataconnection *connections,
                                      int connsize, int periodms) {
  // Connect sockets
  for (int i = 0; i < connsize; i++) {
    // TODO: error handling
    (void)conn_open(&connections[i]);
  }
  while (1) {
    // Collect data samples
    usleep(periodms * 1000);
    // Read data from connections
    for (int i = 0; i < connsize; i++) {
      if (conn_read(&connections[i]) < 0) {
        // Error, close connection and reconnect
        conn_close(&connections[i]);
        conn_open(&connections[i]);
      }
    }
    report_readings(connections, connsize);
  }
  // Close connections
  for (int i = 0; i < connsize; i++) {
    conn_close(&connections[i]);
  }
}