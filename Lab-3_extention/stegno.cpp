// Author: Kartikei Mittal
// Stegnography
// Compile as ` g++ -std=gnu++11 stegno.cpp -lgdi32 `

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include "D:/CodeSoftwareSetup/CImg-2.7.1/CImg.h"

#define COLOR 1
#define BITS_PER_CHAR 8
#define MAX_BITS_SIZE 20
#define C_UCHAR const unsigned char
#define BOOL_VECTOR std::vector<bool>
#define IMAGE cimg_library::CImg<unsigned char>

#define cin std::cin
#define cout std::cout
#define cerr std::cerr
#define endl std::endl
#define string std::string
#define exception std::exception

void print() { cout<<endl; }
template<typename T, typename... Rest>
void print(T t, Rest... rs) { cout<<t<<" "; print(rs...); }

template <typename T>
T input(string prompt) {
    T t;
    cout<<prompt; cin>>t;
    return t;
}

bool file_exist(const string& name){
    if (FILE *file = fopen(name.c_str(), "r")) { fclose(file); return true; }
    return false;
}

string read_file(string file_name)
{
    if(!file_exist(file_name))
        throw string("File to read do not exist");
    char temp; string data = "";
    std::ifstream file(file_name.c_str(), std::ios::binary);
    while(!file.eof()) { file.get(temp); data += temp; }
    file.close();
    return data;
}

BOOL_VECTOR to_bits(int64_t a, const int size = MAX_BITS_SIZE)
{
    BOOL_VECTOR ans, ans_;
    ans_.clear(); ans.clear();
    
    int64_t t = a;
    while(a)
    {
        ans.push_back(a%2);
        a /= 2;
    }
    
    int temp = ( (size-ans.size()) > 0 )? (size-ans.size()) : 0;
    while(temp--)
        ans.push_back(false);
    
    for(int i = ans.size()-1; i>=0; --i)
        ans_.push_back(ans[i]);
    return ans_;
}

BOOL_VECTOR to_bits(string a)
{
    int index = 0;
    BOOL_VECTOR ans(BITS_PER_CHAR * a.length(), false);
    for(char ch : a)
    {
        BOOL_VECTOR temp = to_bits(int(ch), BITS_PER_CHAR);
        for(int i=0; i<BITS_PER_CHAR; ++i)
            ans[index++] = temp[i];
    }
    return ans;
}

int pow(int a, int b)
{
    int ans = 1;
    while(b--)
        ans *= a;
    return ans; 
}

int bits2int(BOOL_VECTOR in, int size=MAX_BITS_SIZE)
{
    int64_t ans = 0;
    for(int i=0; i<size; ++i) if(in[i])
        ans += pow(2, size-i-1);
    return ans;
}

string from_bits(BOOL_VECTOR in, int size)
{
    string ans = "";
    BOOL_VECTOR char_bits;
    for(int i=0; i<(size/BITS_PER_CHAR); ++i)
    {
        char_bits.clear();
        for(int j=0; j<BITS_PER_CHAR; ++j)
            char_bits.push_back(in[i*BITS_PER_CHAR+j]);
        ans += char(bits2int(char_bits, BITS_PER_CHAR));
    }
    return ans;
}

IMAGE load_image(string name) { return IMAGE (name.c_str()); }

void save_image(IMAGE im, string name) { im.save(name.c_str()); }

IMAGE encrypt(string message, string input_file)
{
    int64_t index = 0;
    IMAGE img(input_file.c_str());
    if(message.length() > pow(2,MAX_BITS_SIZE))
        throw string("Not enough size bits");
    BOOL_VECTOR plain = to_bits(message);
    BOOL_VECTOR size_bits = to_bits(message.length());
    
    for(bool b : plain)
        size_bits.push_back(b);
    plain = size_bits;
    
    if(plain.size() > (img.width()*img.height()))
        throw string("Message is to large.");
    
    for(int j = 0; j<img.height() && index < plain.size(); ++j)
    for(int i = 0; i<img.width() && index < plain.size(); ++i)
    {
        uint16_t pixel[] = {
            img(i, j, 0, 0),
            img(i, j, 0, 1),
            img(i, j, 0, 2)
        };
        if(
            (plain.at(index) == true && (pixel[COLOR] % 2) == 0) ||
            (plain.at(index) == false && (pixel[COLOR] % 2) == 1)
        )
        {
            pixel[COLOR] += (pixel[COLOR] != 255)? 1 : -1;
            C_UCHAR color_spec[] = { (C_UCHAR)pixel[0], (C_UCHAR)pixel[1], (C_UCHAR)pixel[2] };
            img.draw_point(i, j, 0, color_spec);    
        }
        ++index;
    }
    return img;
}

string decrypt(string file)
{
    int64_t i_ = 0, j_ = 0, size, counter = MAX_BITS_SIZE;
    BOOL_VECTOR result_bits, size_bits;
    IMAGE img(file.c_str());
    while(counter--)
    {
        size_bits.push_back(img(j_++, i_, 0, COLOR)%2);
        if(j_ == img.width())
            ++i_, j_ = 0;
    }

    size = bits2int(size_bits) * BITS_PER_CHAR;
    while(size--)
    {
        result_bits.push_back(img(j_++, i_, 0, COLOR)%2);
        if(j_ == img.width())
            ++i_, j_ = 0;
    }
    return from_bits(result_bits, result_bits.size());
}

int main() try
{   
    string file = input<string>("Enter input file: ");
    if(!file_exist(file))
        throw string("Input file do not exist.");
    int choice = input<int>("\t1. Encrypt\n\t2. Decrypt\n\t\tChoose: ");
    if(choice == 1)
    {
        if(file_exist("encrypt_"+file))
            throw string("Output file exist, cannot overwrite.");
        string message = read_file(input<string>("Enter message file: "));
        save_image( encrypt(message, file), "encrypt_"+file);
    }
    else if(choice == 2)
        print("Hidden message:", decrypt(file));
    else
        print("Invalid Choice");
    
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }
