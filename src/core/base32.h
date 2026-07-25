#ifndef BASE32_H
#define BASE32_H

#include <string>
#include <vector>
#include <cstdint>

namespace base32
{
    /**
     * Decode a Base32-encoded string (RFC 4648).
     *
     * Supports:
     *  - Uppercase and lowercase letters
     *  - Ignores spaces
     *  - Ignores '=' padding
     *
     * Example:
     * "JBSWY3DPEHPK3PXP"
     */
    std::vector<uint8_t> decode(const std::string& input);

    /**
     * Returns true if a character is a valid Base32 character.
     */
    bool isValidChar(char c);
}

#endif // BASE32_H