#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "imageUtils.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string imgFile = "test_img.jpg";
    string destFile = "test_img_data.txt";
    parseJpgImage(imgFile, destFile);
    return 0;
}


