// Author: Kartikei Mittal
// Rail Fence Cipher
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

// class Rail_Fence
// {
// public:
//     void encrypt_file(string input_file, string output_file)
//     {
//         string cipher = "";
//         input_file = read_file(input_file);
//         if(input_file.length() & 2 != 0)
//             input_file += 'X';
//         for(int i=0; i<input_file.size(); i+=2)
//             cipher += toupper(input_file[i]);
//         for(int i=1; i<input_file.size(); i+=2)
//             cipher += toupper(input_file[i]);
//         write_file(output_file, cipher);
//     }
//     void decrypt_file(string input_file, string output_file)
//     {
//         int64_t mid;
//         string plain = "";
//         input_file = read_file(input_file);
//         mid = input_file.length() / 2;
//         for(int i=0; i<mid; ++i)
//         {
//             plain += tolower(input_file[i]);
//             plain += tolower(input_file[mid+i]);
//         }
//         write_file(output_file, plain);
//     }
// };


class Rail_Fence
{
    string KEY;
public:
    Rail_Fence(string num)
    {
        unordered_map<char, bool> validity_map;
        for(char ch : num)
            if(!isdigit(ch))
                throw string("ValueError: Non - digit encountered in key");
            else if(validity_map[ch])
                throw string("ValueError: Repetetive digit encountered in key");
            else
                validity_map[ch] = true;
        string copy = num;
        sort(copy.begin(), copy.end());
        for(int i=0; i<copy.length(); ++i)
        if(copy[i] != '0'+i)
            throw string("Non strict sequence encountered");
        //Haa
    }
};

int main() try
{
    Rail_Fence hl("3102");
    // hl.encrypt_file("a.txt", "b.txt");
    // hl.decrypt_file("b.txt", "c.txt");
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
