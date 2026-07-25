#include "sha1.h"

#include <cstring>


SHA1::SHA1()
{
    reset();
}


void SHA1::reset()
{
    m_totalBytes = 0;

    m_h0 = 0x67452301;
    m_h1 = 0xEFCDAB89;
    m_h2 = 0x98BADCFE;
    m_h3 = 0x10325476;
    m_h4 = 0xC3D2E1F0;

    m_bufferSize = 0;

    std::memset(m_buffer, 0, sizeof(m_buffer));
}


uint32_t SHA1::leftRotate(uint32_t value, uint32_t bits)
{
    return (value << bits) | (value >> (32 - bits));
}


void SHA1::processBlock(const uint8_t block[64])
{
    uint32_t w[80];


    // Convert block into 16 big-endian words
    for (int i = 0; i < 16; i++)
    {
        w[i] =
            (static_cast<uint32_t>(block[i * 4]) << 24) |
            (static_cast<uint32_t>(block[i * 4 + 1]) << 16) |
            (static_cast<uint32_t>(block[i * 4 + 2]) << 8) |
            (static_cast<uint32_t>(block[i * 4 + 3]));
    }


    // Extend words to 80
    for (int i = 16; i < 80; i++)
    {
        w[i] =
            leftRotate(
                w[i - 3] ^
                w[i - 8] ^
                w[i - 14] ^
                w[i - 16],
                1
            );
    }


    uint32_t a = m_h0;
    uint32_t b = m_h1;
    uint32_t c = m_h2;
    uint32_t d = m_h3;
    uint32_t e = m_h4;


    for (int i = 0; i < 80; i++)
    {
        uint32_t f;
        uint32_t k;


        if (i < 20)
        {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40)
        {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60)
        {
            f = (b & c) |
                (b & d) |
                (c & d);

            k = 0x8F1BBCDC;
        }
        else
        {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }


        uint32_t temp =
            leftRotate(a, 5)
            + f
            + e
            + k
            + w[i];


        e = d;
        d = c;
        c = leftRotate(b, 30);
        b = a;
        a = temp;
    }


    m_h0 += a;
    m_h1 += b;
    m_h2 += c;
    m_h3 += d;
    m_h4 += e;
}



void SHA1::update(const uint8_t* data, std::size_t len)
{
    m_totalBytes += len;


    while (len > 0)
    {
        std::size_t copySize =
            64 - m_bufferSize;


        if (copySize > len)
            copySize = len;


        std::memcpy(
            m_buffer + m_bufferSize,
            data,
            copySize
        );


        m_bufferSize += copySize;
        data += copySize;
        len -= copySize;


        if (m_bufferSize == 64)
        {
            processBlock(m_buffer);
            m_bufferSize = 0;
        }
    }
}



std::array<uint8_t,20> SHA1::final()
{
    uint64_t bitLength =
        m_totalBytes * 8;


    // Add 0x80
    m_buffer[m_bufferSize++] = 0x80;


    // Pad until 56 bytes
    if (m_bufferSize > 56)
    {
        while (m_bufferSize < 64)
            m_buffer[m_bufferSize++] = 0;

        processBlock(m_buffer);

        m_bufferSize = 0;
    }


    while (m_bufferSize < 56)
        m_buffer[m_bufferSize++] = 0;



    // Append length (big endian)
    for (int i = 7; i >= 0; i--)
    {
        m_buffer[m_bufferSize++] =
            static_cast<uint8_t>(
                bitLength >> (i * 8)
            );
    }


    processBlock(m_buffer);



    std::array<uint8_t,20> digest;


    uint32_t hash[5] =
    {
        m_h0,
        m_h1,
        m_h2,
        m_h3,
        m_h4
    };


    for (int i = 0; i < 5; i++)
    {
        digest[i * 4] =
            (hash[i] >> 24) & 0xff;

        digest[i * 4 + 1] =
            (hash[i] >> 16) & 0xff;

        digest[i * 4 + 2] =
            (hash[i] >> 8) & 0xff;

        digest[i * 4 + 3] =
            hash[i] & 0xff;
    }


    reset();


    return digest;
}