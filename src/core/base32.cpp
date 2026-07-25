#include "base32.h"

#include <stdexcept>
#include <cctype>

namespace
{
    constexpr char BASE32_ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";

    int charToValue(char c)
    {
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));

        for (int i = 0; i < 32; ++i)
        {
            if (BASE32_ALPHABET[i] == c)
                return i;
        }

        return -1;
    }
}

namespace base32
{

bool isValidChar(char c)
{
    if (std::isspace(static_cast<unsigned char>(c)))
        return true;

    if (c == '=')
        return true;

    return charToValue(c) != -1;
}

std::vector<uint8_t> decode(const std::string& input)
{
    std::vector<uint8_t> output;

    uint32_t buffer = 0;
    int bitsLeft = 0;

    for (char c : input)
    {
        if (std::isspace(static_cast<unsigned char>(c)))
            continue;

        if (c == '=')
            break;

        int value = charToValue(c);

        if (value == -1)
            throw std::runtime_error("Invalid Base32 character.");

        buffer <<= 5;
        buffer |= static_cast<uint32_t>(value);

        bitsLeft += 5;

        while (bitsLeft >= 8)
        {
            bitsLeft -= 8;

            output.push_back(
                static_cast<uint8_t>(
                    (buffer >> bitsLeft) & 0xFF
                )
            );
        }
    }

    return output;
}

} // namespace base32