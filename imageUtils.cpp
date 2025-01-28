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

// Function to reverse the byte order of a 4-byte integer
int reverseBytes(int value)
{
    return ((value >> 24) & 0xFF) | // Move byte 0 to byte 3
           ((value << 8) & 0xFF0000) | // Move byte 1 to byte 2
           ((value >> 8) & 0xFF00) |   // Move byte 2 to byte 1
           ((value << 24) & 0xFF000000); // Move byte 3 to byte 0
}

void parsePngImage(const string &imageName, const string &destinationName)
{
    ifstream ifs (imageName);
    ifs.seekg(ios::beg);

    // read file signature
    char file_signature[8];
    for (int i=0; i<8; i++)
    {
        ifs.read(&file_signature[i], sizeof(char));
    }
    cout << "format:     "
         << (char)file_signature[1] 
         << (char)file_signature[2] 
         << (char)file_signature[3] 
         << endl;
    if (!(file_signature[0] == (char)0x89
     && file_signature[1] == (char)0x50
     && file_signature[2] == (char)0x4e
     && file_signature[3] == (char)0x47
     && file_signature[4] == (char)0x0d
     && file_signature[5] == (char)0x0a
     && file_signature[6] == (char)0x1a
     && file_signature[7] == (char)0x0a))
    {
    cerr << "file isn't in PNG format" << endl;
    }

    // read image header
    int header_size;
    vector<char> header_identifier(4);
    int image_width, image_height;
    int bpp, color, compression, filter, interlace;
    ifs.read((char*)&header_size, sizeof(header_size));
    header_size = reverseBytes(header_size); // the files stores integers in big-endian format
    ifs.read(header_identifier.data(), 4);
    ifs.read((char*)&image_width, sizeof(image_width));
    image_width = reverseBytes(image_width);
    ifs.read((char*)&image_height, sizeof(image_height));
    image_height = reverseBytes(image_height);
    unsigned char byte;
    ifs.read((char*)&byte, 1); 
    bpp = (int)byte;                // number of bits per sample or per palette index (not per pixel)
    ifs.read((char*)&byte, 1);
    color = (int)byte;
    ifs.read((char*)&byte, 1);
    compression = (int)byte;     // only 0
    ifs.read((char*)&byte, 1);
    filter = (int)byte;          // only 0
    ifs.read((char*)&byte, 1);
    interlace = (int)byte;       // 0: no interlace,  1: Adam7 interlace

    //skip the CRC for now
    cout << ifs.tellg() << endl;
    ifs.seekg(4,ios::cur);
    cout << ifs.tellg() << endl;
    
    cout << endl << header_identifier[0] << header_identifier[1] << header_identifier[2] << header_identifier[3] << ":       " << header_size << " bytes" << endl;
    cout << "width:      " << image_width << "px" << endl;
    cout << "height:     " << image_height << "px" << endl;
    cout << "bpp:        " << bpp << endl; 
    cout << "color:      " << color << endl; 
    cout << "compress:   " << compression << endl; 
    cout << "filter:     " << filter << endl; 
    cout << "interlace:  " << interlace << endl;

    // read image data
    int data_size;
    vector<char> data_id(4);

    ifs.read((char*)&data_size, 4);
    data_size = reverseBytes(data_size); // the files stores integers in big-endian format
    ifs.read(data_id.data(), 4);
    
    // skip over unnecessary data chunks (for now)
    while (!(data_id[0] == (char)0x54 &&
             data_id[1] == (char)0x41 &&
             data_id[1] == (char)0x44 &&
             data_id[1] == (char)0x49))
    {
        //skip unnecessary data
        ifs.seekg(4 + data_size,ios::cur);

        // read new size/identifier
        ifs.read((char*)&data_size, 4);
        data_size = reverseBytes(data_size); // the files stores integers in big-endian format
        ifs.read(data_id.data(), 4);
        cout << endl << data_id[0] << data_id[1] << data_id[2] << data_id[3] << ":       " << data_size << " bytes" << endl;
        if (data_size == 0)
        {
            break;
        }
    }

    int compression_method, add_flag_checks;

    cout << endl << data_id[0] << data_id[1] << data_id[2] << data_id[3] << ":       " << data_size << " bytes" << endl;

    // TODO skip over the non essencial data chunks (those which start with a non capital letter)
}
