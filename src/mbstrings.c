#include "mbstrings.h"

/* mbslen - multi-byte string length
 * - Description: returns the number of UTF-8 code points ("characters")
 * in a multibyte string. If the argument is NULL or an invalid UTF-8
 * string is passed, returns -1.
 *
 * - Arguments: A pointer to a character array (`bytes`), consisting of UTF-8
 * variable-length encoded multibyte code points.
 *
 * - Return: returns the actual number of UTF-8 code points in `src`. If an
 * invalid sequence of bytes is encountered, return -1.
 *
 * - Hints:
 * UTF-8 characters are encoded in 1 to 4 bytes. The number of leading 1s in the
 * highest order byte indicates the length (in bytes) of the character. For
 * example, a character with the encoding 1111.... is 4 bytes long, a character
 * with the encoding 1110.... is 3 bytes long, and a character with the encoding
 * 1100.... is 2 bytes long. Single-byte UTF-8 characters were designed to be
 * compatible with ASCII. As such, the first bit of a 1-byte UTF-8 character is
 * 0.......
 *
 * You will need bitwise operations for this part of the assignment!
 */
size_t mbslen(const char* bytes) {
    // if the input is NULL, return -1
    if (bytes == NULL) {
        return -1;
    }

    size_t length = 0;
    // while we havent hit the null terminator in the string
    while(*bytes != '\0') {
        // if the first bit is 0, then it is a single byte character
        // 0x80 = 10000000
        // 0x80 & 0x80 = 10000000 & 10000000 = 10000000 = 0x80
        if ((*bytes & 0x80) == 0) {
            length++;
            bytes++;
        } 
        else if ((*bytes & 0xE0) == 0xC0) {
            // if the first 3 bits are 110, then it is a 2 byte character
            length++;
            bytes += 2;
        }
        else if ((*bytes & 0xF0) == 0xE0) {
            // if the first 4 bits are 1110, then it is a 3 byte character
            length++;
            bytes += 3;
        }
        else if ((*bytes & 0xF8) == 0xF0) {
            // if the first 5 bits are 11110, then it is a 4 byte character
            length++;
            bytes += 4;
        }
        else {
            // if the first bit is not 0, 110, 1110, or 11110, then it is an invalid character
            return -1;
        }
    }
    return length;
}
