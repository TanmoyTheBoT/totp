#ifndef SHA1_H
#define SHA1_H

#include <array>
#include <cstddef>
#include <cstdint>

class SHA1
{
public:
    SHA1();

    // Feed data into the hash
    void update(const uint8_t* data, std::size_t len);

    // Convenience overload
    void update(const char* data, std::size_t len)
    {
        update(reinterpret_cast<const uint8_t*>(data), len);
    }

    // Finish hashing and return the 20-byte digest.
    // The object is reset after calling final().
    std::array<uint8_t, 20> final();

    // Reset internal state
    void reset();

private:
    void processBlock(const uint8_t block[64]);

    static uint32_t leftRotate(uint32_t value, uint32_t bits);

private:
    uint64_t m_totalBytes;

    uint32_t m_h0;
    uint32_t m_h1;
    uint32_t m_h2;
    uint32_t m_h3;
    uint32_t m_h4;

    uint8_t m_buffer[64];
    std::size_t m_bufferSize;
};

#endif // SHA1_H