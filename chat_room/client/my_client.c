/***************************************************************
作者：           王阁阁 
文件名：           my_client
功能：             聊天室客户端
***************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "wrap.h"
#include "analyse_message.h"

#define MAXLINE        200       /*缓存区大小*/
#define SERV_PORT     8000

/****************发送的消息类型标记***************************/
#define REGISTER         0      /*注册 登录*/
#define CAT_ONLINE       1      /*查看在线用户*/
#define GROUP            2      /*群发消息*/
#define PERSONAL         3      /*私聊消息*/

int sign_forbid = 1;            /*禁言标记位*/

/********************************************************
函数名：          main
功能：            与其他用户聊天
********************************************************/
int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;

	linkk p_struct_message;     /*存放发送内容的结构体*/
	linkk p_struct_message_r;   /*存放接收内容的结构体*/

	char buf[MAXLINE];
	int sockfd, n, nn,nready;
	fd_set rset, allset;        /*读文件集*/

	int tmp_analyse;            /*存放函数返回的消息类型*/
	char dest = '\n';

	if (argc != 2)
	{
	    printf("请输入正确的IP\n");
	    exit(0);
	}
    
	sockfd = Socket(AF_INET, SOCK_STREAM, 0); 

	bzero(&servaddr, sizeof(servaddr));       //设置套接字
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(SERV_PORT);
    
	Connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));//连接服务器
        
	p_struct_message = (linkk)malloc(sizeof(*p_struct_message)); /*申请消息结构体的内存*/
	p_struct_message_r = (linkk)malloc(sizeof(*p_struct_message)); /*申请消息结构体的内存*/

	int fd = open("./chat_record.txt",O_RDWR | O_APPEND | O_CREAT,0644);/*打开存放聊天记录的文件*/
	if (fd == -1)
	{
	    perror("open chat_record:");
	}

        land(sockfd,p_struct_message);                      //用户登录
        
	/*存放聊天记录的登录用户名*/
//        write(fd,&dest,1);
        write(fd,&dest,1);
        write(fd,p_struct_message->id_me,8);
        
	/*添加文件描述符到读文件集*/
        FD_ZERO(&allset);         
	FD_SET(sockfd,&allset);
	FD_SET(STDIN_FILENO,&allset);

        while (1)
	{
	    rset = allset;
	    memset(buf,0,MAXLINE);
            if ((nready = select(sockfd+1,&rset,NULL,NULL,NULL)) < 0)
	    {
                 perr_exit("select error");
	    }
	    if (FD_ISSET(sockfd,&rset))     /*从服务器读到消息*/
	    {
	         if ((n = Read(sockfd,p_struct_message_r,sizeof(*p_struct_message))) == -1)
		 {
		     perr_exit("read socked");
		 }
		 if (n == 0)                  /*服务器关闭*/
		 {
		     printf("server is poweroff\n");
		     exit(0);
		 }

                 analyse_message_write(p_struct_message_r,fd);/*处理从服务器读到的消息*/

		 if (nready == 1)        /*输入格式错误*/
		 {
		     continue;
		 }
	    }

	    if (FD_ISSET(STDIN_FILENO,&rset))   /*从终端读到数据*/
	    {
	         if ((n = Read(0,buf,MAXLINE)) == -1)
		 {
		     perr_exit("read 0");
		 }
		 else
		 {
                     /*处理从终端读到的数据*/
		     tmp_analyse = analyse_message_read(p_struct_message,buf,fd);

		     if (tmp_analyse == -1)
		     {
		         printf_s();
		         continue;
		     }
		 }

		 if (sign_forbid == 1)       /*禁言判断*/
		 {
                     if ((n = Write(sockfd,p_struct_message,sizeof(*p_struct_message))) == -1)
		     {
		          perr_exit("write 1");
		     }
		     if (tmp_analyse == 8)
		     {
		          printf("服务器和客户端都将关闭\n");
			  exit(0);
		     }
		 }
		 else
		 {
		     printf("你已被管理员禁言，只能收到别人的消息\n");
		 }
	    }

	}

	Close(sockfd);
	return 0;
}
