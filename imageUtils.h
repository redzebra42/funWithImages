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

// writes the data of an image into another file
void parseJpgImage(const string &imageName, const string &destinationName);

#endif