#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<winsock.h>
#pragma comment(lib,"Ws2_32.lib")




int main()
{


	//1.��ʼ������
	WSADATA d;
	WSAStartup(0x202, &d);
	//2.�����ֻ�
	int vivo = socket(AF_INET, SOCK_DGRAM, 0);
	int opt = 1;
	setsockopt(vivo, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
	//3.�����ֻ���
	sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;//��������ַ
	myaddr.sin_port = htons(7979);//�������ID
	myaddr.sin_addr.S_un.S_addr = INADDR_ANY;//������ַ
	bind(vivo, (sockaddr*)&myaddr, sizeof(myaddr));

	//4.�շ���Ϣ
	while (1)
	{

		char msg[100] = "";
		//recv(�ֻ����洢�������ȣ�Ĭ��0)
		recv(vivo, msg, 99, 0);
		printf("%s\n", msg);
	
	}


	closesocket(vivo);
	return 0;
}