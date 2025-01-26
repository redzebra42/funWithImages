#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

void parseJpgImage(const string &imageName, const string &destinationName)
{
    //open the files
    ifstream ifs (imageName, ios::in | ios::binary);
    ofstream ofs (destinationName);

    // check if the file opens
    if (!(ifs.is_open() && ofs.is_open()))
    {
        cerr << "failed to open the files" << endl;
    }

    // read the file in a buffer vector
    ifs.seekg(0, ios::end);
    int size = ifs.tellg();
    ifs.seekg(0, ios::beg);
    vector<char> buffer(size);
    ifs.read(buffer.data(), size);
    ifs.close();

    for (int i=0; i<size; i++)
    {
        // Start of Image
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xD8)
        {
            cout << "SOI:   " << i << endl;
        }

        // Application Specific Segment
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xE0)
        {
            cout << "APP0:  " << i << endl;
        }

        // Quantization Table
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xDB)
        {
            cout << "DQT:   " << i << endl;
        }

        // Huffman Table
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xC4)
        {
            cout << "DHT:   " << i << endl;
        }

        // Start of Frame
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xC0)
        {
            cout << "SOF0:  " << i << endl;
        }

        // Start of Scan
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xDA)
        {
            cout << "SOS:   " << i << endl;
        }

        // End of Image
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xD9)
        {
            cout << "EOI:   " << i << endl;
        }
    }
}