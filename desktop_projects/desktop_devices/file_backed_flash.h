//
// Created by matth on 3/25/2022.
//

#ifndef DESKTOP_PROJECTS_DESKTOP_DEVICES_FILE_BACKED_FLASH_H_
#define DESKTOP_PROJECTS_DESKTOP_DEVICES_FILE_BACKED_FLASH_H_

#include <cstdint>
#include <fstream>
#include <string>
#include <utility>

class FileBackedFlash {
 public:
  explicit FileBackedFlash(std::string path, size_t len)
      : filepath(std::move(path)), m_len(len) {
    reinit(false);
  }

  void reinit(bool overwrite);
  bool readStart(uint32_t startLoc, uint32_t numBytes, uint8_t* pdata);
  bool writeStart(uint32_t startLoc, uint32_t numBytes, uint8_t* pdata);

 private:
  std::string filepath;
  size_t m_len;
};

#endif  // DESKTOP_PROJECTS_DESKTOP_DEVICES_FILE_BACKED_FLASH_H_
