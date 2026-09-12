#ifndef QRCODE_H
#define QRCODE_H

#include <string>
#include <vector>

class QRCode
{
public:
    static void generatePNG(
        const std::string& text,
        const std::string& filename
    );

    // Generate PNG data in memory and return as vector
    static std::vector<uint8_t> generatePNG(
        const std::string& text,
        int size = 300
    );
};

#endif