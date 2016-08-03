#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <arpa/inet.h>

#include "VoicePCM.h"

char*
GetToken() {
	static char *token = NULL;

	if( token != NULL ) return token;

	// 读取百度语音授权码
	int fd, size;
	struct stat statbuf;  
	stat( TOKENDIR, &statbuf );  
	size = statbuf.st_size;  

	token = malloc( size );

	fd = open( TOKENDIR, O_RDONLY );
	size = read(fd, token, size);
	token[size-1] = 0;
	close(fd);

	return token;
}
