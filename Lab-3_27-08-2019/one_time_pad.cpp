// Author: Kartikei Mittal
// Vegenere(One Time Pad modification) Cipher
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
string read_file(string file_name)
{
    char temp; string data = "";
    ifstream file(file_name.c_str(), ios::binary);
    if(!file.is_open()) throw "Input file not found!";
    while(!file.eof())
    {
        file.get(temp);
        data += temp;
    }file.close();
    return data;
}

void write_file(string file_name, string data)
{
    ofstream file(file_name.c_str(), ios::binary);
    for(char ch : data) file.put(ch);
    file.close();
}

class Vegenere_CipherOTP
{
    string Key;
public:
    Vegenere_CipherOTP(int64_t file_len, string key_file="")
    {
        if(key_file != "")
            Key = read_file(key_file);
        else
        {
            Key = "";
            std::random_device rd;
            std::mt19937 eng(rd());
            std::uniform_int_distribution<> distr(65, 65+26);
            while(file_len--)
                Key += distr(eng);
            write_file("key.txt", Key); 
        }
    }

    string encrypt(string msg)
    {
        string encryptedMsg = "";
        for(int i=0; i<msg.length(); ++i)
            if(isalpha(msg.at(i)))
                encryptedMsg += (char )((toupper(msg[i]) + Key.at(i)) % 26) + 'A';
            else
                encryptedMsg += msg.at(i);
        return encryptedMsg;
    }

    string decrypt(string enMsg)
    {
        string decryptedMsg = "";
        for(int i = 0; i<enMsg.length(); ++i)
            if(isalpha(enMsg[i]))
                decryptedMsg += (char )(( enMsg.at(i) - Key.at(i) + 'a' - 'A' - 6 ) % 26) + 'A';
            else
                decryptedMsg += enMsg.at(i);
        return decryptedMsg;
    }
    void encrypt_file(string input_file, string output_file) {
        write_file(output_file, encrypt(read_file(input_file)));
    }
    void decrypt_file(string input_file, string output_file) {
        write_file(output_file, decrypt(read_file(input_file)));
    }
};

int main() try
{
    Vegenere_CipherOTP h1(file_size("a.txt"));
    h1.encrypt_file("a.txt", "b.txt");
    Vegenere_CipherOTP h2(file_size("b.txt"), "key.txt");
    h2.decrypt_file("b.txt", "c.txt");
    // string k; int a;
    // string i, o;
    // cout<<"Hello! --"<<endl;
    // cout<<"Enter key file: "; cin>>k;
    // Play_Fair_Cipher cc(read_file(k));
    // cout<<"Enter Input file: ";
    // cin>>i;
    // cout<<"Enter Output file: ";
    // cin>>o;
    // cout<<"--------------------"<<endl
    //     <<"1. Encrypt"<<endl
    //     <<"2. Decrypt"<<endl
    //     <<"\tChoose: ";
    // cin>>a;
    // if(a!=1 && a!=2)
    // {
    //     cout<<"Invalid choice.";
    //     return 0;
    // }
    // if(a==1)
    //     cc.encrypt_file(i, o);
    // else
    //     cc.decrypt_file(i, o);
    cout<<"Done";
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }
