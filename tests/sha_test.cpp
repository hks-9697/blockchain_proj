#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;
#define INT_MAX 750000000
#include <openssl/sha.h>
string sha256(const string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

string proof_of_work(string s1,string data,int k)
{
	string str;
    for (long long i = 0; i < INT_MAX; ++i)
    {
        stringstream ss;
        ss << i;
        str = ss.str();
        string check=s1+str+data;
        string hash=sha256(check);
        int flag=0;
        for (int i = 0; i < k; ++i)
        {
            if(hash[i]!='0')
            {
                flag=1;
            }
        }
        if (flag==0)
        {
            break;
        }

    }
   // cout<<endl<<s1+str+data<<endl;
   // cout<<sha256(s1+str+data)<<endl;
    return (str);
}

int main() {
    string s1="1234567890_1";
    int k=5;
    cout << sha256("1234567890_1") << endl;
    string data;
    cin>>data;
    string str;
    str=proof_of_work(s1,data,k);
    cout<<endl<<s1+str+data<<endl;
    cout<<sha256(s1+str+data)<<endl;
    return 0;
}
