#ifndef QRCODE_H
#define QRCODE_H

#include <string>


class QRCode
{
public:

    static void generatePNG(
        const std::string& text,
        const std::string& filename
    );

};


#endif