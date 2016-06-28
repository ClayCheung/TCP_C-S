/*
 * =====================================================================================
 *
 *       Filename:  time_tcp_client.c
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
		char buffer[1024];
		memset(buffer, 0, sizeof(buffer));
		size_t size;
		if((size = read( sockfd,
						 buffer,
						 sizeof(buffer))) < 0){
			perror("read error");
		}
		if(write(STDOUT_FILENO, buffer, size) != size){
			perror("write error");
		}

		/*-----------------------------------------------------------------------------
		 *  步骤4：关闭socket
		 *-----------------------------------------------------------------------------*/
		close(sockfd);


	return 0;
}				/* ----------  end of function main  ---------- */















