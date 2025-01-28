#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <string>
#include <vector>

using namespace std;

struct ImageData
{
    int h, w; // height and width
    vector<int> data; // for now, an int represents a pixel
};

// Function to reverse the byte order of a 4-byte integer
int reverseBytes(int value);

// writes the data of an image into another file (doesn't work yet...)
void parseJpgImage(const string &imageName, const string &destinationName);

// parse a png image
void parsePngImage(const string &imageName, const string &destinationName);

#endif