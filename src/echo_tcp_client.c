/*
 * =====================================================================================
 *
 *       Filename:  echo_tcp_client.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月26日 10时03分40秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ClayZhang (伦), 
 *   Organization:  
 *
 * =====================================================================================
 */


#include <netdb.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include "msg.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
	if(argc < 3){
		printf("usage: %s ip port\n",argv[0]);
		exit(1);
	}
	/* 步骤1：创建socket */
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0){
		perror("socket error");
		exit(1);
	}
	
	/*-----------------------------------------------------------------------------
	 *  步骤1.5：定义并填充 IP  port (server端的)
	 *-----------------------------------------------------------------------------*/
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(atoi(argv[2]));
	//将ip地址转换成网络字节序后填入serveraddr中
	inet_pton(	AF_INET,
				argv[1],
				&serveraddr.sin_addr.s_addr);
	/*-----------------------------------------------------------------------------
	 *步骤2：客户端调用connect()函数连接到服务器端  
	 *-----------------------------------------------------------------------------*/
		if(connect( sockfd,
					(struct sockaddr *)&serveraddr,
					sizeof(serveraddr)) < 0){
			perror("connect error");
			exit(1);
		}
	

		/*-----------------------------------------------------------------------------
		 *  步骤3：调用IO函数（read/write）和服务器端进行双向通信
		 *-----------------------------------------------------------------------------*/
		char buff[512];
		size_t size;
		char *prompt = ">";
		while(1){
			memset(buff, 0, sizeof(buff));
			write(STDOUT_FILENO, prompt, 1);
			//读取键盘的输入，缓存进buff
			size = read(STDIN_FILENO, buff,sizeof(buff));
			if(size < 0){
				continue;			
			}
			buff[size-1] = '\0';
			//把缓存buff，写入server端
			if(write_msg(sockfd, buff, sizeof(buff)) < 0){
				perror("write msg error");
				continue;
			}else{//如果写入成功，则读取server返回的信息
				if(read_msg(sockfd, buff,
									sizeof(buff)) < 0){
					perror("read msg error");
					continue;
				}else{//读取server返回的信息printf到屏幕上
					printf("%s\n", buff);
				}
			}



		}
		/*-----------------------------------------------------------------------------
		 *  步骤4：关闭socket
		 *-----------------------------------------------------------------------------*/
		close(sockfd);


	return 0;
}				/* ----------  end of function main  ---------- */















