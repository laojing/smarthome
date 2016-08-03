#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/msg.h>
#include <arpa/inet.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/rand.h>
#include <openssl/err.h>

#include "Voice.h"

#define SPAN 100

extern signed char buf[BUFSIZE];

static int stand[SPAN];
static int compare[SPAN];

char token[100];

int CalcWeight( signed char *buffer, int *data ) {

	memset( data, 0, sizeof(int)*SPAN );

	int totalcycle = 0;
	int cycle = 0;
	int startspan = 0;
	int startpos = 0;
	int stoppos = 0;
	for( int i=0; i<BUFSIZE/2; i+= 128 ) {
		float sum = 0;
		for( int j=0; j<128; j++) {
			sum += fabs((buffer[2*(i+j)+1] * 0xff + buffer[2*(i+j)])/32768.0);
		}
		if( sum < 5 && startspan == 0 ) { startspan = i; }
		if( startspan > 0 && startpos == 0 && sum > 8 ) {
			startpos = i;
			stoppos = 0;
//			printf( "\nStartPos:%d\n", i );
		}
		if( startspan > 0 && startpos > 0 && stoppos == 0 && sum < 6 ) {
			totalcycle++;
			printf( ":%d:\n", i-startpos );
//			printf( "\nStopPos:%d:   %d\n", i, i-startpos );
			if( i-startpos > 5000 && i-startpos < 9000 ) {

				cycle++;
				for( int k=startpos; k<i-1; k+=2 ){
					int index = (int)((buffer[k+1] * 0xff + buffer[k])/32768.0*SPAN);
					for( int h=0; h<index; h++ ) {
						data[h]++;
					}
				}
			} else {
				if( i-startpos > 9000 ) cycle = 0;
				break;
			}
			startpos = 0;
			stoppos = i;
		}

//		printf( "%8.2f", sum ); 
//		if( ii++ % 10 == 0 ) printf( "\n" );
	}

	if( totalcycle > 5 ) cycle = 0;
	if( cycle > 0 ) {
		for( int h=0; h<SPAN; h++ ) {
			if( data[h] > 0 ) data[h] /= cycle;
		}
	}	

	return cycle;

	/*
	float oldsum = 0;
	int start = 0;
	for( int i=128*20; i<LEN/2; i+= 128 ) {
		float sum = 0;
		int j;
		for( j=0; j<128; j++) {
			sum += fabs((buffer[2*(i+j)+1] * 0xff + buffer[2*(i+j)])/32768.0);
		}
		if( sum > 14 && oldsum > 11 ){
			start = i - 128;
			break;
		}
		oldsum = sum;
	}
	if( start == 0 || start >= LEN/2 ) {
		printf( "no voice input\n" ); fflush( stdout );
	} else {
		for( int i=start; i<start+LEN/4; i++ ){
			int index = (int)((buffer[2*(i)+1] * 0xff + buffer[2*i])/32768.0*SPAN);
			for( int k=0; k<index; k++ ) {
				data[k]++;
			}
		}
	}
	*/
}

void VoiceInitToken() {

	// 读取百度语音授权码
	int fd, size;
	fd = open( TOKENDIR, O_RDONLY );
	size = read(fd, token, sizeof(token));
	token[size-1] = 0;
	close(fd);

	// 读取标准语音码
	FILE *filed = fopen( STANDDIR, "rb" );
	fread( buf, BUFSIZE, 1, filed );
	fclose( filed );

	CalcWeight( buf, stand );
}

int CompareStand( signed char *buffer ) {

	int cycle = CalcWeight( buffer, compare );

	float result = 0;
	for( int k=0; k<SPAN; k++ ) {
		if( (stand[k] + compare[k]) != 0 ) result += 1.0 * abs( stand[k] - compare[k] ) / (stand[k] + compare[k]);
//		result += 1.0 * abs( stand[k] - compare[k] );
//		if( stand[k] != 0 ) result += 1.0 * abs( stand[k] - compare[k] ) / stand[k];
//		printf( "%6.3f", 1.0 * abs( stand[k] - compare[k] ) / stand[k] );
	}

	printf( "\n compare: %f\n", result );
	if( result > 40 || cycle < 2 ) return 0;
	else return 1;

	return 0;
}

int UpdateAuthenToken() {
	return 0;
}
