// client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "windows.h"
#include "process.h" 

#include "iostream"
using namespace std;
#pragma comment(lib,"ws2_32.lib")

#define Chat_Type_Num 2 //聊天类型

const char * g_strFlag[] =
{
	"-w",//世界频道
	"-p",//私聊
	""
};
enum Chat_Type
{
	Type_World,//世界频道
	Type_Private
};

Chat_Type g_curChatType = Type_World;//默认世界频道

void Connect(SOCKET sock)
{
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	serverAddress.sin_port = htons(1986);//和服务器一样

	//开始连接
	if (connect(sock, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		cout << "和服务器连接失败"<<WSAGetLastError()<<endl;
		exit(-1);
	}
}

void SendMsg(SOCKET clientSocket)
{
	char buf[2048] = { 0 };
	while (1)
	{
		gets_s(buf);

		if (strlen(buf) == 2)//因为切换的命令是两个字符
		{
			//可能输入的是切换命令
			if (strcmp(buf, g_strFlag[Type_World]) == 0)
			{
				//要切换到世界频道
				g_curChatType = Type_World;
				cout << "【世界】：";
				continue;
			}
			else if (strcmp(buf, g_strFlag[Type_Private]) == 0)
			{
				//要切换到私聊频道
				cout << "【系统】:切换到私聊频道,请以“角色名 聊天内容”的格式发送信息" << endl;
				cout << "【私聊】：";
				g_curChatType = Type_Private;
				continue;
			}
		}
		//strcpy_s(&buf[strlen(buf)], g_strFlag[g_curChatType]);
		strcpy_s(&buf[strlen(buf)], strlen(buf) + 1, g_strFlag[g_curChatType]);

		//发送给服务器
		if (send(clientSocket, buf, strlen(buf), 0) == SOCKET_ERROR)
		{
			cout << "发送消息失败";
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
			cout << "连接失败" << endl;
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
		cout << "套接字初始化失败";
		return -1;
	}

	SOCKET clientSocket;
	if ((clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		cout << "创建套接字失败";
		return -1;
	}

	Connect(clientSocket);//连接服务器

	_beginthread(Receive, 0, &clientSocket);//创建线程
	SendMsg(clientSocket);//发送消息
    return 0;
}

