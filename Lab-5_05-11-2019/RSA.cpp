#include <bits/stdc++.h>

#define cin std::cin
#define cout std::cout
#define cerr std::cerr
#define endl std::endl
#define vector std::vector
#define string std::string
#define exception std::exception

#define BOOL_VECTOR vector<bool>
#define INT_VECTOR vector<int64_t>
#define BIG_INT uint64_t

#define PRIME_LIST_SIZE 545
#define ALPHABET string("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz0123456789~`!@#$^&*()_+=-{}|:<>?[];', ./")

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
    cout<<"[ ";
    for(const ValueType& v : c) { print__(v); cout<<", "; }
    cout<<"]"<<endl;
}
template<typename T, typename... Rest>
void print(T t, Rest... rs) { print__(t); cout<<" "; print(rs...); }

string read_file(string file_name)
{
    char temp; string data = "";
    std::ifstream file(file_name.c_str(), std::ios::binary);
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
    std::ofstream file(file_name.c_str(), std::ios::binary);
    for(char ch : data) file.put(ch);
    file.close();
}

// GCD
BIG_INT gcd(BIG_INT n1, BIG_INT n2)
{
    BIG_INT tmp;
    while (n2 != 0) {
        tmp = n1;
        n1 = n2;
        n2 = tmp % n2;
    }
    return n1;
}

BIG_INT __random__(BIG_INT range__)
{
    std::mt19937 rng;
    rng.seed(std::random_device()());
    rng.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, range__);
    return dist(rng);
}

//Fast Modular Exponentiation
BIG_INT mod_Exp(BIG_INT b, BIG_INT e, BIG_INT m)
{
    BIG_INT remainder=0, x = 1;
    while (e!=0)
    {
        remainder = e%2;
        e /= 2;
        if(remainder == 1)
            x = (x*b) % m;
        b = (b * b) % m; // New base equal b^2 % m
    }
    return x;
}

//Sieve Of Eratosthenes
INT_VECTOR getSieveOfEratosthenes(BIG_INT max)
{
    BOOL_VECTOR primes(max, true);
    
    for(BIG_INT i = 3; i < max ; i+=2)
        if(primes[i]) 
            for(int j = i*i; j<max; j += i)
                primes[j] = false;
    
    INT_VECTOR ret;
    ret.reserve(primes.size()/2);
    
    ret.push_back(2);
    for(BIG_INT i=3; i<max; i+=2)
        if(primes[i])
            ret.push_back(i);
    return ret;
}


class KEY
{
    BIG_INT publicK, privateK, modN;
public:
    KEY() { ; }
    KEY(BIG_INT p, BIG_INT mN, bool encrypt_decrypt)
    {
        publicK = privateK = 0;
        modN = mN;
        (encrypt_decrypt)? publicK = p : privateK = p;
    }
    KEY(BIG_INT puK, BIG_INT prK, BIG_INT mN):
        publicK(puK), privateK(prK), modN(mN) {;}

    static vector<KEY> generate_keys()
    {
        bool success = false;
        int attempt = 0;
        KEY k;
        vector<KEY> kList;
        INT_VECTOR eList;
        INT_VECTOR primes = getSieveOfEratosthenes(PRIME_LIST_SIZE);

        BIG_INT prime1, prime2, modulasN, publicK, privateK, totient;

        // Every possible prime number combination
        for(int ii = 0; ii<primes.size(); ++ii)
        for(int jj = 0; jj<primes.size(); ++jj)
        {
            if(ii == jj)
                continue;
            prime1 = primes[ii];
            prime2 = primes[jj];

            modulasN = prime1 * prime2;
            totient = (prime1-1) * (prime2-1) ;

            for(BIG_INT e=0; e<totient; ++e)
                if(gcd(e, totient) == 1)
                    eList.push_back(e);

            for(BIG_INT i_ = 0; i_<eList.size(); ++i_)
            {
                publicK = eList[i_];
                // publicK * privateK === 1 mod (totient)
                double o=2.0, temp=0;
                do{
                    temp = (1.0 + o * double(totient)) / double(publicK);
                    ++o;
                } while(floor(temp) != ceil(temp) && o < totient);
                privateK = BIG_INT(temp);

                success = true;
                for(char i : ALPHABET)
                    if(
                        i != char(
                            mod_Exp(mod_Exp(i, publicK, modulasN),privateK,modulasN)
                        ) ||
                        modulasN > 256 )
                    {
                        success = false;
                        break;
                    }
                if(success)
                {
                    KEY _k_(publicK, privateK, modulasN);
                    if(std::find(kList.begin(), kList.end(), _k_) == kList.end())
                        kList.push_back(_k_);
                }
                ++attempt;

                if(kList.size() == 2000)
                {
                    print("Prime numbers:", primes.size());
                    print("Valid keys:", kList.size());
                    print("Total tested:", attempt);
                    return kList;
                }
            }
        }
        return kList;
    }
    friend std::ostream& operator<<(std::ostream& os, KEY const& obj);
    friend BIG_INT RSA_encrypt(BIG_INT, KEY& );
    friend BIG_INT RSA_decrypt(BIG_INT, KEY& );
    friend bool operator==(KEY k1, KEY k2);
};
std::ostream& operator<<(std::ostream& os, KEY const& obj)
{
    // cout<<"Public Key:"<<obj.publicK<<" Private Key:"<<obj.privateK<<" Modulas N:"<<obj.modN;
    print("Public Key:", obj.publicK, "Private Key:", obj.privateK, "Modulas N:", obj.modN);
    return os;
}
bool operator==(KEY k1, KEY k2)
{
    if(
        k1.publicK == k2.publicK &&
        k1.privateK == k2.privateK &&
        k1.modN == k2.modN
    )
        return true;
    return false;
}

BIG_INT RSA_encrypt(BIG_INT data, KEY& k) {
    return mod_Exp(data, k.publicK, k.modN);
}

BIG_INT RSA_decrypt(BIG_INT data, KEY& k) {
    return mod_Exp(data, k.privateK, k.modN);
}

string RSA_encrypt(string data, KEY& k)
{
    string ans = "";
    for(char ch : data)
        ans += char(RSA_encrypt(ch, k));
    return ans;
}

string RSA_decrypt(string data, KEY& k)
{
    string ans = "";
    for(char ch : data)
        ans += char(RSA_decrypt(ch, k));
    return ans;
}

int main() try
{
    print("Hello World!");
    vector<KEY> keys = KEY::generate_keys();
    KEY k = keys[__random__(keys.size() - 1)];
    print(k);
    string en = RSA_encrypt("Hello World", k);
    print("**", en, "**");
    string de = RSA_decrypt(en, k);
    print(de);

    // int choice;
    // BIG_INT p, mN;
    // string inFile, outFile, data;

    // cout<<"------------------** Welcome to RSA **------------------"<<endl
    //     <<"1. Encrypt"<<endl
    //     <<"2. Decrypt"<<endl
    //     <<"3. Generate key"<<endl
    //     <<"0. Exit"<<endl
    //     <<"\t Please choose: ";
    // cin>>choice;

    // if(choice == 0)
    //     cout<<"Byeeeeee";
    // else if (choice == 1)
    // {
    //     cout<<"Enter input file name: "; cin>>inFile;
    //     cout<<"Enter output file name: "; cin>>outFile;
    //     data = read_file(inFile);
    //     cout<<"Enter PUBLIC KEY(public key, modulas N): "; cin>>p>>mN;
    //     KEY ke(p, mN, true);
    //     write_file(outFile, RSA_encrypt(data, ke));
    // }
    // else if (choice == 2)
    // {
    //     cout<<"Enter input file name: "; cin>>inFile;
    //     cout<<"Enter output file name: "; cin>>outFile;
    //     data = read_file(inFile);
    //     cout<<"Enter PRIVATE KEY(private key, modulas N): "; cin>>p>>mN;
    //     KEY kd(p, mN, false);
    //     write_file(outFile, RSA_decrypt(data, kd));
    // }
    // else if(choice == 3)
    // {
    //     vector<KEY> keys = KEY::generate_keys();
    //     KEY k = keys[__random__(keys.size() - 1)];
    //     cout<<"Key: "<<k;
    // }
    // else
    //     cerr<<"Invalid choice exiting.";
    return 0;
}
catch(string &e) { cerr<<"Error: "<<e<<endl; }
catch(exception &e) { cerr<<e.what()<<endl; }
catch(...) { cerr<<"\n\nSomething Wrong\n\n"<<endl; }
