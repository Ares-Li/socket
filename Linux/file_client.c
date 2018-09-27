#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#pragma pack(1)
typedef struct _file_info
{
	char name[51];
	unsigned int size;

}file_info;
#pragma pack()


int main(int argc, char** argv)
{
	if(argc != 3)
        {
                fprintf(stderr,"argc error");
                exit(1);
        }
    //    struct stat st;

        file_info fi;
//        fi.size = st.st_size;



	//创建套接字
	int sock;
	sock = socket(AF_INET,SOCK_STREAM,0);

	//绑定地址(可以省略)
	struct sockaddr_in myaddr;
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	myaddr.sin_port = htons(9999);	//不同主机端口无影响

	if(bind(sock,(struct sockaddr*)&myaddr,sizeof(myaddr)) == -1)
	{
		perror("bind error");
	}

	//指定服务器
	struct sockaddr_in srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = inet_addr(argv[1]);
	srvaddr.sin_port = htons(atoi(argv[2]));
	
	//连接服务器
	if(connect(sock,(struct sockaddr*)&srvaddr,sizeof(srvaddr)) == -1)
	{
		perror("connect error");
	}
	
	//收发数据
	char buff[1024] ;
	int ret = 0;
	FILE* fp = NULL;
	
	ret = recv(sock,&fi,sizeof(fi),0);
	printf("%d\n",fi.size);
	if(ret>0)
	{
		fp = fopen(fi.name,"wb");
		if(fp == NULL)
		{
			perror("Open file fail");
			exit(1);
		}
	}

	while(1)
	{
		ret = recv(sock,buff,sizeof(buff),0);
		if(ret>0)
		{
			fwrite(buff,1,ret,fp);
		}
		else break;
	}

	fclose(fp);
	close(sock);

	return 0;
}
