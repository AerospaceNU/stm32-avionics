//
// Created by matth on 3/25/2022.
//

#include "file_backed_flash.h"

#include <sys/stat.h>

#include <cstring>

bool FileBackedFlash::writeStart(uint32_t startLoc, uint32_t numBytes,
                                 uint8_t *pdata) {
  FILE *ptr;
  ptr = fopen(filepath.c_str(), "r+b");  // w for write, b for binary
  if (!ptr) return false;

  // Seek to the start location and write
  // TODO should we simulate flash erase?
  fseek(ptr, startLoc, SEEK_SET);
  fwrite(pdata, numBytes, 1, ptr);

  // clean up
  fclose(ptr);

  return true;
}

bool FileBackedFlash::readStart(uint32_t startLoc, uint32_t numBytes,
                                uint8_t *pdata) {
  FILE *ptr;
  ptr = fopen(filepath.c_str(), "rb");  // r for read, b for binary
  if (!ptr) return false;

  // Seek to the start location and read
  fseek(ptr, startLoc, SEEK_SET);
  fread(pdata, numBytes, 1, ptr);

  // clean up
  fclose(ptr);

  return true;
}

bool fileExists(const std::string &filename) {
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1) {
    return true;
  }
  return false;
}

void FileBackedFlash::reinit(bool overwrite) {
  if (fileExists(filepath) && !overwrite) {
    return;
  }

  auto len = m_len;

  uint8_t ff[4096];
  memset(ff, 0xff, 4096);

  std::ofstream ofs;
  ofs.open(filepath, std::ios::out | std::ios::binary);
  while (len > sizeof(ff)) {
    size_t bytes_to_write = std::min(sizeof(ff), len);
    ofs.write((char *)ff, bytes_to_write);
    len -= bytes_to_write;
  }
  ofs.write((char *)ff, len);
}
