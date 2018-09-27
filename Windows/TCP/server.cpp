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
	//local.sin_addr.S_un.S_addr = INADDR_ANY;//�����ַ
	bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));

	listen(sListen, 1);//����һ���׽ӿڲ��������������

	sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);//����һ���׽ӿڽ��ܵ�һ������
	printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr),
		ntohs(client.sin_port));

	while (TRUE) {
		printf("���пͻ��˳ɹ�����\n");
		ret = recv(sClient, szMessage, MSGSIZE, 0);//���������������ӵ����ݱ�����ʽ�׽ӿڽ������ݵĽ��ա�
		szMessage[ret] = '\0';
		printf("Received [%d bytes]: '%s'\n", ret, szMessage);
	}
	return 0;
}