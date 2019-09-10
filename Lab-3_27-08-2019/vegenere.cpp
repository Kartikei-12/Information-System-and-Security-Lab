// Author: Kartikei Mittal
// Vegenere Cipher
#include <bits/stdc++.h>
    using namespace std;

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

class Vegenere_Cipher
{
    string Key;
public:
    Vegenere_Cipher(): Key("SECURITY") {;}
    Vegenere_Cipher(string a) {
        for(char ch : a)
        if(isalpha(ch))
            Key += toupper(ch);
    }

    string encrypt(string msg)
    {
        size_t kLength = Key.length();
        string encryptedMsg = "";
        for(int i=0; i<msg.length(); ++i)
            if(isalpha(msg.at(i)))
                encryptedMsg += (char )((toupper(msg[i]) + Key.at(i%kLength)) % 26) + 'A';
            else
                encryptedMsg += msg.at(i);
        return encryptedMsg;
        // encryptedMsg += (char )(( msg.at(i) + Key.at(i%kLength) + 'A' - 'a' ) % 26) + 'a';
    }

    string decrypt(string enMsg)
    {
        size_t kLength = Key.length();
        string decryptedMsg = "";
        for(int i = 0; i<enMsg.length(); ++i)
            if(isalpha(enMsg[i]))
                decryptedMsg += (char )(( enMsg.at(i) - Key.at(i%kLength) + 'a' - 'A' - 6 ) % 26) + 'A';
            else
                decryptedMsg += enMsg.at(i);
        return decryptedMsg;
        // decryptedMsg += (char )(( encryptedMsg.at(i) - Key.at(i%kLength) - 6 ) % 26) + 'a';
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
    Vegenere_Cipher hl("ENG");
    hl.encrypt_file("a.txt", "b.txt");
    hl.decrypt_file("b.txt", "c.txt");
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
