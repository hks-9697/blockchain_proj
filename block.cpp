#include <iostream>
#include <bits/stdc++.h>
#include "crypt.cpp"
class block{
	public:
	long long no;
	string nonce;
	string data;
	string prev;
	string hash;
	block()
	{
		no=1;
		
		data="";
		prev="0000000000000000000000000000000000000000000000000000000000000000";
		string s1;
		stringstream ss;
        ss << no;
        s1=ss.str();
        nonce=proof_of_work(s1,data+prev,4);
		hash=sha256(s1+nonce+data+prev);
	}
	~block()
	{}

	void get_data(long long n,string s1,string pre)
	{
		no=n;
		
		data=s1;
		prev=pre;
		string sn;
		stringstream ss;
        ss << no;
        sn=ss.str();
		nonce=proof_of_work(sn,s1+pre,4);
		hash=sha256(sn+nonce+data+prev);

	}
};
class block_chain{
	public:
	std::vector<block> chain;
	int size;
	block_chain(){
		size=0;
	}
	void add_transaction(string data)
	{
		block blk;
		if(size==0)
		{
			blk.get_data(size+1,data,"0000000000000000000000000000000000000000000000000000000000000000");
			size++;
			chain.push_back(blk);
		}
		else
		{
			blk.get_data(size+1,data,chain[size-1].hash);
			size++;
			chain.push_back(blk);
		}
	}
	static int verify(block_chain o)
	{
		int n;
		n=o.get_size();
		//string tp="0000000000000000000000000000000000000000000000000000000000000000";
		for (int i = n-1; i >=0; --i)
		{
			string nonce=o.chain[i].nonce;
			string data=o.chain[i].data;
			string prev=o.chain[i].prev;
			string hash=o.chain[i].hash;
			stringstream ss;
			ss<<i+1;
			string n=ss.str();
			if(i!=0)
		{
		if(hash==sha256(n+nonce+data+prev)&&o.chain[i-1].hash==prev)
		{
			
		}
		else{return 0;}
	}
	else
	{
		if(hash==sha256(n+nonce+data+prev)&&"0000000000000000000000000000000000000000000000000000000000000000"==prev)
		{
			
		}
		else
		{
			return 0;
		}
	}


		}

		return(1);
	}
	void add_block(string nonce,string data,string prev,string hash)
	{
		block blk;
		blk.nonce=nonce;
		blk.data=data;
		blk.prev=prev;
		blk.hash=hash;
		stringstream ss;
		cout<<data;
		ss << size+1;
		string n=ss.str();
		if(size!=0)
		{
		if(hash==sha256(n+nonce+data+prev)&&chain[size-1].hash==prev)
		{
			if(size<=chain.size())
			{
			size++;
			chain.push_back(blk);
			}
			else
			{
				chain[size++]=blk;
			}
		}
	}
	else
	{
		if(hash==sha256(n+nonce+data+prev)&&"0000000000000000000000000000000000000000000000000000000000000000"==prev)
		{
			if(size<=chain.size())
			{
			size++;
			chain.push_back(blk);
			}
			else
			{
				chain[size++]=blk;
			}
		}
	}
	}

	void drop_chain()
	{
		chain.clear();
		size=0;
	}
	
	int get_size()
	{
		return size;
	}

};



