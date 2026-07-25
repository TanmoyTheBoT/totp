#include "qrcode.h"
#include "qrcodegen.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <vector>


void QRCode::generatePNG(
    const std::string& text,
    const std::string& filename
)
{
    using namespace qrcodegen;


    QrCode qr =
        QrCode::encodeText(
            text.c_str(),
            QrCode::Ecc::MEDIUM
        );


    int qrSize = qr.getSize();


    int scale = 10;
    int border = 4;


    int imageSize =
        (qrSize + border * 2) * scale;


    std::vector<unsigned char> image(
        imageSize * imageSize * 3,
        255
    );


    for(int y = 0; y < qrSize; y++)
    {
        for(int x = 0; x < qrSize; x++)
        {

            if(qr.getModule(x,y))
            {

                for(int dy = 0; dy < scale; dy++)
                {
                    for(int dx = 0; dx < scale; dx++)
                    {

                        int px =
                            (x + border) * scale + dx;

                        int py =
                            (y + border) * scale + dy;


                        int index =
                            (py * imageSize + px) * 3;


                        image[index] = 0;
                        image[index+1] = 0;
                        image[index+2] = 0;

                    }
                }
            }
        }
    }


    stbi_write_png(
        filename.c_str(),
        imageSize,
        imageSize,
        3,
        image.data(),
        imageSize * 3
    );
}