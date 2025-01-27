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

    //find the index for each part of the file
    bool baselineDTC;
    int SOI_index, DRI_index, APP0_index, SOF0_index, SOF2_index, SOS_index, COM_index, EOI_index;
    vector<int> DHT_index, DQT_index;
    for (int i=0; i<size; i++)
    {
        //TODO add the other markers like APPn and RSTn

        // Start of Image
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xD8)
        {
            SOI_index = i;
            cout << "SOI:   " << SOI_index << endl;
        }

        // Define Restart Interval
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xDD)
        {
            DRI_index = i;
            cout << "DRI:   " << DRI_index << endl;
        }

        // Application Specific Segment
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xE0)
        {
            APP0_index = i;
            cout << "APP0:  " << i << endl;
        }

        // Quantization Table (multiple)
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xDB)
        {
            DQT_index.push_back(i);
            cout << "DQT:   " << i << endl;
        }

        // Huffman Table (multiple)
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xC4)
        {
            DHT_index.push_back(i);
            cout << "DHT:   " << i << endl;
        }

        // Start of Frame (baseline DTC)
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xC0)
        {
            baselineDTC = true;
            SOF0_index = i;
            cout << "SOF0:  " << i << endl;
        }

        // Start of Frame (Progressive DTC)
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xC2)
        {
            baselineDTC = false;
            SOF2_index = i;
            cout << "SOF2:  " << i << endl;
        }

        // Start of Scan
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xDA)
        {
            SOS_index = i;
            cout << "SOS:   " << i << endl;
        }

        // Commenraire
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xFE)
        {
            COM_index = i;
            cout << "COM:   " << i << endl;
        }

        // End of Image
        if (buffer[i] == (char)0xFF && buffer[i+1] == (char)0xD9)
        {
            EOI_index = i;
            cout << "EOI:   " << i << endl;
        }
    }

    // printing specific parts
    for (int i=SOS_index+1; i<SOS_index + 20; i++)
    {
        cout << hex << showbase << reinterpret_cast<void*>(buffer[i]) << " ";
    }
    cout << endl;
}