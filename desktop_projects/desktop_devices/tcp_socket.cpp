//
// Created by matth on 3/27/2022.
//

#include "tcp_socket.h"

#include <cmath>
#include <cstddef>

#include "data_structures.h"
#include "tasks/radio_packet_types.h"

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
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
                 reinterpret_cast<char *>(&opt), sizeof(opt))) {
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

#if defined(__WIN32__) && !defined(socketlen_t)
#define socklen_t int
#endif
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                           (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  this->client_fd = new_socket;

  // Set receive timeout
  struct timeval tv = {0, 10000};
  setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char *>(&tv),
             sizeof(tv));
}

bool TcpSocket::writeData(uint8_t *data, size_t len) {
  if (this && client_fd) {
    send(client_fd, (char *)data, len, 0);
  }
  return true;
}

bool TcpSocket::readData() {
  char buffer[512];
  memset(buffer, 0, sizeof(buffer));

  int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);

  if (bytes_received > 0 && buffer[0] == TELEMETRY_ID_STRING) {
    RadioRecievedPacket_s packet;
    packet.radioId = 0;
    packet.rssi = 0;
    packet.crcFromRadio = true;
    packet.lqi = 0;

    void *stringPacket = buffer + offsetof(RadioPacket_s, payload);
    uint8_t len = *((uint8_t *)stringPacket + offsetof(CliStringPacket_s, len));
    uint8_t *fullString =
        (uint8_t *)stringPacket + offsetof(CliStringPacket_s, string);

    RadioPacket_s packetOnAir = {0};
    packetOnAir.packetType = buffer[0];

    // Copy in 48 byte chunks (ew)
    int txlen;
    int offset = 0;
    do {
      txlen = std::min<uint8_t>(len, RADIO_MAX_STRING);
      memcpy(packetOnAir.payload.cliString.string, fullString + offset, txlen);
      packetOnAir.payload.cliString.len = txlen;
      offset += txlen;
      len -= txlen;

      // Force tx-id to increment every time
      packetOnAir.payload.cliString.id = lastTxId++;

      memcpy(packet.data, &packetOnAir, sizeof(packetOnAir));

      cb_enqueue(rxBuffer, (unknownPtr_t)&packet);
    } while (len);
  }

  return true;
}

void TcpSocket::setRXBuffer(CircularBuffer_s *rx_buffer) {
  rxBuffer = rx_buffer;
}
