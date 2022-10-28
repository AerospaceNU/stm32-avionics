//
// Created by matth on 3/25/2022.
//

#include "file_backed_flash.h"

#include <sys/stat.h>

#include <cstring>

bool FileBackedFlash::WriteStart(uint32_t startLoc, uint32_t numBytes,
                                 uint8_t *pdata) {
  FILE *ptr;
  ptr = fopen(filepath.c_str(),"wb");  // w for write, b for binary
  if (!ptr) return false;

  // Seek to the start location and write
  // TODO should we simulate flash erase?
  fseek(ptr, startLoc, SEEK_SET);
  fwrite(pdata, numBytes, 1, ptr);

  // clean up
  fclose(ptr);

  return true;
}

bool FileBackedFlash::ReadStart(uint32_t startLoc, uint32_t numBytes,
                                uint8_t *pdata) {
  FILE *ptr;
  ptr = fopen(filepath.c_str(),"rb");  // r for read, b for binary
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

void FileBackedFlash::Reinit(bool overwrite) {
  if (fileExists(filepath) && !overwrite) {
    return;
  }

  auto len = m_len;

  uint8_t ff[4096];
  memset(ff, 0xff, 4096);

  std::ofstream ofs;
  ofs.open(filepath, std::ios::out | std::ios::binary);
  while (len > sizeof(ff)) {
    ofs.write((char *)ff, sizeof(ff));
    len -= sizeof(ff);
  }
  ofs.write((char *)ff, len);
}
