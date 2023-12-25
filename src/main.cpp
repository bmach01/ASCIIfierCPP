#include "Bitmap.h"


int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        Bitmap bmp("../resources/" + std::string(argv[i]));
        bmp.getASCII("../output/" + std::string(argv[i]) + ".txt");
    }

    return 0;
}