// Author: Kartikei Mittal
// Data Encryption Standard Algorithm

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

#define N_ROUNDS 16
#define BITS_PER_CHAR 8
// #define MAX_CHAR 2048
// #define MAX_BITS MAX_CHAR*BITS_PER_CHAR

#define cin std::cin
#define cout std::cout
#define cerr std::cerr
#define endl std::endl
#define string std::string
#define exception std::exception
#define vector std::vector

#define BOOL_VECTOR vector<bool>
#define INT_VECTOR vector<int>

// Initial Permutation
INT_VECTOR IP = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9,  1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// IP Inverse
INT_VECTOR inv_IP = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25
};


// Sub Key Generation

// PC1  64 -> 56 bit
INT_VECTOR PC1 = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10,  2, 59, 51, 43, 35, 27,
    19, 11,  3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14,  6, 61, 53, 45, 37, 29,
    21, 13,  5, 28, 20, 12,  4
}; 

// PC2 56 -> 48 bit
INT_VECTOR PC2 = {
    14, 17, 11, 24,  1,  5,
    3, 28, 15,  6, 21, 10,
    23, 19, 12,  4, 26,  8,
    16,  7, 27, 20, 13,  2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Left Shift Table
INT_VECTOR shift_bits = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// E 32 -> 48 bit
INT_VECTOR E = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1
};

// S-Box 8, 4, 16
vector<vector<vector<int>>> S_Box = {
	{  
		{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},  
		{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},  
		{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0}, 
		{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13} 
	},
	{  
		{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},  
		{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5}, 
		{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},  
		{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}  
	}, 
	{  
		{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},  
		{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},  
		{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},  
		{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}  
	}, 
	{  
		{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},  
		{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},  
		{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},  
		{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}  
	},
	{  
		{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},  
		{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},  
		{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},  
		{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}  
	},
	{  
		{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},  
		{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},  
		{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},  
		{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}  
	}, 
	{  
		{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},  
		{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},  
		{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},  
		{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}  
	}, 
	{  
		{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},  
		{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},  
		{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},  
		{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}  
	} 
};

// Permutation P
INT_VECTOR P = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25
};


void print() { cout<<endl; }
template<typename T, typename... Rest>
void print(T t, Rest... rs) { cout<<t<<" "; print(rs...); }

template <typename T>
T input(string prompt) { T t; cout<<prompt; cin>>t; return t; }

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

void write_file(string file_name, string data)
{
    ofstream file(file_name.c_str(), std::ios::binary);
    for(char ch : data) file.put(ch);
    file.close();
}

BOOL_VECTOR operator^(BOOL_VECTOR a, BOOL_VECTOR b)
{
    BOOL_VECTOR ans_;
    ans_.reserve(a.size());
    for(int i=0; i<a.size(); ++i)
        ans_[i] = a[i] ^ b[i];
    return ans_;
}

BOOL_VECTOR to_bits(int64_t a, const int size = BITS_PER_CHAR)
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

BOOL_VECTOR pad(BOOL_VECTOR in_, int by)
{
    BOOL_VECTOR out_ = in_;
    by = by - in_.size() % by;
    while(by--)
        out_.push_back(false);
    return out_;
}

vector<BOOL_VECTOR> split(BOOL_VECTOR data, int size)
{
    BOOL_VECTOR block;
    vector<BOOL_VECTOR> blocks;
    data = pad(data, size);
    for(int i=0; i<int(data.size() / size); ++i)
    {
        block.clear();
        for(int j=0; j<size; ++j)
            block.push_back(data[(i*size) + j]);
        blocks.push_back(block);
    }
    return blocks;
}

BOOL_VECTOR combine(vector<BOOL_VECTOR> blocks)
{
    BOOL_VECTOR data;
    for(int i=0; i<blocks.size(); ++i)
        for(int j=0; j<blocks[i].size(); ++j)
            data.push_back(blocks[i][j]);
    return data;
}

BOOL_VECTOR left_shift(BOOL_VECTOR data, int by)
{
    BOOL_VECTOR ans = data;
    for(int i = data.size()-1; i>=0; --i)
        data[i] = ((i - by) < 0)? ans[i-by+data.size()] : ans[i-by];
    return data;
}

BOOL_VECTOR f_function(BOOL_VECTOR in_, BOOL_VECTOR subKey)
{
    int row, col, num;
    BOOL_VECTOR expand(48, false),
        out_(in_.size(), false),
        temp(4, false),
        temp_(in_.size(), false);
    
    for(int i=0; i<48; ++i)
        expand[47-i] = in_[32-E[i]];
    
    expand = expand ^ subKey;

    int index = 0;
    for(int i=0; i<48; ++i)
    {
	   row = 2*expand[47-i] + expand[47-i-5];
        col = 8*expand[47-i-1] + 4*expandR[47-i-2] + 2*expandR[47-i-3] + expandR[47-i-4];
        num = S_Box[int(i/6)][row][col];
        out_[31-index] = temp[3];
        out_[31-index-1] = temp[2];
		out_[31-index-2] = temp[1];
		out_[31-index-3] = temp[0];
        index += 4;
    }

    temp_ = out_;
	for(int i=0; i<32; ++i)
		out_[31-i] = temp_[32-P[i]];
	return out_;
}

class EN_DE
{
    vector<BOOL_VECTOR> sub_keys;
public:
    EN_DE(BOOL_VECTOR key)
    {
        BOOL_VECTOR pc1(56, false),
            left(28, false),
            right(28, false),
            single_sub_key(56, false);

        sub_keys.reserve(N_ROUNDS);
        
        for(int i=0; i<56; ++i)
            pc1[55-i] = key[64 - PC1[i]];
        for(int i = 0; i<N_ROUNDS; ++i)
        {
            for(int j=28; j<56; ++j)
                left[j-28] = pc1[j];
            for(int j=0; j<28; ++j)
                right[j] = pc1[j];
            left = left_shift(left, shift_bits[i]);
            right = left_shift(right, shift_bits[i]);
            for(int j=28; j<56; ++j)
                pc1[j] = left[j-28];
            for(int j=0; j<28; ++j)
                pc1[j] = right[j];
            for(int j=0;  j<48; ++j)
                single_sub_key[47-j] = pc1[56 - PC2[j]];
            sub_keys[i] = single_sub_key;
        }
    }
    BOOL_VECTOR encrypt_block(BOOL_VECTOR block)
    {
        BOOL_VECTOR ip(block.size(), false),
            left(block.size(), false),
            right(block.size(), false),
            new_left(32, false),
            new_right(32, false),
            cipher(64, false);

        for(int i=0; i<block.size(); ++i)
            ip[block.size() - i - 1] = block[block.size() - IP[i]];
        
        for(int i=32; i<64; ++i)
            left[i-32] = ip[i];
        for(int i=0; i<32; ++i)
            right[i] = ip[i];
        
        for(int i=0; i<16; ++i)
        {
            new_left = right;
            right = left ^ f_function(right, sub_key[i]);
            left = new_left;
        }
        
        for(int i=0; i<32; ++i)
            cipher[i] = left[i];
        for(int i=32; i<64; ++i)
            cipher[i] = right[i-32];
        
        ip = cipher;
        for(int i=0; i<64; ++i)
            cipher[63-i] = ip[64-IP1[i]];
        
        return cipher;
    }
};
void encrypt()
{
    
    string in_data = read_file(input<string>("Enter message file name: ")),
        out_file = input<string>("Enter output file name: "),
        key = read_file(input<string>("Enter key file name: ")),
        out_data;
    
    EN_DE obj(key);
    BOOL_VECTOR data = to_bits(in_data);
    vector<BOOL_VECTOR> blocks = split(data, 64);

    for(int i=0; i<blocks.size(); ++i)
        blocks[i] = EN_DE.encrypt_block(blocks[i]);
    out_data = from_bits(combine(blocks));
    write_file(out_file, out_data);
}

int main()
{
    int choice;
    print("Welcome to CPP implementation of DES Algorithm");
    do
    {
        print("\t1. Encrypt");
        print("\t2. Decrypt");
        print("\t0. Exit");
        choice = input<int>("\tChoose: ");    
        if(choice == 1)
            encrypt();
        else if(choice == 2)
            decrypt();
        else if(choice == 3)
            break;
        else
            print("ERROR: Invalid Choice");
    }while(choice != 0);
    print("Byee");
    return 0;
}