// Author: Kartikei Mittal
// Play Fair Cipher
#include <bits/stdc++.h>
    using namespace std;
#define int_pair pair<int, int>
#define char_pair pair<char, char>
#define cp2p_map unordered_map<pair<char, char>, pair<char, char>, hash_pair>
struct hash_pair { 
    template <class T1, class T2> 
    size_t operator()(const pair<T1, T2>& p) const
    { 
        auto hash1 = hash<T1>{}(p.first); 
        auto hash2 = hash<T2>{}(p.second); 
        return hash1 ^ hash2; 
    } 
};

string read_file(string file_name)
{
    char temp;
    string data = "";
    ifstream file(file_name.c_str(), ios::binary);
    if(!file.is_open())
        throw "Input file not found!";
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
    for(char ch : data)
        file.put(ch);
    file.close();
}

class Play_Fair_Cipher
{
    vector<vector<char>> KeyMatrix;
    cp2p_map encrypt_map;
    cp2p_map decrypt_map;
    int_pair get_index(char c) {
        for(int i=0; i<5; ++i)
        for(int j=0; j<5; ++j)
            if(KeyMatrix[i][j] == c)
                return int_pair(i, j);
    }
public:
    Play_Fair_Cipher(string key_)
    {
        int x = 0, y = 0, p = 0;
        unordered_map<char, bool> validity_map; validity_map['J'] = true;
        KeyMatrix.clear();
        for(int i=0; i<25; ++i)
            KeyMatrix.push_back(vector<char>(5, 'x'));
        for(int i=0; i<key_.length(); ++i)
            if(!isalpha(key_[i]))
                throw "Invalid key!!\n";
            else
                key_[i] = toupper(key_[i]);
        
        for(char  ch : key_)
            if(!validity_map[ch])
            {
                if(ch == 'I' || ch == 'J')
                    KeyMatrix[x][y] = 'I', validity_map['I'] = true;     
                else
                    KeyMatrix[x][y] = ch, validity_map[ch] = true;
                ++y;
                if(y == 5)
                    y = 0, ++x;
            }
        for(int i=x; i<5; ++i)
            for(int j = (i == x)? y: 0; j<5; ++j, ++p)
                if(!validity_map[p+'A'])
                    KeyMatrix[i][j] = p+'A';
                else
                    --j;
        for(int i=0; i<5; ++i)
        {
            for(int j=0; j<5; ++j)
                cout<<KeyMatrix[i][j]<<" ";
            cout<<endl;
        }
        /*
        for(int i=4; i>=x; --i)
            for(int j = 4; j>=((i==x)? y : 0); --j, ++p)
                if(!validity_map[p+'A'])
                    KeyMatrix[i][j] = p+'A';
                else
                    ++j;
        */
        for(int i=0; i<26; ++i)
        for(int j=0; j<26; ++j)
        {
            char p1 = 'A'+i, p2 = 'A'+j, e1,e2;
            if(p1 == p2) p2 = 'X';
            if(p1 == 'J') p1 = 'I';
            if(p2 == 'J') p2 = 'I';
            int_pair p1_i = get_index(p1), p2_i = get_index(p2);
            if(p1_i.first == p2_i.first) {
                e1 = KeyMatrix[p1_i.first][(p1_i.second+1)%5];
                e2 = KeyMatrix[p2_i.first][(p2_i.second+1)%5];
            } else if(p1_i.second == p2_i.second) {
                e1 = KeyMatrix[(p1_i.first+1)%5][p1_i.second];
                e2 = KeyMatrix[(p2_i.first+1)%5][p2_i.second];
            } else {
                e1 = KeyMatrix[p1_i.first][p2_i.second];
                e2 = KeyMatrix[p2_i.first][p1_i.second];
            }
            encrypt_map[char_pair('A'+i,'A'+j)] = char_pair(e1,e2);
            decrypt_map[char_pair(e1,e2)] = char_pair(p1,p2);
        }
        KeyMatrix.clear();
    }

    void encrypt_file(string input_file, string output_file)
    {
        int p = 0;
        string u_data = "";
        input_file = read_file(input_file);
        for(char ch : input_file)
            if(isalpha(ch))
                u_data += toupper(ch);
        if(u_data.length()%2 != 0)
            u_data += 'X';
        for(int i=0; i<u_data.length(); i+=2)
        {
            char_pair temp(u_data[i], u_data[i+1]);
            u_data[i] = encrypt_map[temp].first;
            u_data[i+1] = encrypt_map[temp].second;
        }
        for(int i=0; i<input_file.length(); ++i)
            if(isalpha(input_file[i]))
                input_file[i] = u_data[p++];
        write_file(output_file, input_file);
    }
    void decrypt_file(string input_file, string output_file)
    {
        int p = 0;
        string u_data = "";
        input_file = read_file(input_file);
        for(char ch : input_file)
            if(isalpha(ch))
                u_data += ch;
        for(int i=0; i<u_data.length(); i+=2)
        {
            // if(u_data[i] == 'T' && u_data[i+1] == 'T')
            // {
            //     u_data[i] = 't';
            //     u_data[i+1] = 't';
            //     continue;    
            // }
            char_pair temp(u_data[i], u_data[i+1]);
            u_data[i] = decrypt_map[temp].first;
            u_data[i+1] = decrypt_map[temp].second;
        }
        for(int i=0; i<input_file.length(); ++i)
            if(isalpha(input_file[i]))
                input_file[i] = u_data[p++];
        cout<<input_file;
        write_file(output_file, input_file);
    }
};

int main() try
{
    Play_Fair_Cipher pf("cryptography");
    pf.encrypt_file("a.txt", "b.txt");
    pf.decrypt_file("b.txt", "c.txt");
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
catch(string &e) { cerr<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }
