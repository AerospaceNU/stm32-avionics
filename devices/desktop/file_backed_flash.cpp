//
// Created by matth on 3/25/2022.
//

#include "file_backed_flash.h"

#include <sys/stat.h>

#include <cstring>

bool FileBackedFlash::writeStart(uint32_t startLoc, uint32_t numBytes,
                                 uint8_t *pdata) {
  // Open a new output stream in binary mode
  std::fstream ofs{filepath};
  // and check it worked
  if (!ofs.good()) return false;

  // Seek to the position and write the data
  if (!ofs.seekp(startLoc, std::ios_base::beg)) return false;
  if (!ofs.write((char *)pdata, numBytes)) return false;

  ofs.close();
  return true;
}

bool FileBackedFlash::readStart(uint32_t startLoc, uint32_t numBytes,
                                uint8_t *pdata) {
  // Open a new input stream in binary mode
  std::ifstream ifs;
  ifs.open(filepath, std::ios::in | std::ios::binary);
  // and check it worked
  if (!ifs.good()) return false;

  // Seek to the position and read the bytes
  if (!ifs.seekg(startLoc)) return false;
  if (!ifs.read((char *)pdata, numBytes)) return false;

  ifs.close();
  return true;
}

bool fileExists(const std::string &filename) {
  struct stat buf;
  if (stat(filename.c_str(), &buf) != -1) {
    return true;
  }
  return false;
}

void FileBackedFlash::reinit() {
  if (fileExists(filepath)) {
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
