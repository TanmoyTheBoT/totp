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

std::vector<uint8_t> QRCode::generatePNG(  // static
    const std::string& text,
    int size
)
{
    using namespace qrcodegen;

    QrCode qr = QrCode::encodeText(text.c_str(), QrCode::Ecc::MEDIUM);

    int qrSize = qr.getSize();  // Actual QR modules (e.g., 25x25, 29x29, etc.)

    // Calculate scale to fit size with small white border
    int border = 2;  // 2 module white border on each side
    int totalModules = qrSize + (border * 2);
    int scale = size / totalModules;

    // Actual image size based on calculated scale
    int imageSize = totalModules * scale;

    // Initialize to white
    std::vector<unsigned char> image(imageSize * imageSize * 3, 255);

    // Only render the actual QR code modules (0 to qrSize-1)
    for (int y = 0; y < qrSize; y++) {
        for (int x = 0; x < qrSize; x++) {
            // Check if this module is black in the QR code
            if (qr.getModule(x, y)) {
                // Draw scaled pixel block with border offset
                for (int dy = 0; dy < scale; dy++) {
                    for (int dx = 0; dx < scale; dx++) {
                        int px = (x + border) * scale + dx;
                        int py = (y + border) * scale + dy;

                        // Safety check - stay within bounds
                        if (px < imageSize && py < imageSize) {
                            int index = (py * imageSize + px) * 3;
                            image[index] = 0;      // R
                            image[index + 1] = 0;  // G
                            image[index + 2] = 0;  // B
                        }
                    }
                }
            }
        }
    }

    // Write to memory buffer
    std::vector<uint8_t> pngData;
    int len;
    unsigned char* png = stbi_write_png_to_mem(
        image.data(),
        imageSize * 3,  // stride
        imageSize,      // width
        imageSize,      // height
        3,              // channels
        &len
    );

    if (png) {
        pngData.assign(png, png + len);
        free(png);
    }

    return pngData;
}