#ifndef __ANALYSE_MESSAGE_H__
#define __ANALYSE_MESSAGE_H__

/*发送结构体类型消息*/
typedef struct message
{
    int stamp;         /*消息类型标记*/
    char to_id[9];     /*对方id*/
    char mge[100];     /*消息内容*/
    char id_me[9];     /*自己id*/
}*p_message;

char user_link(char *id,int connfd);
void analyse(p_message p_read,int sockfd,int fd);

#endif
