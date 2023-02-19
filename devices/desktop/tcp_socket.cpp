//
// Created by matth on 3/27/2022.
//

#include "tcp_socket.h"

#include "data_structures.h"

#define BUFFER_LEN 100

TcpSocket::TcpSocket(int port) {
  int new_socket;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  this->client_fd = new_socket;

  // Set receive timeout
  struct timeval tv = {0, 10000};
  setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

bool TcpSocket::writeData(uint8_t *data, size_t len) {
  if (this && client_fd) {
    send(client_fd, data, len, 0);
  }
  return true;
}

bool TcpSocket::readData() {
  char buffer[BUFFER_LEN];
  memset(buffer, 0, BUFFER_LEN);

  int bytes_received = recv(client_fd, buffer, BUFFER_LEN, 0);

  if (bytes_received > 0) {
    printf("Received Radio Message [");
    fwrite(buffer, 1, bytes_received, stdout);
    printf("]\n");

    static RadioRecievedPacket_s packet;
    packet.radioId = 0;
    packet.rssi = 0;
    packet.crc = true;
    packet.lqi = 0;
    memset(packet.data, 0, sizeof(packet.data));
    memcpy(packet.data, buffer, bytes_received);

    cb_enqueue(rxBuffer, &packet);
  }

  return true;
}

void TcpSocket::setRXBuffer(CircularBuffer_s *rx_buffer) {
  rxBuffer = rx_buffer;
}
