#include "Bitmap.h"

Bitmap::Bitmap(std::string filename) {

    std::ifstream file(filename);
    if (!readHeaders(file)) {
        std::cerr << "Failed to read the file.\n";
        file.close();
        return;
    }
    readImage(file);

    file.close();

}

bool Bitmap::readHeaders(std::ifstream &file) {
    file.seekg(0, std::ios::beg);

    file.read((char*)&fileHeader.bfType, sizeof(WORD));
    file.read((char*)&fileHeader.bfSize, sizeof(DWORD));
    file.read((char*)&fileHeader.bfReserved1, sizeof(WORD));
    file.read((char*)&fileHeader.bfReserved1, sizeof(WORD));
    file.read((char*)&fileHeader.bfOffBits, sizeof(DWORD));

    // std::cout << "bfType: " << fileHeader.bfType << std::endl;
    // std::cout << "bfSize: " << fileHeader.bfSize << std::endl;
    // std::cout << "bfReserved1: " << fileHeader.bfReserved1 << std::endl;
    // std::cout << "bfReserved2: " << fileHeader.bfReserved2 << std::endl;
    // std::cout << "bfOffBits: " << fileHeader.bfOffBits << std::endl;

    if (fileHeader.bfType != 0x4D42) return false;

    file.read((char*)&infoHeader.biSize, sizeof(DWORD));
    file.read((char*)&infoHeader.biWidth, sizeof(LONG));
    file.read((char*)&infoHeader.biHeight, sizeof(LONG));
    file.read((char*)&infoHeader.biPlanes, sizeof(WORD));
    file.read((char*)&infoHeader.biBitCount, sizeof(WORD));
    file.read((char*)&infoHeader.biCompression, sizeof(DWORD));
    file.read((char*)&infoHeader.biSizeImage, sizeof(DWORD));
    file.read((char*)&infoHeader.biXPelsPerMeter, sizeof(LONG));
    file.read((char*)&infoHeader.biYPelsPerMeter, sizeof(LONG));
    file.read((char*)&infoHeader.biClrUsed, sizeof(DWORD));
    file.read((char*)&infoHeader.biClrImportant, sizeof(DWORD));

    rowSize =  ((((infoHeader.biWidth * infoHeader.biBitCount) + 31) & ~31) >> 3); // IN BYTE
    infoHeader.biSizeImage = (abs(infoHeader.biHeight) * rowSize); // IN PIXEL
    pixelArraySize = infoHeader.biSizeImage / 3;

    // std::cout << "BITMAPINFOHEADER Members:\n";
    // std::cout << "biSize: " << infoHeader.biSize << "\n";
    // std::cout << "biWidth: " << infoHeader.biWidth << "\n";
    // std::cout << "biHeight: " << infoHeader.biHeight << "\n";
    // std::cout << "biPlanes: " << infoHeader.biPlanes << "\n";
    // std::cout << "biBitCount: " << infoHeader.biBitCount << "\n";
    // std::cout << "biCompression: " << infoHeader.biCompression << "\n";
    // std::cout << "rowSize: " << rowSize << "\n";
    // std::cout << "biSizeImage: " << infoHeader.biSizeImage << "\n";
    // std::cout << "biXPelsPerMeter: " << infoHeader.biXPelsPerMeter << "\n";
    // std::cout << "biYPelsPerMeter: " << infoHeader.biYPelsPerMeter << "\n";
    // std::cout << "biClrUsed: " << infoHeader.biClrUsed << "\n";
    // std::cout << "biClrImportant: " << infoHeader.biClrImportant << "\n";

    if (infoHeader.biPlanes != 1) return false;
    if (infoHeader.biBitCount != 24) return false;
    if (infoHeader.biCompression != 0) return false;
    
    pixelArray = new Pixel[pixelArraySize];

    return true;
}


void Bitmap::readImage(std::ifstream &file) {
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    int padding = rowSize - (3 * infoHeader.biWidth);

    if (padding <= 0) {
        file.read((char*)(pixelArray), infoHeader.biSizeImage);
        return;
    }

    uint8_t r = 0, g = 0, b = 0;
    LONG lim = abs(infoHeader.biHeight);
    char discard[4]{};

    for (LONG i = 0; i < lim; i++) {
        file.read((char*)(&pixelArray[i * infoHeader.biWidth]), infoHeader.biWidth * 3);
        // discard padding
        file.read(discard, padding);
    }
}

void Bitmap::getASCII(std::string filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to write to file.\n";
        return;
    }

    DWORD row = infoHeader.biHeight - 1;
    DWORD end = 0;
    DWORD step = -1;

// For uncompressed RGB bitmaps, if biHeight is positive, the bitmap is a bottom-up DIB with the origin at the lower left corner.
//  If biHeight is negative, the bitmap is a top-down DIB with the origin at the upper left corner.

    // bottom-up bitmap
    if (infoHeader.biHeight < 0) {
        row = 0;
        end = -(infoHeader.biHeight + 1);
        step = 1;
    }

    for (; row != end - 1; row += step) {
        for (DWORD index = 0; index < infoHeader.biWidth; index++) {
            file << std::string(3, pixelArray[row * infoHeader.biWidth + index].getASCIIchar());
            // std::cout << std::string(3, pixelArray[row * infoHeader.biWidth + index].getASCIIchar());
        }
        file << "\n";
        // std::cout << "\n";
    }

    std::cout << "Successfully written to file.\n";
    file.close();
}

Bitmap::~Bitmap() {
    delete[] pixelArray;
}