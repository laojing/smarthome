#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "VoicePCM.h"

int GetContentLength(char *revbuf)
{
    char *p1 = NULL, *p2 = NULL;
    int HTTP_Body = 0;//内容体长度

    p1 = strstr(revbuf,"Content-Length");
    if(p1 == NULL)
        return -1;
    else
    {
        p2 = p1+strlen("Content-Length")+ 2;
        HTTP_Body = atoi(p2);
        return HTTP_Body;
    }
}

void SaveFile( char *rbuf, int len, int new ) {
	int fd;
	if( new )
		fd = open( VOICEDIR, O_WRONLY|O_CREAT|O_TRUNC, 0777 );
	else
		fd = open( VOICEDIR, O_WRONLY|O_CREAT|O_APPEND, 0777 );
	write( fd, rbuf, len );
	close( fd );
}

int 
Text2Voice( char *voice ) {
	int hand = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( hand == -1 ) {
		printf( "Create Socket Error\n" );
		close( hand );
		return 0;
	}
	struct sockaddr_in addr;
	memset( (char*)&addr, '\0', sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons( 80 );
	addr.sin_addr.s_addr = inet_addr( "111.13.82.15" );
	bzero( &(addr.sin_zero), 8 );

	if ( connect( hand, (struct sockaddr*)&addr, sizeof(addr) ) == -1 ) {
		printf ( "Connect Failed\n" );
		close( hand );
		return 0;
	}
	char xbuf[] = 
"GET /text2audio?tex=%s&lan=zh&cuid=%s&ctp=1&tok=%s HTTP/1.1\n\
Host:111.13.82.15\n\
Accept:*/*\n\
Accept-Language:zh-cn\n\
Connection:Close\n\
\n\
";
	char sbuf[2024];
	sprintf ( sbuf, xbuf, voice, "5404a64ddb31", GetToken() );
	send ( hand, sbuf, strlen(sbuf), 0 );
	char rbuf[10240];
	int len = recv ( hand, rbuf, 10240, 0 );
	int authenfailed = 0;
	if ( len > 0 ) {
		int filelen = GetContentLength( rbuf );
		if( filelen < 0 ) { close( hand ); return 0; };
		char *p = strstr( rbuf, "\r\n\r\n" );
		if( strstr( p+4, "authentication failed" ) ) {
			authenfailed = 1;
		} else {
			SaveFile( p+4, len - (p+4 - rbuf), 1 );
			while( (len = recv ( hand, rbuf, 10240, 0 )) > 0 ){
				SaveFile( rbuf, len, 0 );
			}
		}
	}
	close ( hand );

	/*
	if( authenfailed ) {
		UpdateAuthenToken();
//		VoiceGet( voice );
	} else {
		free( voice );
	}
	*/
	return 0;
}
