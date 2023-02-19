#ifndef UTILS_STRING_SLICE_H
#define UTILS_STRING_SLICE_H

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
    bool operator==(const char *other);

    /**
     * Get a pointer to the first character of the slice.
     * @return
     */
    const char *startPtr();

    /**
     * Get a pointer to the base character of the backing string.
     * @return
     */
    const char **basePtr();

    /**
     * Get the length of the slice.
     * @return
     */
    int length();

    /**
     * Get a character at a specific index
     * @param index
     * @return that character.
     */
    char operator[](int index);

    /**
     * Get a pointer to the end of this slice.
     * @return The endpointer.
     */
    const char *endptr();

    /**
     * Copy characters from this slice into a buffer.
     * @param buffer
     * @param n number of characters to copy.
     */
    void copy(char *buffer, int n);

    /**
     * If this string is wrapped by (), return a new slice
     * without the parens.
     * @return StringSlice without the parens.
     */
    StringSlice inner();

    /**
     * Split a StringSlice into multiple other string slices by a delimiter.
     * @param source The source string slice to split.
     * @param slices Array of slices to split into.
     * @param sliceCount The maximum number of slices to split (probably size of slices)
     * @param delim Delimiter character to split on
     * @return int representing the number of splits made.
     */
    static int split(StringSlice &source, StringSlice *slices, int sliceCount, char delim);
};

#endif // UTILS_STRING_SLICE_H
