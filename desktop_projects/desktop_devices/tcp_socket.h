#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_TCP_SOCKET_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_TCP_SOCKET_H_

#if defined(__WIN32__)
#include <winsock2.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#endif

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>  // NOLINT
#include <vector>

#include "circular_buffer.h"

class TcpSocket {
 public:
  explicit TcpSocket(int port);

  bool writeData(uint8_t* data, size_t len);

  bool readData();

  void setRXBuffer(CircularBuffer_s* rx_buffer);

 private:
  struct sockaddr_in address {};
  int addrlen = sizeof(address);

#if defined(__WIN32__)
  SOCKET server_fd;
#else
  int server_fd;
#endif
  volatile int client_fd;

  CircularBuffer_s* rxBuffer;

  std::thread acceptThread;

  uint8_t lastTxId = 0;
};

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_TCP_SOCKET_H_
