// Author: Kartikei Mittal
// Hill Cipher
#include <bits/stdc++.h>
    using namespace std;
#define LONG int64_t
#define MAT vector<vector<int64_t>>
#define CHAR_SET_SIZE 26
#define LOWER_LIMIT 65

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

LONG mod_plain(LONG a)
{
    a %= CHAR_SET_SIZE;
    if(a < 0)
        a += CHAR_SET_SIZE;
    return a;
}

LONG mod_mul_inv(LONG a)
{
    LONG ans=1, num = mod_plain(a);
    while((num*ans) % CHAR_SET_SIZE != 1)
    {
        cout<<"\nx: "<<ans<<" num*ans: "<<num*ans<<" mod 26: "<<(num*ans) % CHAR_SET_SIZE<<endl;
        if(ans == CHAR_SET_SIZE -1)
            throw string(
                "Invalid key, modular multiplicative inverse does not exist"
            );
        ++ans;
    }
    cout<<"\nModular mul inverse: "<<ans<<endl;
    return ans;
}

class Matrix
{
    MAT mat;
public:
    Matrix(){ mat.clear(); }
    Matrix(int a1)
    {
        vector<LONG> a(a1, 0.0);
        for(int i=0; i<a1; ++i)
            mat.push_back(a);
    }
    Matrix(int a1, int a2)
    {
        vector<LONG> a(a2, 0.0);
        for(int i=0; i<a1; ++i)
            mat.push_back(a);
    }
private:
    static MAT co_factor(MAT mat1, int p, int q, int size1)
    {
        Matrix temp(size1);
        int i = 0, j = 0;
        for(int row = 0; row<size1; ++row)
        for(int col = 0; col<size1; ++col) 
            if(row != p && col != q) 
            { 
                temp.mat[i][j++] = mat1[row][col];
                if(j == size1 - 1) {
                    j = 0; i++; 
                }
            }
        return temp.mat;
    }
    static LONG determinant(MAT mat, int size1)
    {
        LONG D = 0;
        if(size1 == 1)
            return mat[0][0];
        int sign = 1;
        for(int i = 0; i<size1; ++i, sign *= -1)
            D += sign * mat[0][i] * determinant(
                co_factor(mat, 0, i, size1), size1 - 1
            );
        return D;
    }
public:
    LONG size() { return mat.size(); }
    bool check_square()
    {
        int a = mat.size();
        for(vector<LONG> i : mat)
            if(i.size() != mat.size())
                return false;
        return true;
    }
    LONG determinant() { 
        cout<<"\nDeterminant: "<<determinant(mat, mat.size())<<endl;
        return determinant(mat, mat.size()); }
    Matrix adjoint()
    {
        if(!check_square())
            throw string("Non-square matrix adjoint not possible.");
        int sign = 1;
        Matrix adj(mat.size());
        if(mat.size() == 1) 
        { 
            adj.mat[0][0] = 1; 
            return adj; 
        }
        for(int i=0; i<mat.size(); ++i) 
        for(int j=0; j<mat.size(); ++j) 
        {
            sign = ((i+j)%2)? -1: 1;
            adj.mat[j][i] = sign*determinant(
                co_factor(mat, i, j, mat.size()), mat.size()-1
            ); 
        }
        cout<<"\nAdjoint\n";
        adj.show();
        return adj;
    }
    Matrix inverse()
    {
        if(!check_square())
            throw string("Non-square matrix, inverse not possible.");
        Matrix inv(mat.size());
        LONG det = mod_mul_inv(determinant(mat, mat.size()));
        if(det == 0)
            return inv;
        MAT adj = adjoint().mat;
        for(int i=0; i<mat.size(); ++i) 
        for(int j=0; j<mat.size(); ++j)
            inv.mat[i][j] = (mod_plain(adj[i][j]) * det)%CHAR_SET_SIZE;
        return inv;
    }
    void show()
    {
        for(int i=0; i<mat.size(); ++i)
        {
            for(int j=0; j<mat[i].size(); ++j)
                cout<<mat[i][j]<<" ";
            cout<<endl;
        }
    }
    vector<LONG> unroll()
    {
        vector<LONG> ans;
        for(vector<LONG> i : mat)
            for(LONG ele : i)
                ans.push_back(ele);
        return ans;
    }
    friend Matrix operator * (Matrix , Matrix );
    friend class Hill_Cipher;
    // friend int main();
};
Matrix operator * (Matrix a, Matrix b)
{
    MAT A = a.mat;
    MAT B = b.mat;
    if(A[0].size() != B.size())
        throw string("Incompatible size for matrix multiplication.");
    Matrix ans(A.size(), B[0].size());
    MAT C = ans.mat;
    for(int i=0; i<A.size(); ++i)
    for(int j=0; j<B[0].size(); ++j)
    for(int k=0; k<B.size(); ++k)
        C[i][j] += A[i][k] * B[k][j];
    ans.mat = C;
    return ans;
}

class Hill_Cipher
{
    Matrix KeyMatrix;
    Matrix InvKeyMatrix;
public:
    Hill_Cipher(string key_)
    {
        // int x = 0, y = 0, p = 0;
        // unordered_map<char, bool> validity_map;
        KeyMatrix = Matrix(2);
        // for(int i=0; i<key_.length(); ++i)
        //     if(!isalpha(key_[i]))
        //         throw "Invalid key!!\n";
        //     else
        //         key_[i] = toupper(key_[i]);
        // for(char  ch : key_)
        //     if(!validity_map[ch])
        //     {
        //         if(ch == 'I' || ch == 'J')
        //             KeyMatrix.mat[x][y] = (int)('I'-LOWER_LIMIT), validity_map['I'] = true;     
        //         else
        //             KeyMatrix.mat[x][y] = (int)(ch-LOWER_LIMIT), validity_map[ch] = true;
        //         ++y;
        //         if(y == KeyMatrix.size())
        //             y = 0, ++x;
        //     }
        // for(int i=x; i<KeyMatrix.size(); ++i)
        //     for(int j = (i == x)? y: 0; j<KeyMatrix.size(); ++j, ++p)
        //         if(!validity_map[p+'A'])
        //             KeyMatrix.mat[i][j] = (int)(p+'A'-LOWER_LIMIT);
        //         else
        //             --j;
        KeyMatrix.mat[0] = {7, 3};
        KeyMatrix.mat[1] = {2, 5};
        if(KeyMatrix.determinant() == 0)
            throw "Invalid Key, determinat zero";
        InvKeyMatrix = KeyMatrix.inverse();
        // cout<<"\n-------------------------------------------------\n";
        // KeyMatrix.show();
        // cout<<"Det: "<<KeyMatrix.determinant()<<endl;
        // cout<<"\n-------------------------------------------------\n";
        // KeyMatrix.adjoint().show();
        // cout<<"\n-------------------------------------------------\n";
        // KeyMatrix.inverse().show();
        // cout<<"\n-------------------------------------------------\n";
    }

    void encrypt_file(string input_file, string output_file)
    {
        int size = KeyMatrix.size(), extra, p=0;
        vector<LONG> u_data, cipher;
        Matrix data_matrix;
        
        input_file = read_file(input_file);
        for(char ch : input_file)
            if(isalpha(ch))
                u_data.push_back((LONG)(toupper(ch)-LOWER_LIMIT));
        extra = size - (u_data.size()%size);
        while(extra--)
            u_data.push_back((LONG)('X'-LOWER_LIMIT));
        
        for(int i=0; i<u_data.size(); i += size)
        {
            int j = size;
            vector<LONG> temp;
            temp.clear();
            for(int j=0; j<size; ++j)
                temp.push_back(u_data[i+j]);
            data_matrix.mat.push_back(temp);
        }
        cipher = (data_matrix * KeyMatrix).unroll();
        for(int i=0; i<input_file.length(); ++i)
            if(isalpha(input_file[i]))
                input_file[i] = (char)((cipher[p++]%CHAR_SET_SIZE)+LOWER_LIMIT);
        cout<<"\nCipher\n";
        for(int i : cipher)
            cout<<mod_plain(i)<<" ";
        write_file(output_file, input_file);
    }
    void decrypt_file(string input_file, string output_file)
    {
        int size = KeyMatrix.size(), p=0;
        vector<LONG> u_data, plain;
        Matrix data_matrix;

        input_file = read_file(input_file);
        for(char ch : input_file) if(isalpha(ch))
            u_data.push_back((LONG)(ch-LOWER_LIMIT));
        
        for(int i=0; i<u_data.size(); i += size)
        {
            vector<LONG> temp;
            int j = size;
            for(int j=0; j<size; ++j)
                temp.push_back(u_data[i+j]);
            data_matrix.mat.push_back(temp);
        }
        plain = (data_matrix * InvKeyMatrix).unroll();
        for(int i=0; i<input_file.length(); ++i)
            if(isalpha(input_file[i]))
                input_file[i] = (char)((plain[p++]%CHAR_SET_SIZE)+LOWER_LIMIT);
        write_file(output_file, input_file);
    }
};

int main() try
{ 
    // Hill_Cipher hl("fird");
    // hl.encrypt_file("a.txt", "b.txt");
    // hl.decrypt_file("b.txt", "c.txt");
    cout<<"A"<<mod_mul_inv(1);
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
