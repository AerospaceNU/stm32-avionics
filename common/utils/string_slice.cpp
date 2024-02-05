#include "string_slice.h"

#include <string.h>

StringSlice::StringSlice(const char** src, int start, int end) {
  this->src = src;
  this->start = start;
  this->end = end;
  this->len = end - start;
}

StringSlice::StringSlice() {
  this->src = nullptr;
  this->start = 0;
  this->end = 0;
  this->len = 0;
}

bool StringSlice::operator==(const char* other) const {
  if (strnlen(other, this->len + 1) != static_cast<unsigned int>(this->len)) {
    return false;
  }
  return strncmp(other, startPtr(), this->len) == 0;
}

const char** StringSlice::basePtr() const { return this->src; }

const char* StringSlice::startPtr() const { return *(this->src) + this->start; }

int StringSlice::length() const { return this->len; }

char StringSlice::operator[](int index) const {
  if (index >= length()) {
    return '\0';
  }
  const char* src = *(this->src);
  return src[start + index];
}

StringSlice StringSlice::inner() const {
  return StringSlice(this->src, start + 1, end - 1);
}

const char* StringSlice::endptr() const {
  return *(this->src) + length() + this->start;
}

void StringSlice::copy(char* buffer, int n) const {
  for (int i = 0; i < this->length() && i < n; ++i) {
    buffer[i] = this->operator[](i);
  }
}

int StringSlice::split(StringSlice* slices, int sliceCount, const char delim) {
  int sliceIndex = 0;
  int start = 0;
  int current = 0;
  int nestDepth = 0;
  int n = this->length();
  while (sliceIndex < sliceCount) {
    while (current < n) {
      if (nestDepth == 0 && this->operator[](current) == delim) {
        break;
      }
      if (this->operator[](current) == '(') {
        nestDepth++;
      }
      if (this->operator[](current) == ')') {
        nestDepth--;
        if (nestDepth < 0) {
          return -1;
        }
      }
      current++;
    }
    if (current == n) {
      if (start < current) {
        slices[sliceIndex++] = StringSlice(this->basePtr(), this->start + start,
                                           this->start + current);
      }
      break;
    }
    slices[sliceIndex++] = StringSlice(this->basePtr(), this->start + start,
                                       this->start + current);
    start = ++current;
  }
  return sliceIndex;
}
