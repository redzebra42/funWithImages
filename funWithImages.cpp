#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "imageUtils.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string imgFile = "test_img.png";
    string destFile = "test_img_data.txt";
    parsePngImage(imgFile, destFile);
    return 0;
}


