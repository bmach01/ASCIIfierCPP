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
    //if (fileHeader.bfReserved1 != 0 || fileHeader.bfReserved2 != 0) return false;


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

    rowSize = (infoHeader.biWidth * sizeof(Pixel) + 3) & ~3;
    infoHeader.biSizeImage = infoHeader.biHeight * rowSize;
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
    
    pixelArray = new Pixel[pixelArraySize * 100];

    return true;
}


void Bitmap::readImage(std::ifstream &file) {
    file.seekg(fileHeader.bfOffBits, std::ios::beg);
    file.read((char*)(pixelArray), infoHeader.biSizeImage);
}

void Bitmap::getASCII(std::string filename) {
    std::ofstream file(filename);

    DWORD i =  pixelArraySize - 1;
    do {
        file << pixelArray[i].getASCIIchar() << "\u2009";
        i--;
        if (i % infoHeader.biWidth == 0) {
            file << "\n";
        } 

    } while (i > 0);

    std::cout << "Successfully written to file.\n";
    file.close();

}

Bitmap::~Bitmap() {
    delete[] pixelArray;
}