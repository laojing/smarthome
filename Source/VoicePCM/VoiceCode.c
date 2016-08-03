#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>

#include "VoicePCM.h"

extern signed char RecordBuf[];

#define SPAN 100

static int stand[SPAN];
static int standtmp[SPAN];
static int compare[SPAN];

// 计算声音纵向切割的权重
int CalcWeight( signed char *buffer, int *data ) {

	memset( data, 0, sizeof(int)*SPAN );

	int totalcycle = 0;
	int cycle = 0;
	int startspan = 0;
	int startpos = 0;
	int stoppos = 0;
	int bufsize = CODESECOND*SECONDSIZE/2;
	for( int i=0; i<bufsize; i+= 128 ) {
		float sum = 0;
		for( int j=0; j<128; j++) {
			sum += fabs((buffer[2*(i+j)+1] * 0xff + buffer[2*(i+j)])/32768.0);
		}
		if( sum < 5 && startspan == 0 ) { startspan = i; }
		if( startspan > 0 && startpos == 0 && sum > 8 ) {
			startpos = i;
			stoppos = 0;
		}
		if( startspan > 0 && startpos > 0 && stoppos == 0 && sum < 6 ) {
			totalcycle++;
			printf( ":%d:\n", i-startpos );
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
	}

	if( totalcycle > 5 ) cycle = 0;
	if( cycle > 0 ) {
		for( int h=0; h<SPAN; h++ ) {
			if( data[h] > 0 ) data[h] /= cycle;
		}
	}	
	return cycle;
}

void VoiceCodeStandInit() {
	// 读取标准语音码
	FILE *fd = fopen( STANDDIR, "rb" );
	fread( RecordBuf, CODESECOND*SECONDSIZE, 1, fd );
	fclose( fd );
	CalcWeight( RecordBuf, stand );

//	fd = fopen( TMPSTANDDIR, "rb" );
//	fread( RecordBuf, CODESECOND*SECONDSIZE, 1, fd );
//	fclose( fd );
//	CalcWeight( RecordBuf, standtmp );
}

int CompareStand( signed char *buffer ) {
	int cycle = CalcWeight( buffer, compare );
	if( cycle == 0 ) return 0;
	if( cycle > 2 ) return 1;

	float result = 0;
	if( cycle >1 ) {
		for( int k=0; k<SPAN; k++ ) {
//			if( (stand[k] + compare[k]) != 0 ) result += 1.0 * abs( stand[k] - compare[k] ) / (stand[k] + compare[k]);
			if( (stand[k] + compare[k]) != 0 ) result += 1.0 * abs( stand[k] - compare[k] );
		}
	} else {
		for( int k=0; k<SPAN; k++ ) {
			if( (standtmp[k] + compare[k]) != 0 ) result += 1.0 * abs( standtmp[k] - compare[k] );
		}
	}

	printf( "cycle:%d compare: %f\n", cycle, result );
	if( result < 40 && cycle > 1 ) return 1;
	if( result < 4000 && cycle == 1 ) return 1;
	return 0;
}

int SureVoiceInput( char *buf ) {
	char *result = Voice2Text( buf, CODESECOND*SECONDSIZE );
	if( result == NULL ) return 0;
	if( strstr( result, "长江" ) ) return 1;
	else return 0;
}

void SaveVoiceCodeStand( signed char *buf ) {
	char f[10];
	int fd = open( "/root/home/tmp.mat", O_WRONLY|O_CREAT, 0777 );
	for( int i=0; i<CODESECOND*SECONDSIZE; i+=2 ) {
		sprintf( f, "%d\n", buf[i+1]*0xff + buf[i] );
		write( fd, f, strlen(f) );
	}
	close( fd );
	/*
	int fd = open( TMPSTANDDIR, O_WRONLY|O_CREAT, 0777 );
	write( fd, buf, CODESECOND*SECONDSIZE );
	printf( "size:%d\n", CODESECOND*SECONDSIZE );
	close( fd );
	*/
}

int fuck = 0;
int MonitorVoiceCode( int fdsp ) {


	printf( "%d record start...", fuck++ ); fflush( stdout );
	signed char *buf = VoicePCMRecord( fdsp, CODESECOND );
	printf( " voice recod " ); fflush( stdout );
	if( buf == NULL ) return 0;
	SaveVoiceCodeStand( buf );
	return 0;
//	if( CompareStand( buf ) == 0 ) return 0;
//	printf( "\n" ); fflush( stdout );
//	return SureVoiceInput( buf );
}

