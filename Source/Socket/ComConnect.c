#include "Com.h"

extern int endSignal;
void
*ComThread() {
	while( !endSignal ) {
	}
}



#define MYPORT  8088

int endSignal = 0;

pthread_t thread[2];

#define CMDLEN 30
#define SENDLEN 26
char sendcmd[CMDLEN];


pthread_t socketclient;
/*
void* SocketClient ( void* conn_fd ) {
	int conn = *((int *)conn_fd);
	printf ( "new client:%d\n", conn );
	fflush ( stdout );
    char buffer[CMDLEN];
//    while(1) {
        memset(buffer,0,sizeof(buffer));
        int len = recv(conn, buffer, sizeof(buffer),0);
		if ( len >= 9 ) {
			int cmdtype = buffer[0] - '0';
			int position = (buffer[1]-'0')*10 + (buffer[2]-'0');
			int datatype = buffer[3] - '0';
			if( cmdtype == 1 ) {   // 修改参数
				// 人体感应电源
				if( position == 0 ) {
					// 数据类型是boolean
					if( datatype == 1) {
						if( buffer[4] == 't' )
							pSut->P_ActiveHuman = true;
						else
							pSut->P_DeactiveHuman = true;
					}
				}
			} else if( cmdtype == 2 ) {   // 修改灯
				// 数据类型是boolean
				if( datatype == 1) {
					if( buffer[4] == 't' )
						ConAndCmdChannel( position, OPEN );
					else
						ConAndCmdChannel( position, CLOSE );
				}
			} else if( cmdtype == 3 ) {   // 查询状态
				if( position == 1) {           // 查询灯状态
				} else if( position == 2) {    // 查询人体感应状态
				}
			}


//			buffer[len] = 0;
//			send(conn, command, SENDLEN, 0);
		} 
//		sleep(1);
//    }
	printf ( "close client:%d\n", conn );
	fflush ( stdout );
    close(conn);
}


void* SocketServer () {
    ///定义sockfd
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);

    ///定义sockaddr_in
    struct sockaddr_in server_sockaddr;
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_port = htons(MYPORT);
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    ///bind，成功返回0，出错返回-1
    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)
    {
		printf( "Bind Server Error!\n" );
		fflush( stdout );
		close(server_sockfd);
		pthread_exit ( NULL );
    }

    ///listen，成功返回0，出错返回-1
    if(listen(server_sockfd,20) == -1)
    {
		printf( "Listen Server Error!\n" );
		fflush( stdout );
		close(server_sockfd);
		pthread_exit ( NULL );
    }

	printf( "Listenning...\n" );
	fflush( stdout );

    ///客户端套接字
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);

    ///成功返回非负描述字，出错返回-1
 	while ( GetEndSignal() == 0 ) {
		int conn = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn<0) {
			perror("connect");
			exit(1);
		} else {
			pthread_create(&socketclient, NULL, SocketClient, (void *)&conn);
		}
	}

    close(server_sockfd);
	pthread_exit ( NULL );
}
*/

void* SocketClient () {

	///定义sockaddr_in
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(MYPORT);  ///服务器端口
//	servaddr.sin_addr.s_addr = inet_addr("115.28.215.22");  ///服务器ip
	servaddr.sin_addr.s_addr = inet_addr("192.168.1.102");  ///服务器ip

 	while ( GetEndSignal() == 0 ) {
		///定义sockfd
		int sock_cli = socket(AF_INET,SOCK_STREAM, 0);

		///连接服务器，成功返回0，错误返回-1
		if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
		{
			close(sock_cli);
			perror("sutconnect");
			sleep(10);
			continue;
		}

		char sendbuf[512];
		char buffer[512];

		sendbuf[0] = 'h';
		send(sock_cli, sendbuf, 1,0); ///发送
		int len = recv(sock_cli, buffer, sizeof(buffer),0); ///接收
		if ( len >= 5 ) {
			int cmdtype = buffer[0] - '0';
			int position = (buffer[1]-'0')*10 + (buffer[2]-'0');
			int datatype = buffer[3] - '0';
			if( cmdtype == 1 ) {   // 修改参数
				// 人体感应电源
				if( position == 0 ) {
					// 数据类型是boolean
					if( datatype == 1) {
						pSut->P_ActiveHuman = buffer[4]-'0';
					}
				}
			} else if( cmdtype == 2 ) {   // 修改灯
				// 数据类型是boolean
				if( datatype == 1) {
					ConAndCmdChannel( position, buffer[4]-'0' );
				}
			}
		}

		close(sock_cli);
		sleep(1);
	}
}

