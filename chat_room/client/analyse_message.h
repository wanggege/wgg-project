#ifndef __ANALYSE_MESSAGE_H__
#define __ANALYSE_MESSAGE_H__

/*发送结构体类型消息*/
typedef struct message
{
    int stamp;         /*消息类型标记*/
    char to_id[9];     /*对方id*/
    char mge[100];     /*消息内容*/
    char id_me[9];     /*自己id*/
}*linkk;

void land(int sockfd,linkk p);    /*注册 登录*/
int land_enter(char *buf,int n);  /*服务类型检测*/
void analyse_message_write(linkk p_message,int fd);/*收数据函数*/
int analyse_message_read(linkk p_message,char *buf,int fd);/*发数据函数*/

void printf_s(void);   /*打印输入格式*/

#endif
