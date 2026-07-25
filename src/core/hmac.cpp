#include "hmac.h"
#include "sha1.h"

#include <vector>
#include <cstring>


std::array<uint8_t,20> hmacSha1(
    const std::vector<uint8_t>& key,
    const uint8_t* message,
    std::size_t messageLength)
{
    constexpr std::size_t BLOCK_SIZE = 64;


    std::vector<uint8_t> processedKey(BLOCK_SIZE, 0);


    /*
        If key > 64 bytes:
            key = SHA1(key)

        Else:
            pad key with zeros
    */
    if (key.size() > BLOCK_SIZE)
    {
        SHA1 sha;

        sha.update(
            key.data(),
            key.size()
        );

        auto hash = sha.final();


        std::memcpy(
            processedKey.data(),
            hash.data(),
            hash.size()
        );
    }
    else
    {
        std::memcpy(
            processedKey.data(),
            key.data(),
            key.size()
        );
    }



    std::vector<uint8_t> ipad(BLOCK_SIZE);
    std::vector<uint8_t> opad(BLOCK_SIZE);



    for(std::size_t i = 0; i < BLOCK_SIZE; i++)
    {
        ipad[i] = processedKey[i] ^ 0x36;
        opad[i] = processedKey[i] ^ 0x5c;
    }



    /*
        Inner hash:

        SHA1(
            ipad || message
        )
    */

    SHA1 inner;


    inner.update(
        ipad.data(),
        ipad.size()
    );


    inner.update(
        message,
        messageLength
    );


    auto innerHash = inner.final();



    /*
        Outer hash:

        SHA1(
            opad || innerHash
        )
    */

    SHA1 outer;


    outer.update(
        opad.data(),
        opad.size()
    );


    outer.update(
        innerHash.data(),
        innerHash.size()
    );


    return outer.final();
}