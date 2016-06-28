/*
 * =====================================================================================
 *
 *       Filename:  time_tcp_server.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年06月25日 23时22分48秒
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
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <signal.h>
#include <time.h>

int sockfd;

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  sig_handler
 *  Description:  Ctrl+C 的信号处理
 * =====================================================================================
 */
void sig_handler(int signo)
{
	if(signo == SIGINT){
		printf("server closed\n");
		/* 步骤6：关闭socket */
		close(sockfd);
		exit(1);
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  out_addr
 *  Description:  输出连接上来的客户端相关信息 
 * =====================================================================================
 */
void out_addr(struct sockaddr_in *clientaddr)
{
	//将端口从网络字节序转换成主机字节序
	int port = ntohs(clientaddr->sin_port);
	char ip[16];
	memset(ip,0,sizeof(ip));
	//将ip地址从网络字节序转换成点分十进制
	inet_ntop(	AF_INET,
				&clientaddr->sin_addr.s_addr,
				ip,
				sizeof(ip));
	printf("client:%s:%d connected\n", ip, port);
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  do_service
 *  Description:  调用IO 和客户端通信
 * =====================================================================================
 */
void do_service(int fd)
{
	//获得客户端系统时间
	long t = time(0);
	char *s = ctime(&t);
	size_t size = strlen(s) * sizeof(char);
	//将服务器端获得的系统时间写到客户端
	if(write(fd, s,size) != size){
		perror("write error");
	}
}


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

int main(int argc, char *argv[])
{
	if(argc < 2){
		printf("usage: %s #port\n",argv[0]);
		exit(1);
	}
	if(signal(SIGINT, sig_handler) == SIG_ERR){
		perror("signal sigint error");
		exit(1);
	}

	
	/*-----------------------------------------------------------------------------
	 *  步骤1： 创建socket
	 *  注： socket创建在内核中，是一种结构体，这里用函数构建之
	 *  AF——INET ： IPv4
	 *  SOCK_STREAM: tcp协议
	 *-----------------------------------------------------------------------------*/
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		perror("socket error");
		exit(1);
	}


	printf("socket created!\n");

	/*-----------------------------------------------------------------------------
	 *  步骤2：调用bind（）将socket和地址
	 *  （包括ip port）进行绑定
	 *-----------------------------------------------------------------------------*/

	
	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));		//内存清空，好习惯
	//往地址中填入ip port internet地址族类型
	serveraddr.sin_family = AF_INET;	
	serveraddr.sin_port = htons(atoi(argv[1]));//port  ascii->int(host)->network  16位
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(sockfd, (struct sockaddr *)&serveraddr,
							sizeof(serveraddr)) < 0){
		perror("bind err");
		exit(1);
	}

	printf("bind is done!\n");

	/*-----------------------------------------------------------------------------
	 *  步骤3：调用listen()启动监听（指定Port监听）
	 *  通知系统去接受来自客户端的连接请求
	 *  （将连接请求放置到对应的队列当中）
	 *	第二个参数：队列长度
	 *-----------------------------------------------------------------------------*/

	if(listen(sockfd, 10) < 0){
		perror("listen error");
		exit(1);
	}

	
	printf("listening!\n");
	/*-----------------------------------------------------------------------------
	 *  步骤4：调用accept()从队列中获得
	 *  一个客户端的请求连接,并返回一个新的socket描述符
	 *  × 返回值fd是 客户端的网络文件描述符：代表客户端的文件
	 *  × 注意：若没有客户端连接，调用此函数后会阻塞，
	 *	        直到获得一个客户端的连接。
	 *  ×× accept(), IO, close()这三个步骤都在while(1)死循环中
	 *-----------------------------------------------------------------------------*/

	
	struct sockaddr_in clientaddr;
	socklen_t clientaddr_len = sizeof(clientaddr);
	while(1){
		int fd = accept(sockfd,
					(struct sockaddr *)&clientaddr,
					&clientaddr_len);
		if(fd <0){
			perror("accept error");
			continue;
		}
				/*-----------------------------------------------------------------------------
				 *  步骤5：调用IO函数（read/write）和
				 *  连接的客户端进行双向通信
				 *-----------------------------------------------------------------------------*/
		out_addr(&clientaddr);
		do_service(fd);
				/*-----------------------------------------------------------------------------
				 *  步骤6：关闭socket
				 *-----------------------------------------------------------------------------*/
		close(fd);
	}




	return 0;
}



















