#ifndef UTILS_STRING_SLICE_H_
#define UTILS_STRING_SLICE_H_

/**
 * A helper class to represent slices of strings.
 */
struct StringSlice {
 private:
  const char **src;
  int start;
  int end;
  int len;

 public:
  /**
   * Construct a StringSlice
   * @param src Source string to use.
   * @param start The starting character, included
   * @param end The ending character, excluded
   */
  StringSlice(const char **src, int start, int end);

  /**
   * Default constructor.
   */
  StringSlice();

  /*
   * See if this StringSlice matches a string constant.
   */
  bool operator==(const char *other) const;

  /**
   * Get a pointer to the first character of the slice.
   * @return
   */
  const char *startPtr() const;

  /**
   * Get a pointer to the base character of the backing string.
   * @return
   */
  const char **basePtr() const;

  /**
   * Get the length of the slice.
   * @return
   */
  int length() const;

  /**
   * Get a character at a specific index
   * @param index
   * @return that character.
   */
  char operator[](int index) const;

  /**
   * Get a pointer to the end of this slice.
   * @return The endpointer.
   */
  const char *endptr() const;

  /**
   * Copy characters from this slice into a buffer.
   * @param buffer
   * @param n number of characters to copy.
   */
  void copy(char *buffer, int n) const;

  /**
   * If this string is wrapped by (), return a new slice
   * without the parens.
   * @return StringSlice without the parens.
   */
  StringSlice inner() const;

  /**
   * Split a StringSlice into multiple other string slices by a delimiter.
   * @param slices Array of slices to split into.
   * @param sliceCount The maximum number of slices to split (probably size of
   * slices)
   * @param delim Delimiter character to split on
   * @return int representing the number of splits made.
   */
  int split(StringSlice *slices, int sliceCount, const char delim);
};

#endif  // UTILS_STRING_SLICE_H_
