// Author: Kartikei Mittal
#include<iostream>
#include<vector>
#include<exception>
#include<fstream>

#define cin std::cin
#define cout std::cout
#define cerr std::cerr
#define endl std::endl
#define vector std::vector
#define string std::string
#define ifstream std::ifstream
#define exception std::exception

#define DELIMITER ' '
#define INT_VECTOR vector<int>
#define CHAR_VECTOR vector<char>
#define INT_MATRIX vector<vector<int>>
#define CHAR_MATRIX vector<vector<char>>

void print__(auto a) { cout<<a; }
void print__(const string& a) { cout<<a; }
template <
    template <typename, typename...> class ContainerType,
    typename ValueType,
    typename... Args
>
void print__(const ContainerType<ValueType, Args...>& c)
{
    cout<<"[ ";
    for(const ValueType& v : c)
    {
        print__(v);
        cout<<", ";
    }
    cout<<"]";
}
void print() { cout<<endl; }
template<typename T, typename... Rest>
void print(T t, Rest... rs) {
    print__(t);
    cout<<" ";
    print(rs...); 
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------

bool file_exist(const string& name){
    if (FILE *file = fopen(name.c_str(), "r")) { fclose(file); return true; }
    return false;
}

class Graph_Matrix
{
    uint64_t N_nodes, N_edges;
    CHAR_VECTOR nodes;
    INT_MATRIX mat;

    void from_to_paths(char root, char paraent, CHAR_VECTOR& to__)
    {
        int index = get_index(root);
        if(index == -1)
            throw string("ERROR: root not found.");
        for(int i=0; i<mat[index].size(); ++i)
            if(mat[index][i] == 1 && nodes[i] != paraent)
            {
                to__.push_back(nodes[i]);
                from_to_paths(nodes[i], root, to__);
                if(paraent == DELIMITER)
                    to__.push_back(DELIMITER);
            }
    }
public:
    Graph_Matrix(string in_file)
    {
        char ch;
        char a, b;
        if(!file_exist(in_file))
            throw string("File to read do not exist");
        ifstream in_data(in_file.c_str());
        in_data>>N_nodes>>N_edges;

        INT_VECTOR row(N_nodes, 0);
        INT_MATRIX aaa(N_nodes, row);
        mat = aaa;

        for(int i=0; i<N_nodes; ++i)
        {
            in_data>>ch;
            nodes.push_back(ch);
        }

        for(int i=0; i<N_edges; ++i)
        {
            in_data>>a>>b;
            mat[a - 'A'][b - 'A'] = 1;
            mat[b - 'A'][a - 'A'] = 1;
        }
    }

    void __print__()
    {
        print("Adjancey Matrix:");
        cout<<"  ";
        print(nodes);
        for(int i=0; i<mat.size(); ++i)
        {
            cout<<nodes[i]<<" ";
            print(mat[i]);
        }
    }

    int get_index(char a)
    {
        for(int i=0; i<nodes.size(); ++i)
            if(nodes[i] == a)
                return i;
        return -1;
    }

    CHAR_MATRIX from_paths(char root)
    {
        CHAR_VECTOR ans, ans_;
        from_to_paths(root, DELIMITER, ans);
        CHAR_MATRIX ans__;
        for(char ch : ans)
        {
            if(ch == DELIMITER)
            {
                ans__.push_back(ans_);
                ans_.clear();
            }
            else
                ans_.push_back(ch);
        }
        return ans__;
    }

    void all_paths()
    {
        for(char ch : nodes)
        {
            cout<<"Paths from "<<ch<<" :";
            print(from_paths(ch));
        }
    }

};

int main() try
{
    print("Hello World");

    string name;
    cout<<"Enter file name: ";
    cin>>name;
    Graph_Matrix gm(name);
    gm.__print__();
    gm.all_paths();
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }
