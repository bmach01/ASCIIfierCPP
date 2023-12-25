#include "Bitmap.h"

Bitmap::Bitmap(std::string filename) {

    std::ifstream file(filename);
    if (!readHeaders(file)) {
        std::cerr << "Something's wrong with the provided BMP file.\n";
        file.close();
        return;
    }
    readImage(file);

    file.close();

}

bool Bitmap::readHeaders(std::ifstream &file) {

    file.read((char*)&fileHeader.bfType, sizeof(WORD));
    file.read((char*)&fileHeader.bfSize, sizeof(DWORD));
    file.read((char*)&fileHeader.bfReserved1, sizeof(WORD));
    file.read((char*)&fileHeader.bfReserved1, sizeof(WORD));
    file.read((char*)&fileHeader.bfOffBits, sizeof(DWORD));

    if (fileHeader.bfType != 19778) return false;
    //if (fileHeader.bfReserved1 != 0 || fileHeader.bfReserved2 != 0) return false;

    std::cout << "bfType: " << fileHeader.bfType << std::endl;
    std::cout << "bfSize: " << fileHeader.bfSize << std::endl;
    std::cout << "bfReserved1: " << fileHeader.bfReserved1 << std::endl;
    std::cout << "bfReserved2: " << fileHeader.bfReserved2 << std::endl;
    std::cout << "bfOffBits: " << fileHeader.bfOffBits << std::endl;



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

    if (infoHeader.biPlanes != 1) return false;
    if (infoHeader.biBitCount != 24) return false;
    if (infoHeader.biCompression != 0) return false;
    

    stride = ((((infoHeader.biWidth * infoHeader.biBitCount) + 31) & ~31) >> 3) / 3;
    infoHeader.biSizeImage = abs(infoHeader.biHeight) * stride; // IN PIXELS

    pixelArray = new Pixel[infoHeader.biSizeImage];

    std::cout << "BITMAPINFOHEADER Members:\n";
    std::cout << "biSize: " << infoHeader.biSize << "\n";
    std::cout << "biWidth: " << infoHeader.biWidth << "\n";
    std::cout << "biHeight: " << infoHeader.biHeight << "\n";
    std::cout << "biPlanes: " << infoHeader.biPlanes << "\n";
    std::cout << "biBitCount: " << infoHeader.biBitCount << "\n";
    std::cout << "biCompression: " << infoHeader.biCompression << "\n";
    std::cout << "stride: " << stride << "\n";
    std::cout << "biSizeImage: " << infoHeader.biSizeImage << "\n";
    std::cout << "biXPelsPerMeter: " << infoHeader.biXPelsPerMeter << "\n";
    std::cout << "biYPelsPerMeter: " << infoHeader.biYPelsPerMeter << "\n";
    std::cout << "biClrUsed: " << infoHeader.biClrUsed << "\n";
    std::cout << "biClrImportant: " << infoHeader.biClrImportant << "\n";

    return true;
}

void Bitmap::readImage(std::ifstream &file) {
    //file.seekg(fileHeader.bfOffBits - file.tellg());
    uint8_t r = 0, g = 0, b = 0;

    // left-down OR left-top
    DWORD row = 0;
    DWORD index = 0;
    DWORD end = infoHeader.biHeight - 1;
    DWORD step = 1;

    if (infoHeader.biHeight > 0) {
        row = end;
        end = 0;
        step = -1;
    }

    while (row != end) {
        index = 0;
        while (index < infoHeader.biWidth) {
            file.read((char*)&r, 1);
            file.read((char*)&g, 1);
            file.read((char*)&b, 1);

            pixelArray[infoHeader.biWidth * row + index] = Pixel(r, g, b);
            index++;
        }
        row += step;
    }
}

void Bitmap::getASCII(std::string filename) {
    std::ofstream file(filename);

    DWORD i = 0;
    do {
        file << pixelArray[i].getASCIIchar() << " ";
        i++;
        if (i % stride == 0) file << "\n";

    } while (i < infoHeader.biSizeImage);


    file.close();

}

Bitmap::~Bitmap() {
    delete[] pixelArray;
}