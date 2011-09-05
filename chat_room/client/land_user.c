/*********************************************************
文件名：             land_user.c
功能：               用户注册及登录
作者：               王阁阁 
函数：land_enter :   检测用户需要的服务
      land       :   注册及登录
**********************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include "wrap.h"
#include "analyse_message.h"

#define MAXLINE        200       /*缓存区大小*/

/****************发送的消息类型标记***************************/
#define REGISTER         0      /*登录*/
#define CAT_ONLINE       1      /*查看在线用户*/
#define GROUP            2      /*群发消息*/
#define PERSONAL         3      /*私聊消息*/
#define EXIT             4      /*下线*/
#define TAP_PEOPLE       5      /*踢人*/
#define FORBID           6      /*禁言*/
#define REGISTER_P       9      /*注册*/

#define LOGIN            1      /*登录入口*/
#define REGISTE          2      /*注册入口*/

extern int sign_forbid;         /*声明禁言标记位*/

/*字符串比较*/
int land_enter(char *buf,int n)
{
    if (strncmp(buf,"3",1) == 0)
    {
        exit(0);
    }
    if (strncmp(buf,"1",1) == 0)
    {
        return LOGIN;
    }
    if (strncmp(buf,"2",1) == 0)
    {
        return REGISTE;
    }
    return -1;
}

/************************************************************
函数名：           land
功能：             客户端登录程序
输入参数：         与服务器链接的文件描述符
************************************************************/
void land(int sockfd,linkk p)
{
    int n;
    char buf[MAXLINE];
    begin:
    printf("欢迎使用，您的肯定将是对我最大的支持\n");
    printf("      1.login\n      2.register\n      3.exit\n\n");
    
    n = read(0,buf,MAXLINE);

    switch (land_enter(buf,n)) /*选择服务类型*/
    {
        case LOGIN:
		  goto login;
        case REGISTE:
	          goto registe;
	case -1:
	          goto begin;
    }

    registe:
         printf("请输入你想注册的id号（8位）：\n");
         if ((n = Read(0,p->mge,MAXLINE)) != 9)  //输入8位id 
         {
              goto registe;
         }
     registe_passward:
         printf("请输入你的密码（1～8位）：\n");
         n = Read(0,buf,MAXLINE);           //输入1~8位密码
         printf("请再次输入你的密码（1～8位）：\n");
         n = Read(0,p->mge+8,MAXLINE);           //输入1~8位密码
	 if (strncmp(buf,p->mge+8,n) != 0)
	 {
	     printf("两次密码不相同\n");
	     goto registe_passward;
	 }
         *(p->mge+7+n) = '\0';
         p->stamp = REGISTER_P;

         Write(sockfd,p,sizeof(*p)); /*发送给服务器信息*/

	 Read(sockfd,buf,1);

         if (*buf == '0')
	 {
	     printf("注册失败，此id之前已被注册\n");
	     goto begin;
	 }
	 if (*buf == '1')
	 {
	     printf("注册成功，感谢你对我们的支持\n\n");
	     goto begin;
	 }

    login:
         printf("请输入你的id号（8位）：\n");
         if ((n = Read(0,p->mge,MAXLINE)) != 9)  //输入8位id 
         {
              goto login;
         }
    login_passward:
         strncpy(p->id_me,p->mge,8);
 	 *(p->id_me + 8) = '\0';
         printf("请输入你的密码（1～8位）：\n");

         n = Read(0,p->mge+8,MAXLINE);           //输入1~8位密码
         *(p->mge+7+n) = '\0';
         p->stamp = REGISTER;

         Write(sockfd,p,sizeof(*p));

	 Read(sockfd,buf,1);

	 if (*buf == '0')
	 {
	      printf("密码错误\n");
	      goto login_passward;
	 }
	 if (*buf == '2')
	 {
	     printf("该用户已在线，请登录其他用户\n");
	     goto login;
	 }
	 if (*buf == '3')
	 {
	     printf("此用户未注册，请先注册或登录其他用户\n");
	     goto login;
	 }
	 if (*buf == '1')
	 {
	     printf("登录成功，欢迎您使用慧杰聊天室\n\n");
	     printf_s();     /*打印输入格式*/
	 }
}
