#include "hotp.h"
#include "hmac.h"

#include <array>
#include <iomanip>
#include <sstream>


std::vector<uint8_t> HOTP::counterToBytes(uint64_t counter)
{
    std::vector<uint8_t> bytes(8);


    /*
        RFC 4226

        Counter must be
        8-byte big endian
    */

    for(int i = 7; i >= 0; i--)
    {
        bytes[i] = counter & 0xff;
        counter >>= 8;
    }


    return bytes;
}



uint32_t HOTP::dynamicTruncate(
    const std::array<uint8_t,20>& hash)
{
    /*
        RFC 4226

        Offset =
        lower 4 bits of last byte
    */

    int offset = hash[19] & 0x0f;



    uint32_t binaryCode =
        ((hash[offset] & 0x7f) << 24)
        |
        ((hash[offset + 1] & 0xff) << 16)
        |
        ((hash[offset + 2] & 0xff) << 8)
        |
        (hash[offset + 3] & 0xff);



    return binaryCode;
}



std::string HOTP::generate(
    const std::vector<uint8_t>& key,
    uint64_t counter,
    int digits)
{
    auto counterBytes =
        counterToBytes(counter);



    auto hash =
        hmacSha1(
            key,
            counterBytes.data(),
            counterBytes.size()
        );



    uint32_t code =
        dynamicTruncate(hash);



    uint32_t otp =
        code % 100000000;



    if(digits == 6)
        otp %= 1000000;

    else if(digits == 8)
        otp %= 100000000;



    std::ostringstream output;


    output
        << std::setw(digits)
        << std::setfill('0')
        << otp;



    return output.str();
}