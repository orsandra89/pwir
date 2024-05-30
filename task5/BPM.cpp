#ifndef BPM_H
#define BPM_H

#include <iostream>
#include <fstream>
#include <vector>

class BMP;

class Drawable {
public:
    virtual void draw(BMP &image) const = 0;
    virtual ~Drawable() = default;
};

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType{0x4D42};  // "BM"
    uint32_t bfSize{0};       // Size of the file (in bytes)
    uint16_t bfReserved1{0};  // Reserved, always 0
    uint16_t bfReserved2{0};  // Reserved, always 0
    uint32_t bfOffBits{54};   // Offset to start of pixel data
};

struct BMPInfoHeader {
    uint32_t biSize{40};      // Size of the info header (in bytes)
    int32_t biWidth{0};       // Width of the image
    int32_t biHeight{0};      // Height of the image
    uint16_t biPlanes{1};     // Number of color planes
    uint16_t biBitCount{24};  // Number of bits per pixel
    uint32_t biCompression{0}; // Compression type
    uint32_t biSizeImage{0};   // Image size (in bytes)
    int32_t biXPelsPerMeter{0x0B13}; // Pixels per meter (horizontal)
    int32_t biYPelsPerMeter{0x0B13}; // Pixels per meter (vertical)
    uint32_t biClrUsed{0};       // Number of colors used
    uint32_t biClrImportant{0};  // Number of important colors
};
#pragma pack(pop)

class BMP {
private:
    int width;
    int height;
    std::vector<uint8_t> pixels;
public:
    BMP(int width, int height) : width(width), height(height) {
        pixels.resize(width * height * 3, 255);  // Initialize with white color
    }

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            size_t index = ((height - y - 1) * width + x) * 3;
            pixels[index] = b;
            pixels[index + 1] = g;
            pixels[index + 2] = r;
        }
    }

    void save(const std::string &filename) {
        BMPHeader bmpHeader;
        BMPInfoHeader bmpInfoHeader;

        bmpInfoHeader.biWidth = width;
        bmpInfoHeader.biHeight = height;
        bmpHeader.bfSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + pixels.size();
        bmpInfoHeader.biSizeImage = pixels.size();

        std::ofstream outFile(filename, std::ios::binary);
        outFile.write(reinterpret_cast<const char *>(&bmpHeader), sizeof(bmpHeader));
        outFile.write(reinterpret_cast<const char *>(&bmpInfoHeader), sizeof(bmpInfoHeader));
        outFile.write(reinterpret_cast<const char *>(pixels.data()), pixels.size());
    }

    void drawAll(const std::vector<std::shared_ptr<Drawable>> &drawables) {
        for (const auto &drawable : drawables) {
            drawable->draw(*this);
        }
    }
};

#endif
