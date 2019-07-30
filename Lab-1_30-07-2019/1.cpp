// Author: Kartikei Mittal
// Caesar Cipher
#include <bits/stdc++.h>

#define cin std::cin
#define cout std::cout
#define endl std::endl
#define string std::string
#define fstream std::fstream
#define ifstream std::ifstream
#define ofstream std::ofstream

// void print() { cout<<endl; }
// template <typename T>
// void print(T t) { cout<<t<<" "; }
// template <typename T, typename... Rest>
// void print(T t, Rest... rest)
// {
//     print(t);
//     print(rest...);
// }

uint64_t file_size(string name)
{
    fstream my(name.c_str(), std::ios::in);
    auto a = my.tellg();
    my.seekg(0, std::ios::end);
    auto b = my.tellg() - a;
    my.close();
    return (b-a);
}

class Caesar_Cipher
{
    int key, LAST, START;

    char encrypt(char in)
    {
        if(!isalpha(in))
            return in;
        in = tolower(in)+key;
        if(in <= LAST)
            return toupper(in);
        else
            return toupper(in+START-LAST-1);
    }
    char decrypt(char in)
    {
        if(!isalpha(in))
            return in;
        in = tolower(in) - key;
        if(in<START)
            return in+LAST-START+1;
        else
            return in;
    }
public:
    Caesar_Cipher(int a): 
        key(abs(a)%26),
        LAST(122),
        START(97) {;}

    void encrypt_file(string input, string output)
    {
        char temp;
        uint64_t size = file_size(input); 
        ifstream input_file(input.c_str(), std::ios::binary);
        ofstream output_file(output.c_str(), std::ios::binary);
        if(!input_file.is_open())
        {
            cout<<"Input file not found!";
            return ;
        }
        for(uint64_t i = 0; i<size; ++i)
        {
            input_file.get(temp);
            output_file.put(encrypt(temp));
        }
    }
    void decrypt_file(string input, string output)
    {
        char temp;
        uint64_t size = file_size(input); 
        ifstream input_file(input.c_str(), std::ios::binary);
        ofstream output_file(output.c_str(), std::ios::binary);
        if(!input_file.is_open())
        {
            cout<<"Input file not found!";
            return ;
        }
        for(uint64_t i = 0; i<size; ++i)
        {
            input_file.get(temp);
            output_file.put(decrypt(temp));
        }
    }
};

int main()
{
    int k, a;
    string i, o;
    cout<<"Hello! --"<<endl;
    cout<<"Enter key: "; cin>>k;
    Caesar_Cipher cc(k);
    cout<<"Enter Input file: ";
    cin>>i;
    cout<<"Enter Output file: ";
    cin>>o;
    cout<<"--------------------"<<endl
        <<"1. Encrypt"<<endl
        <<"2. Decrypt"<<endl
        <<"\tChoose: ";
    cin>>a;
    if(a!=1 && a!=2)
    {
        cout<<"Invalid choice.";
        return 0;
    }
    if(a==1)
        cc.encrypt_file(i, o);
    else
        cc.decrypt_file(i, o);
    cout<<"Done";
    return 0;
}

