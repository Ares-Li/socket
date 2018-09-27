#include <WINSOCK2.H>   
#include <stdio.h>     

#define PORT           5150    
#define MSGSIZE        1024    

#pragma comment(lib, "ws2_32.lib")      

int main()
{
	WSADATA wsaData;
	SOCKET sListen;
	SOCKET sClient;
	SOCKADDR_IN local;
	SOCKADDR_IN client;
	char szMessage[MSGSIZE];
	int ret;
	int iaddrSize = sizeof(SOCKADDR_IN);
	WSAStartup(0x0202, &wsaData);

	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY); 
	//local.sin_addr.S_un.S_addr = INADDR_ANY;//任意地址
	bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));

	listen(sListen, 1);//创建一个套接口并监听申请的连接

	sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);//是在一个套接口接受的一个连接
	printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr),
		ntohs(client.sin_port));

	while (TRUE) {
		printf("已有客户端成功连接\n");
		ret = recv(sClient, szMessage, MSGSIZE, 0);//本函数用于已连接的数据报或流式套接口进行数据的接收。
		szMessage[ret] = '\0';
		printf("Received [%d bytes]: '%s'\n", ret, szMessage);
	}
	return 0;
}