#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<winsock.h>
#pragma comment(lib,"Ws2_32.lib")




int main()
{


	//1.初始化网络
	WSADATA d;
	WSAStartup(0x202, &d);
	//2.购买手机
	int vivo = socket(AF_INET, SOCK_DGRAM, 0);
	int opt = 1;
	setsockopt(vivo, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
	//3.插入手机卡
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;//因特网地址
	myaddr.sin_port = htons(7979);//本程序的ID
	myaddr.sin_addr.S_un.S_addr = INADDR_ANY;//本机地址
	bind(vivo, (sockaddr*)&myaddr, sizeof(myaddr));

	//4.收发信息
	while (1)
	{

		char msg[100] = "";
		//recv(手机，存储区，长度，默认0)
		recv(vivo, msg, 99, 0);
		printf("%s\n", msg);
	
	}


	closesocket(vivo);
	return 0;
}