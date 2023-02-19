#include "string_slice.h"

#include <string.h>

StringSlice::StringSlice(const char **src, int start, int end) {
    this->src = src;
    this->start = start;
    this->end = end;
    this->len = end - start;
}

bool StringSlice::operator==(const char *other) {
    if (strlen(other) != (unsigned int)this->len) {
        return false;
    }
    return strncmp(other, startPtr(), this->len) == 0;
}

const char **StringSlice::basePtr() {
    return this->src;
}

const char *StringSlice::startPtr() {
    return *(this->src) + this->start;
}

int StringSlice::length() {
    return this->len;
}

char StringSlice::operator[](int index) {
    if (index >= length()) {
        return '\0';
    }
    const char *src = *(this->src);
    return src[start + index];
}

StringSlice StringSlice::inner() {
    return StringSlice(this->src, start + 1, end - 1);
}


const char *StringSlice::endptr() {
    return *(this->src) + length() + this->start;
}

void StringSlice::copy(char *buffer, int n) {
    for (int i = 0; i < this->length() && i < n; ++i) {
        buffer[i] = this->operator[](i);
    }
}


int StringSlice::split(StringSlice &source, StringSlice *slices, int sliceCount, char delim) {
    int sliceIndex = 0;
    int start = 0;
    int current = 0;
    int nestDepth = 0;
    int n = source.length();
    while (sliceIndex < sliceCount) {
        while (current < n) {
            if (nestDepth == 0 && source[current] == delim) {
                break;
            }
            if (source[current] == '(') {
                nestDepth++;
            }
            if (source[current] == ')') {
                nestDepth--;
                if (nestDepth < 0) {
                    return -1;
                }
            }
            current++;
        }
        if (current == n) {
            if (start < current) {
                slices[sliceIndex++] = StringSlice(source.basePtr(), source.start + start, source.start + current);
            }
            break;
        }
        slices[sliceIndex++] = StringSlice(source.basePtr(), source.start + start, source.start + current);
        start = ++current;
    }
    return sliceIndex;
}

StringSlice::StringSlice() {}
