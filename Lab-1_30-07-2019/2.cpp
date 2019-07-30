// Author: Kartikei Mittal
// Mono Alphabetic Cipher
#include <bits/stdc++.h>
    using namespace std;
uint64_t file_size(string name)
{
    fstream my(name.c_str(), std::ios::in);
    auto a = my.tellg();
    my.seekg(0, std::ios::end);
    auto b = my.tellg() - a;
    my.close();
    return (b-a);
}

class Mono_Alphabetic_Cipher
{
    int  LAST, START;
    unordered_map<char, char> encrypt_map, decrypt_map;
    char encrypt(char in)
    {
        if(!isalpha(in))
            return in;
        return encrypt_map[tolower(in)];
    }
    char decrypt(char in)
    {
        if(!isalpha(in))
            return in;
        return decrypt_map[in];
    }
public:
    Mono_Alphabetic_Cipher(string key_):
        LAST(122),
        START(97)
    {
        char temp;
        uint64_t size = file_size(key_);
        map<char, bool> validity_map;
        ifstream key_file(key_.c_str(), ios::binary);
        
        if(size != 26)
        {
            cout<<"invalid key file size.";
            throw "";
        }
        key_ = "";
        for(int i=0; i<26; ++i)
        {
            key_file.get(temp);
            if(!validity_map[temp])
            {
                validity_map[temp] = true;
                key_ += temp; 
            }
            else
            {
                cout<<"Invalid key.";
                throw "";
            }
        }
        for(int i=0; i<26; ++i)
        {
            encrypt_map[(char)(i + START)] = key_[i];
            decrypt_map[key_[i]] = (char)(i + START); 
        }
        key_file.close();
    }

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

int main() try
{
    string k; int a;
    string i, o;
    cout<<"Hello! --"<<endl;
    cout<<"Enter key file: "; cin>>k;
    Mono_Alphabetic_Cipher cc(k);
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
catch(string &e) { cerr<<e; }
catch(exception &e) { cerr<<e.what(); }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"; }
