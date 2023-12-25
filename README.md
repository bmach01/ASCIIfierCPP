C++ 24-bit uncompressed BMP to ASCII art converter created for academic purposes.

Written entirely in C++ and standard library.

1. Loads up file header, basic file checks.
2. Loads up info header, basic image checks, calculates crucial parameters such as
   stride, size etc.
3. Loads up the image, accounts for padding.
4. Outputs characters corresponding to light value of each pixel (3 chars per pixel)
   to specified file.
