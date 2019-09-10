// Author: Kartikei Mittal
// Stegnography

#include <iostream>
#include <vector>
#include "D:/CodeSoftwareSetup/CImg-2.7.1/CImg.h"

#define COLOR 2
#define BITS_PER_CHAR 8
#define MAX_BITS_SIZE 20
#define BOOL_VECTOR std::vector<bool>
#define IMAGE cimg_library::CImg<unsigned char>

#define cin std::cin
#define cout std::cout
#define endl std::endl
#define string std::string

void print() { cout<<endl; }
template <
    template <typename, typename> class ContainerType,
    typename ValueType,
    typename AllocType
>
void print(const ContainerType<ValueType, AllocType>& v)
{
    for(ValueType i : v)
        cout<<i<<" ";
    cout<<endl;
}
template<typename T, typename... Rest>
void print(T t, Rest... rs)
{
    cout<<t<<" ";
    print(rs...);
}

template <typename T>
T input(string prompt)
{
    T t;
    cout<<prompt;
    cin>>t;
    return t;
}

BOOL_VECTOR to_bits(int a, int size = MAX_BITS_SIZE)
{
    int index = size-1, temp_size = size;
    BOOL_VECTOR ans(size, false);
    while(a)
    {
        ans[index--] = (a%2==0)? false : true;
        a /= 2;
        if(index == -1)
            return ans;
    }
    return ans;
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

int bits2int(bool in[], int size)
{
    int64_t ans = 0;
    for(int i=0; i<size; ++i)
        if(in[i])
            ans += pow(2, size-i-1);
    return ans;
}

string from_bits(bool in[], int size)
{
    string ans = "";
    for(int i=0; i<(size/BITS_PER_CHAR); ++i)
    {
        bool char_bits[BITS_PER_CHAR];
        for(int j=0; j<BITS_PER_CHAR; ++j)
            char_bits[j] = in[i*BITS_PER_CHAR+j];
        ans += char(bits2int(char_bits, BITS_PER_CHAR));
    }
    return ans;
}

IMAGE load_image(string name="plain.png") { return IMAGE (name.c_str()); }

void save_image(IMAGE im, string name="a_plain.png") { im.save(name.c_str()); }

IMAGE encrypt(string message, string input_file)
{
    int64_t index = 0;
    IMAGE input_image(input_file.c_str());
    BOOL_VECTOR plain = to_bits(message);
    BOOL_VECTOR size_bits = to_bits(message.size());
    for(bool b : plain)
        size_bits.push_back(b);
    plain = size_bits;
    for(int64_t i = 0; i<input_image.width(); ++i)
    for(int64_t j = 0; j<input_image.height(); ++j)
    {
        if(
            (plain[index] == 1 && int(input_image(i, j, 0, COLOR)) % 2 == 0) ||
            (plain[index] == 0 && int(input_image(i, j, 0, COLOR)) % 2 == 1)
        )
            input_image(i, j, 0, COLOR) = int(input_image(i, j, 0, COLOR))+1;
        if(int(input_image(i, j, 0, COLOR)) == 256)
            input_image(i, j, 0, COLOR) = 254;
        ++index;
        if(index >= plain.size())
            return input_image;    
    }
}

int main()
{
    // bool a[] = {
    //     0, 1, 0, 0, 0, 0, 0, 1,
    //     0, 1, 0, 0, 0, 0, 1, 0
    // };
    // print("Hello", "World!");
    // print("from_bits(a): ", from_bits(a, 16));
    // print("to_bits(65):", to_bits(65, 8));
    // print("to_bits(A):", to_bits("A"));
    // int a = input<int>("Hey say some number: ");

    // IMAGE image_i_("plain.png");
    // print(image_i_.width());
    // print((int)image_i_(0, 10, 0, 0));
    // save_image(image_i_);

    // IMAGE a = encrypt("Hey how you doing", "plain.png");
    // save_image(a, "encrypt_plain.png");
    IMAGE a = load_image();
    print(a.width(), a.height());
    for(int i=0; i<500; ++i)
    for(int j=0; j<500; ++j)
    {
        a.draw_point(i, j, 0, 1, 0);
        a.draw_point(i, j, 0, 0, 255);
    }
    save_image(a, "a_plain.png");
    return 0;
}