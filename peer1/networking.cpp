#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<iostream>
#include<bits/stdc++.h>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include<fstream>
#include<thread>
#include "block.cpp"
#define peers 2
using namespace std;
class Ser{
	public:
	int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt;
    int addrlen;
    char buffer[1024] ;
    std::string hello = "Hello from server";

		Ser()
		{
			opt = 1;
			addrlen = sizeof(address);
		}


		void link_my_socket(int port)
		{


			// Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(0);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,&opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
      

			// Forcefully attaching socket to the port 8080
   			 if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
  			  {
        		perror("bind failed");
      			  exit(EXIT_FAILURE);
    			}
    		if (listen(server_fd, 3) < 0)
    			{
       			 perror("listen");
       			 exit(EXIT_FAILURE);
    			}
		}


	void stop()
	{
		close(server_fd);
	}

	int connect()
	{
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    	return new_socket;
	}

	void snd(string str,int sok)
	{
		send(sok , str.c_str() , str.length() , 0 );
    	
	}
	string rcv(int sok)
	{
		valread = read( sok , buffer, 1024);
   		 //printf("%s\n",buffer );
		string str=buffer;
		return str;
	}

};

class cli{
public:

	struct sockaddr_in address;
    int sock , valread;
    struct sockaddr_in serv_addr;
    
    char buffer[1024];

    cli()
    {
    	sock = 0;
    	buffer[1024] = {0};
    	 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        printf("\n Socket creation error \n");
        
    	}
  

   		 memset(&serv_addr, '0', sizeof(serv_addr));
    }

    void link_my_socket(int port)
    {
    	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
      

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        
    }
    }

    void Connect()
    {
    	 if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
       
    }

    }

    void snd(string str)
	{
		send(sock , str.c_str() , str.length() , 0 );
    	
	}
	string rcv()
	{
		valread = read( sock , buffer, 1024);
   		 //printf("%s\n",buffer );
		string str=buffer;
		return str;
	}

};

class connection{
	public:
		int size;
		Ser server;
		std::vector<cli> clients;
        std::vector<int> sockets;
        std::unordered_map<string,string> received_broadcasts;
        const string reply_keyword="reply:";
        const string query_keyword="query:";
        const string mine_keyword="mine:";
		connection()
		{
			size=0;
		}
        void init_s()
        {   
            
            fstream s("myport.txt");
            string t;
            s>>t;
            int x;
            x=stoi(t);
            cout<<"server <created> : "<<x<<endl;
            server.link_my_socket(x);
            for (int i = 0; i < peers; ++i)
            { 
                int x=server.connect();
                sockets.push_back(x);
                size++;
            }
            cout<<"hello"<<endl;

        }

        void init_c()
        {
            fstream s("neighbours.txt");
            string t;
            
            for (int i = 0; i < peers; ++i)
            {
                cli c;
                s>>t;
                int x;
                x=stoi(t);
                cout<<"Client (created) : "<<x<<endl;
                
                c.link_my_socket(x);
                clients.push_back(c);

            }
        }

        void initiate_connection()
        {
            
            std::thread t1(&connection::init_s,this);
           // sleep(1);
            std::thread t2(&connection::init_c,this);
           
            sleep(10);
            cout<<"sleep_done"<<endl;
            std::vector<thread> threads;
            for (int i = 0; i < peers; ++i)
            {
               threads.push_back(std::thread (&connection::connect_client,this,i));
              
            }
             t1.join();
            t2.join();
            cout<<"connetion initiated\n";

            for (int i = 0; i < peers; ++i)
            {
               threads[i].join();
            }

        }

        void connect_client(int i)
        {
            
            clients[i].Connect();
            cout<<"client "<<i<<" connected"<<endl;
            
        }

        void broadcast(string msg)
        {
            for (int i = 0; i < peers; ++i)
            {
                server.snd(msg,sockets[i]);
            }
        }


};
connection a;
block_chain my_chain;
int stop;

void miner(string data)
{
    my_chain.add_transaction(data);
    // std::thread t1(&block_chain::add_transaction,&my_chain,data);
    // t1.detach();
    cout<<"mined:"<<my_chain.chain[my_chain.size-1].hash<<endl;
}
void run_client(int i)
{
	while(1)
	{
    		string str=a.clients[i].rcv();
            
            if(!str.compare(0,a.mine_keyword.size(),a.mine_keyword))
            {
                miner(str.substr(5));
            }
    		
    }
    	
}

    

int main(int argc, char const *argv[])
{
       
    a.initiate_connection();
    cout<<"complete connection"<<endl;
    std::vector<thread> client_threads;
    
    for (int i = 0; i < peers; ++i)
    {
        client_threads.push_back(std::thread(&run_client,i));
    }
    int select;
    while(1)
    {
        cout<<"1.john,2.asdf"<<endl;
        cin>>select;
        switch(select)
        {
            case 1:a.broadcast(a.mine_keyword+"john");miner("john");break;
            case 2:a.broadcast(a.mine_keyword+"asdf");miner("asdf");break;
            

        }
    }


    
    
    for (int i = 0; i < peers; ++i)
    {
        client_threads[i].join();
    }
}
