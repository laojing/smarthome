#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define IPSTR "123.125.65.52"
#define PORT 80
#define BUFSIZE 1024

#define MAX_BUFFER_SIZE 512
#define MAX_BODY_SIZE 1000000

char sbuffer[50000];
char rbuf[10240];

char *VoiceSet( char *audiodata, int buflen )
{
	int result = 0;
	int sockfd, ret, i, h;
	struct sockaddr_in servaddr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		printf("创建网络连接失败,本线程即将终止---socket error!\n");
		exit(0);
	};

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(PORT);
	if (inet_pton(AF_INET, IPSTR, &servaddr.sin_addr) <= 0 ){
		printf("创建网络连接失败,本线程即将终止--inet_pton error!\n");
		exit(0);
	};

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
		printf("连接到服务器失败,connect error!\n");
		exit(0);
	}
	printf("与远端建立了连接\n");

	char xxbuf[] = 
"POST /server_api?lan=zh&cuid=6738489&token=24.bc18b13b0eb89514926aa9a23b24a50c.2592000.1469619546.282335-6738489 HTTP/1.1\n\
Host:123.125.65.52\n\
Connection:Close\n\
Content-Type: audio/pcm;rate=8000\n\
Content-Length: %d\n\n";

	sprintf( sbuffer, xxbuf, buflen );

	int ssendlen = strlen(sbuffer) + buflen + 4;
	memcpy( sbuffer+strlen(sbuffer), audiodata, buflen );
	memcpy( sbuffer+ssendlen-4, "\r\n\r\n", 4 );

	send( sockfd, sbuffer, ssendlen, 0 );

	int llen = recv( sockfd, rbuf, 10240, 0 );
	if( llen > 0 ) {
		printf( "%s\n", rbuf );
//		if( strstr( rbuf, "天王盖地虎" ) ) result = 1;
//		if( strstr( rbuf, "天" ) ) { result = 1; }
//		if( strstr( rbuf, "宝塔" ) ) { result = 1; }
//		if( strstr( rbuf, "土豆" ) ) { result = 1; }
//		if( strstr( rbuf, "地瓜" ) ) { result = 1; }
//		if( strstr( rbuf, "哈哈" ) ) { result = 1; }
	}

	close(sockfd);
    return rbuf;
}
