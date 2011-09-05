/**********************************************
*文件名：  my_link.h
*功能：    提供链表的各种操作
************************************************/

#include "my_link.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

linkk head = NULL; /*链表头指针*/

/*计算字符窗长度*/
int my_strlen(char *buf)
{
    int len = 0;
    if (buf == NULL)
    {
        return 0;
    }
    while ((*buf != '\n') && (*buf != '\0'))
    {
        len++;
	buf++;
    }
    return len;
}

/************************************************
函数名：  make_node
功能：    创建一个链表节点
输入参数：id:用户id
            connfd：服务器与用户间的链接文件描述符
返回值：  节点首地址
************************************************/
linkk make_node(char *id,int connfd)
{
    linkk p = malloc(sizeof(*p));
    int len;
    if (p == NULL)
    {
        return NULL;
    }
    strncpy(p->id,id,8);
    *((p->id) + 8) = '\0';
    len = my_strlen(id);
    strncpy(p->passward,id+8,len - 8);
    *((p->passward) + len - 8) = '\0';

    p->connfd = connfd;
    p->next = NULL;
    return p;
}

/***********************************************
*函数名：  free_node
*功能：    释放节点内存
*************************************************/
void free_node(linkk p)
{
    free(p);
}

/*************************************************
*函数名：  insert
*功能：    在链表中加入一个节点
**************************************************/
void  insert(linkk p)
{
        p->next = head;
	head = p;
}

/************************************************
*函数名：   delete
*功能：     在链表中删除一个结点
************************************************/
void delete(linkk p)
{
    linkk pre = NULL;   
   if (p == head)
   {
       head = p->next;
       return;
   }
   for (pre=head; pre; pre=pre->next)
   {
       if (pre->next == p)
       {
           pre->next = p->next;
	   return;
       }
   }
}

/**************************************************
*函数名：   search
*功能：     查找一个节点
******************************************************/
linkk search(char *id)
{
    linkk p;
    for (p=head; p; p=p->next)
    {
        if (strncmp(p->id, id,8) == 0)
	{
	    return p;
	}
    }
    return NULL;
}
linkk search_connfd(int connfd)
{
    linkk p;
    for (p=head; p; p=p->next)
    {
        if (p->connfd == connfd)
	{
	    return p;
	}
    }
    return NULL;
}

/***************************************************
*函数名：   traverse
*功能：     打印链表中的所有用户信息
****************************************************/
void traverse(void)
{
    linkk p;
    for (p=head; p; p=p->next)
    {
        printf("%s\n",p->id);
        printf("%s\n",p->passward);
    }
}
