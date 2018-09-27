// client.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "windows.h"
#include "process.h" 

#include "iostream"
using namespace std;
#pragma comment(lib,"ws2_32.lib")

#define Chat_Type_Num 2 //��������

const char * g_strFlag[] =
{
	"-w",//����Ƶ��
	"-p",//˽��
	""
};
enum Chat_Type
{
	Type_World,//����Ƶ��
	Type_Private
};

Chat_Type g_curChatType = Type_World;//Ĭ������Ƶ��

void Connect(SOCKET sock)
{
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(1986);//�ͷ�����һ��

	//��ʼ����
	if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cout << "�ͷ���������ʧ��"<<WSAGetLastError()<<endl;
		exit(-1);
	}
}

void SendMsg(SOCKET clientSocket)
{
	char buf[2048] = { 0 };
	while (1)
	{
		gets_s(buf);

		if (strlen(buf) == 2)//��Ϊ�л��������������ַ�
		{
			//������������л�����
			if (strcmp(buf, g_strFlag[Type_World]) == 0)
			{
				//Ҫ�л�������Ƶ��
				g_curChatType = Type_World;
				cout << "�����硿��";
				continue;
			}
			else if (strcmp(buf, g_strFlag[Type_Private]) == 0)
			{
				//Ҫ�л���˽��Ƶ��
				cout << "��ϵͳ��:�л���˽��Ƶ��,���ԡ���ɫ�� �������ݡ��ĸ�ʽ������Ϣ" << endl;
				cout << "��˽�ġ���";
				g_curChatType = Type_Private;
				continue;
			}
		}
		//strcpy_s(&buf[strlen(buf)], g_strFlag[g_curChatType]);
		strcpy_s(&buf[strlen(buf)], strlen(buf) + 1, g_strFlag[g_curChatType]);

		//���͸�������
		if (send(clientSocket, buf, strlen(buf), 0) == SOCKET_ERROR)
		{
			cout << "������Ϣʧ��";
			return;
		}
	}
}

void Receive(void *p)
{
	char buf[2048] = { 0 };
	SOCKET * sock = (SOCKET *)p;
	while (1)
	{
		int bytes;
		if((bytes = recv(*sock,buf,sizeof(buf),0)) == SOCKET_ERROR)
		{
			cout << "����ʧ��" << endl;
			continue;
		}
		buf[bytes] = '\0';
		cout << buf << endl;
	}
}

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cout << "�׽��ֳ�ʼ��ʧ��";
		return -1;
	}

	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		cout << "�����׽���ʧ��";
		return -1;
	}

	Connect(clientSocket);//���ӷ�����

	_beginthread(Receive, 0, &clientSocket);//�����߳�
	SendMsg(clientSocket);//������Ϣ
    return 0;
}

