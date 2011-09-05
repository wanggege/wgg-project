/**************************************************
//文件名：    my_link.h
//功能：      提供链表函数的接口及结构体的定义
***************************************************/

#ifndef __MY_LINK_H__
#define __MY_LINK_H__

typedef struct student *linkk;

/*用户信息结构体*/
struct student
{
    char passward[9];/*密码*/
    char id[9];      /*帐号*/
    int connfd;      /*文件描述符*/
    linkk next;      /*结构体指针*/
};

linkk make_node(char *id,int connfd);/*创建节点*/
void free_node(linkk p);             /*释放节点*/
void insert(linkk p);                /*插入节点*/
void delete(linkk p);                /*删除节点*/
linkk search(char *id);              /*按id搜索节点*/
linkk search_connfd(int connfd);     /*按文件描述符搜索节点*/
void traverse(void);                 /*遍历*/

#endif
