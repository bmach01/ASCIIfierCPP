#include <stdint.h>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <Math.h>
#include <sstream>

typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef int32_t LONG;

struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
};

struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
};

constexpr char ASCII_CHARS[] = "@&%QWNM0gB$#DR8mHXKAUbGOpV4d9h6PkqwSE2]ayjxY5Zoen[ult13If}C{iF|(7J)vTLs?z/*cr!+<>;=^,_:'-.";

struct Pixel {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    uint8_t getIntensity() {
    //    return 0.2126 * red + 0.7152 * green + 0.0722 * blue;
        return std::max(red, std::max(green, blue));
    }

    char getASCIIchar() {
        return ASCII_CHARS[getIntensity() * 89 / 255];
    }

    Pixel() {};
    Pixel(uint8_t r, uint8_t g, uint8_t b) : red(r) , green(g), blue(b) {};
};

class Bitmap {
    private:
        tagBITMAPFILEHEADER fileHeader;
        tagBITMAPINFOHEADER infoHeader;
        DWORD rowSize;
        DWORD pixelArraySize;
        Pixel *pixelArray;

        bool readHeaders(std::ifstream &file);
        void readImage(std::ifstream &file);

    public:
        Bitmap(std::string filename);
        void getASCII(std::string filename);
        ~Bitmap();

};