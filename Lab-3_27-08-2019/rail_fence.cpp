// Author: Kartikei Mittal
// Rail Fence Cipher
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#define max std::max
#define min std::min
#define cin std::cin
#define cout std::cout
#define cerr std::cerr
#define endl std::endl
#define sort std::sort
#define find std::find
#define vector std::vector
#define string std::string
#define ifstream std::ifstream
#define ofstream std::ofstream
#define exception std::exception
#define unordered_map std::unordered_map

#define INT_VECTOR vector<int>
#define CHAR_VECTOR vector<char>
#define CHAR_MATRIX vector<vector<char>>


void print() { cout<<endl; }
void print__(auto a) { cout<<a; }
void print__(string a) { cout<<a; };
template <
    template <typename, typename...> class ContainerType,
    typename ValueType,
    typename... Args
>
void print__(const ContainerType<ValueType, Args...>& c)
{
    cout<<"[";
    for(const ValueType& v : c)
    {
        print__(v);
        cout<<",";
    }
    cout<<"]"<<endl;
}
template<typename T, typename... Rest>
void print(T t, Rest... rs) {
    print__(t);
    cout<<" ";
    print(rs...); 
}


template <
    template <typename, typename> class ContainerType,
    typename ValueType,
    typename AllocType
>
size_t get_index(ValueType ele , const ContainerType<ValueType, AllocType>& v) {
    return find(v.begin(), v.end(), ele) - v.begin();
}


template <typename T>
T input(string prompt) {
    T t;
    cout<<prompt; cin>>t;
    return t;
}

string read_file(string file_name)
{
    char temp; string data = "";
    ifstream file(file_name.c_str(), std::ios::binary);
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
    ofstream file(file_name.c_str(), std::ios::binary);
    for(char ch : data) file.put(ch);
    file.close();
}

class Rail_Fence
{
    int min_, max_;
    INT_VECTOR key_v;
public:
    Rail_Fence(string num)
    {
        max_ = -1;
        min_ = 100;
        int temp;
        string copy = num;
        unordered_map<char, bool> validity_map;
        
        for(char ch : num)
        {
            if(!isdigit(ch))
                throw string("ValueError: Non - digit encountered in key");
            else if(validity_map[ch])
                throw string("ValueError: Repetetive digit encountered in key");
            else
                validity_map[ch] = true;
            temp = ch - 48;
            max_ = max(max_, temp);
            min_ = min(min_, temp);
            key_v.push_back(temp);
        }
        
        sort(copy.begin(), copy.end());
        for(int i=0; i<copy.length(); ++i)
        if( copy[i] != ('0' + i) )
            throw string("Non strict sequence encountered");
    }
    string encrypt_str(string plain)
    {
        int index;
        string en_msg = "";
        CHAR_VECTOR temp_v;
        CHAR_MATRIX mat;
        for(char ch : plain)
        {
            temp_v.push_back(ch);
            if(temp_v.size() == key_v.size())
            {
                mat.push_back(temp_v);
                temp_v.clear();
            }
        }
        if(!temp_v.empty())
        {
            while(temp_v.size() != key_v.size())
                temp_v.push_back(' ');
            mat.push_back(temp_v);
        }
        for(int i=min_; i<=max_; ++i)
        {
            index = get_index(i, key_v);
            for(int j=0; j<mat.size(); ++j)
                en_msg += mat.at(j).at(index);
        }
        return en_msg;
    }

    string decrypt_str(string en_msg)
    {
        int64_t index, msg_index=0;
        int64_t num_rows = en_msg.length() / key_v.size();
        string plain = "";
        CHAR_VECTOR temp_char_v(key_v.size(), ' ');
        CHAR_MATRIX mat(num_rows, temp_char_v);
        for(int i=min_; i<=max_; ++i)
        {
            index = get_index(i, key_v);
            for(int j=0; j<mat.size(); ++j)
                mat[j][index] = en_msg[msg_index++];
        }
        for(int i=0; i<mat.size(); ++i)
            for(int j=0; j<mat[i].size(); ++j)
                plain += mat[i][j];
        return plain;
    }
};

int main() try
{
    // Rail_Fence rf("3102");
    // string e = rf.encrypt_str("Hi my name is kartikei");
    // string d = rf.decrypt_str(e);
    // print("Encrypted:", e);
    // print("Decrypted:", d);
    
    string a = "Hi my name is kartikei";
    print(a);
    
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }


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