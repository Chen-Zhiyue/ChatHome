#include<sys/socket.h>
#include<sys/epoll.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<map>
#include<iostream>


const int MAX_CONN = 1024;

struct Client{
	int sockfd;
	std::string name;
};


int main()
{
	std::cout << "[*] Server is ready!!!"<<std::endl;
	int epfd = epoll_create1(0);
	if(epfd < 0)
	{

	}
		
	int sockfd = socket(AF_INET , SOCK_STREAM , IPPROTO_IP); 

	if(sockfd < 0)
	{
		std::cout << "socket failed !!!"<<std::endl; 
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//addr.sin_addr.s_addr = inet_addr("192.168.70.130");
	//std::cout << addr.sin_addr.s_addr <<std::endl;
	addr.sin_port = htons(9999);

	int ret = bind(sockfd , (struct sockaddr *)&addr , sizeof(addr));
	
	if(ret < 0)
	{

	}

	ret = listen(sockfd , 5);

	if(ret < 0)
	{

	}

	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = sockfd;

	ret = epoll_ctl(epfd , EPOLL_CTL_ADD , sockfd , &ev);
	if(ret)
	{

	}

	std::map<int , Client> clients;


	while(true)
	{
		struct epoll_event events[MAX_CONN];
		int n = epoll_wait(epfd , events , MAX_CONN , -1);
		
		if(n < 0)
		{}
		
//		std::cout <<"[*] ready people: "<<n<<std::endl;
		for(int i = 0; i < n ;i++)
		{
			int fd = events[i].data.fd;
//			std::cout <<"[*] sockfd: "<<sockfd<<std::endl;
//			std::cout <<"[*] fd: "<<fd <<std::endl;
			if(fd == sockfd)
			{
				struct sockaddr_in client_addr;
				socklen_t client_addr_len = sizeof(client_addr);
				int client_sockfd = accept(sockfd , (struct sockaddr *)&client_addr ,&client_addr_len);
				if(client_sockfd < 0)
				{
					std::cout << " Accept failed !!!  "<<std::endl;	
				}

				ev.events = EPOLLIN;
				ev.data.fd = client_sockfd;
				ret = epoll_ctl(epfd , EPOLL_CTL_ADD,client_sockfd , &ev);
				if(ret < 0)
				{

				}
				
				Client client;
				client.sockfd = client_sockfd;
				client.name = "";
				clients[client_sockfd] = client;
			}else
			{
				char buffer[1024];
				int nums = read(fd , buffer , 1024);

				if(nums < 0)
				{
					
				}else if(0 == nums)
				{
					close(fd);
					epoll_ctl(epfd , EPOLL_CTL_DEL , fd ,NULL);
					clients.erase(fd);
				}else
				{
					std::string message(buffer , nums);
					if(clients[fd].name.empty()){
						clients[fd].name = message;
						//std::cout<<"[*] "<<message<<std::endl;
					}else{
						std::string name = clients[fd].name;

						for(auto &c : clients){
				if(c.first != fd){
					write(c.first , ('[' + name + ']' + ": "+message).c_str(),message.size() + name.size() + 4 );

					}
						}
					}
				}
			}
		}

	}
	close(epfd);
	close(sockfd);

//	std::cout << "Success !!!"<<std::endl;
	return 0;
}
