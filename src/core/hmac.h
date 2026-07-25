#ifndef HMAC_H
#define HMAC_H

#include <array>
#include <vector>
#include <cstdint>
#include <cstddef>


/*
    HMAC-SHA1

    RFC 2104

    HMAC(K, message)

    Output:
        20 bytes SHA-1 digest
*/

std::array<uint8_t, 20> hmacSha1(
    const std::vector<uint8_t>& key,
    const uint8_t* message,
    std::size_t messageLength
);


#endif // HMAC_H