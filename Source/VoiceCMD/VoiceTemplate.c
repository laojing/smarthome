#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#include "Template.h"
#include "VoiceCMD.h"
#include "../VoicePCM/VoicePCM.h"

float signal[CODESECOND*SECONDSIZE/2];

char line[100];

char *ReadLine( FILE *fd ) {
    char c; int len = 0;
    while ( (c = fgetc(fd) ) != EOF && c != '\n') {
        line[len++] = c;
    }
	line[len] = 0;
	return line;
}

struct voicetemplate *templates;
int templatenum = 0;

void VoiceTemplateFree() {
	for( int i=0; i<templatenum; i++ ) {
		for( int j=0; j<templates[i].templatenum; j++ ) {
			free( templates[i].items[j].data );
		}
		free( templates[i].items );
		free( templates[i].name );
	}
	free( templates );
}


void VoiceTemplateInit() {
	DIR *dp;
	struct dirent *entry;

	if( ( dp = opendir("/root/template")) == NULL ) {
		printf( "cannot open template dir!\n" );
		return;
	}
	while( (entry = readdir(dp)) != NULL ) {
		templatenum++;
	}
	templatenum -= 2;
	templates = (struct voicetemplate*)malloc( sizeof(struct voicetemplate)*templatenum );
	int index = 0;

	char path[100];
	rewinddir( dp );
	while( (entry = readdir(dp)) != NULL ) {

		if( strlen( entry->d_name ) > 2 ) {
			sprintf( path, "/root/template/%s", entry->d_name );
			DIR *wdir;
			struct dirent *wentry;
			wdir = opendir( path );
			templates[index].name = malloc( strlen(entry->d_name) + 1 );
			strcpy( templates[index].name, entry->d_name );
			templates[index].name[strlen(entry->d_name)] = 0;
			templates[index].templatenum = -2;
			while( (wentry = readdir(wdir)) != NULL ) {
				templates[index].templatenum++;
			}
			templates[index].items = malloc( sizeof(struct templateitem) * templates[index].templatenum );
			rewinddir( wdir );
			int itemindex = 0;
			while( (wentry = readdir(wdir)) != NULL ) {
				if( strstr( wentry->d_name, "mat" ) ) {
					sprintf( path, "/root/template/%s/%s", entry->d_name, wentry->d_name );

					FILE *fd = fopen( path, "rb" );
					int datalen = atoi( ReadLine(fd) );
					templates[index].items[itemindex].length = datalen;
					datalen *= 12;
					templates[index].items[itemindex].data = malloc( sizeof(double)*datalen );
					for( int i=0; i<datalen; i++ ) {
						templates[index].items[itemindex].data[i] = atof( ReadLine(fd) );
					}
					fclose( fd );

					itemindex++;
				}
			}
			index++;

		}
	}
}



void SaveTemplate( double *mfcc, int frame_num, char *voice ) {
	char path[100];
	sprintf( path, "/root/template/%s", voice );

	if( access( path, 0 ) == -1 ) {
		mkdir( path, 0777 );
	}

	char f[10];
	for( int i=0; i<20; i++ ) {
		sprintf( path, "/root/template/%s/%02d.mat", voice, i );
		if( access( path, 0 ) == -1 ) {

			int fd = open( path, O_WRONLY|O_CREAT, 0777 );

			sprintf( f, "%d\n", frame_num );
			write( fd, f, strlen(f) );

			for( int i=0; i<frame_num*12; i++ ) {
				sprintf( f, "%f\n", mfcc[i] );
				write( fd, f, strlen(f) );
			}
			close( fd );

			break;
		}
	}
}

void 
VoiceTemplateDelete( char *voice ) {
	char path[100];
	sprintf( path, "rm -rf /root/template/%s", voice );
	system( path );
}

void 
VoiceTemplateRecord( int fdsp, char *voice ) {

	signed char *buf = VoicePCMRecord( fdsp, CODESECOND );
	for( int i=0; i<CODESECOND*SECONDSIZE; i+=2 ) {
		signal[i/2] = (buf[i+1]*0xff + buf[i]) / 32768.0;
	}

//	FILE *fd = fopen( "/root/home/changjiang05.mat", "rb" );
//	for( int i=0; i<CODESECOND*SECONDSIZE/2; i++ ) {
//		signal[i] = atoi( ReadLine(fd) ) / 32768.0;
//	}
//	fclose( fd );

	float *pSignal = signal + FRAME_MOV*100;
	int len = CODESECOND*SECONDSIZE/2 - 100*FRAME_MOV;

	float noise_zero = NoiseZero( pSignal, len );
	for( int i=0; i<len; i++ ) {
		pSignal[i] -= noise_zero;
	}

	float noise_sum_max = 0;
	float noise_zero_max = 0;
	NoiseLimit( pSignal, len, &noise_sum_max, &noise_zero_max );

	int valid_start[VALID_MAX];
	int valid_end[VALID_MAX];

	int valid_con = EndPoint( pSignal, len, noise_sum_max, noise_zero_max, valid_start, valid_end );

	for( int i=0; i<valid_con; i++ ) {
		float *tmp = pSignal + valid_start[i];
		int frame_num;
		double *mfcc = MFCC( tmp, valid_end[i]-valid_start[i], &frame_num );
		if( frame_num > 40 && frame_num < 100 )
			SaveTemplate( mfcc, frame_num, voice );
		free( mfcc );
	}

}
