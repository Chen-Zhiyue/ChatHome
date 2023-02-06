#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

#include<iostream>
#include<string.h>

#include<pthread.h>

#define MSGSIZE 1024
using namespace std;


char msgbuf[MSGSIZE];

void * sendMsg(void * arg)
{
	int sockfd = *(int *)(arg);

	while(true)
	{
		scanf("%s",msgbuf);
		if(!strcmp(msgbuf , "QUIT\0") || !strcmp(msgbuf , "quit\0"))
		{
			close(sockfd);
			exit(0);
		}
		send(sockfd , msgbuf , strlen(msgbuf),0);
	}

}

void * recvMsg(void * arg)
{
	//int ret = -1;
	int sockfd = *(int *)(arg);
	
	char msg[MSGSIZE];

	while(true)
	{
		int len = recv(sockfd , msg , sizeof(msg) - 1,0);
		if(len == -1)
		{
			return nullptr;
		}
		msg[len] = '\0';
		printf("%s\n",msg);
	}

	return nullptr;

}

int main()
{
	int sockfd = socket(AF_INET,SOCK_STREAM,0);

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9999);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	//int ret = 0;
	int ret = connect(sockfd , (struct sockaddr *)&addr ,sizeof(struct sockaddr_in));
	if(ret >= 0)
	{
		std::cout << "[*] Welcome to your private chat home, Please put in your name: ";
	}else
	{
		std::cout << "[*] Connect error !"<< std::endl;
	}
	
	// thread
	pthread_t sendT;
	pthread_t recvT;

	ret = pthread_create(&sendT,NULL,sendMsg,(void *)&sockfd);
	if(ret)
	{
		std::cout << "[*] Thread Send failed!"<<std::endl;
	}	

	ret = pthread_create(&recvT,NULL,recvMsg,(void *)&sockfd);

	if(ret)
	{
		std::cout << "[*] Thread Recv failed!"<<std::endl;
	}

	pthread_join(sendT,NULL);
	pthread_join(recvT,NULL);

	close(sockfd);

	
	return 0;
	
}
