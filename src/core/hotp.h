#ifndef HOTP_H
#define HOTP_H

#include <string>
#include <vector>
#include <cstdint>
#include <array>


class HOTP
{
public:

    /*
        Generate HOTP code.

        key:
            Binary secret key

        counter:
            Moving counter value

        digits:
            Number of OTP digits
            Default: 6
    */
    static std::string generate(
        const std::vector<uint8_t>& key,
        uint64_t counter,
        int digits = 6
    );


private:

    /*
        Convert counter to
        8-byte big endian format
    */
    static std::vector<uint8_t> counterToBytes(
        uint64_t counter
    );


    /*
        RFC 4226 dynamic truncation
    */
    static uint32_t dynamicTruncate(
        const std::array<uint8_t,20>& hash
    );
};


#endif // HOTP_H