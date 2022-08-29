#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>  // NOLINT
#include <vector>

class TcpSocket {
 public:
  explicit TcpSocket(int port);

  bool writeData(uint8_t *data, size_t len);

 private:
  struct sockaddr_in address {};
  int addrlen = sizeof(address);

  int server_fd;
  volatile int client_fd;

  std::thread acceptThread;
};
