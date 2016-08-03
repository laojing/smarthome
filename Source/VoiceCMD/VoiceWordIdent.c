#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "../home.h"
#include "Template.h"
#include "VoiceCMD.h"
#include "../VoicePCM/VoicePCM.h"

extern float signal[CODESECOND*SECONDSIZE/2];
extern struct voicetemplate *templates;
extern int templatenum;

double GetDistance( double *R, double *T ) {
	double dis = 0;
	for( int i=0; i<12; i++ ) {
		dis += (R[i] - T[i]) * (R[i] - T[i]);
	}
//	return sqrt(dis);
	return dis;
}

double DTW( double *R, int M, double *T, int N ) {
	double disup, disright, disupright;
	double dis = GetDistance( R, T );
	int x = 0;
	int y = 0;

	do {
		disup = (y + 1 < N) ? GetDistance( R, T+12 ) : 1000000.0;
		disright = (x + 1 < M) ? GetDistance( R+12, T ) : 1000000.0;
		disupright = (y + 1 < N && x + 1 < M) ? GetDistance( R+12, T+12 ) : 1000000.0;

		if( disup < disright && disup < disupright ) {
			dis += disup;
			T += 12;
			y++;
		} else if( disright < disupright ) {
			dis += disright;
			R += 12;
			x++;
		} else {
			dis += disupright;
			R += 12; T += 12; 
			x++; y++;
		}

	} while( x < M - 1 || y < N -1 );

	return dis;
}

char *DynamicWarping( double *mfcc, int frame_num, char *mode[], int modelen ) {
	char *res = "---";
	double dismin = 90000.00;

	for( int i=0; i<templatenum; i++ ) {

		int ismode = 0;
		for( int j=0; j<modelen; j++ ) {
			if( strcmp( templates[i].name, mode[j] ) == 0 ) {
				ismode = 1;
				break;
			}
		}
		if( ismode == 0 ) continue;

		for( int j=0; j<templates[i].templatenum; j++ ) {
			double dis = DTW( mfcc, frame_num, templates[i].items[j].data, templates[i].items[j].length );
			if( dis < dismin ) {
				dismin = dis;
				if( dis < IDENTLIMIT ) res = templates[i].name;
			}
		}
	}
	printf( "dis:%f:%s\n", dismin, res );
	return res;
}

char *codes[] = {"changjiang"};
char *commands[] = {"dakai", "guanbi", "baoshi", "xiangsheng", "tuixia", "xiaoqu"};
char *crossname[] = {"baorui", "degang", "xiaoyue"};
char *lightsname[] = {"suoyou", "ciwei", "zhuwei", "shufang", "yimao", "zhuwo", "ciwo", "keting", "shafa", "batai", "rumen", "menqian", "menting", "guodu", "canting", "zoulang", "chufang", "yangtai"};

int VoiceWordIdent( int fdsp, int mode, int *voicemode ) {


	signed char *buf = VoicePCMRecord( fdsp, CODESECOND );
	for( int i=0; i<CODESECOND*SECONDSIZE; i+=2 ) {
		signal[i/2] = (buf[i+1]*0xff + buf[i]) / 32768.0;
	}

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

	int result = 0;
	// 口令检测模式
	if( mode == 0 ) {
		for( int i=0; i<valid_con; i++ ) {
			int frame_num = 0;
			float *tmp = pSignal + valid_start[i];
			double *mfcc = MFCC( tmp, valid_end[i]-valid_start[i], &frame_num );
			char *word = DynamicWarping( mfcc, frame_num, codes, sizeof(codes)/sizeof(char*) );
			if( strcmp(word, codes[0]) == 0 ) {
				result = 1;
			}
			free( mfcc );
		}
	} else if( mode == 1 && valid_con > 0 && valid_con < 3 ) {
		printf( "-------%d\n", valid_con );
		int frame_num = 0;
		float *tmp = pSignal + valid_start[0];
		double *mfcc = MFCC( tmp, valid_end[0]-valid_start[0], &frame_num );
		char *word = DynamicWarping( mfcc, frame_num, commands, sizeof(commands)/sizeof(char*) );
		free( mfcc );

		if( strcmp(word, "tuixia") == 0 ) {
			VoicePCMPlay( fdsp, QUITDIR );
			*voicemode = 0; 
		} else if( strcmp(word, "baoshi") == 0 ) {
 			GiveTime();
			*voicemode = 0; 
		} else if( strcmp(word, "xiaoqu") == 0 ) {
			PlayMp3Rand( "music", NULL );
			*voicemode = 0; 
		} else if( strcmp(word, "xiangsheng") == 0 ) {
			if( valid_con == 1 ) return 0;
			frame_num = 0;
			tmp = pSignal + valid_start[1];
			mfcc = MFCC( tmp, valid_end[1]-valid_start[1], &frame_num );
			word = DynamicWarping( mfcc, frame_num, crossname, sizeof(crossname)/sizeof(char*) );
			free( mfcc );
			if( strcmp(word, "---") != 0 ) {
				PlayMp3Rand( "crosstalk", word );
				*voicemode = 0; 
			}
		} else if( strcmp(word, "guanbi") == 0 ) {
			if( valid_con == 1 ) return 0;
			frame_num = 0;
			tmp = pSignal + valid_start[1];
			mfcc = MFCC( tmp, valid_end[1]-valid_start[1], &frame_num );
			word = DynamicWarping( mfcc, frame_num, lightsname, sizeof(lightsname)/sizeof(char*) );
			free( mfcc );
			if( strcmp(word, "---") != 0 ) {
				int isall = 0;
				int lightlen = sizeof(lightsname)/sizeof(char*);
				for( int k=0; k<lightlen; k++ ) {
					if( isall ) {
						ConAndCmdChannel( k-1, 0 );
						continue;
					}
					if( strcmp( word, lightsname[k] ) == 0 ) {
						if( k==0 ) isall = 1;
						else ConAndCmdChannel( k-1, 0 );
					}
				}
				VoicePCMPlay( fdsp, TASKOKDIR );
			}
		} else if( strcmp(word, "dakai") == 0 ) {
			if( valid_con == 1 ) return 0;
			frame_num = 0;
			tmp = pSignal + valid_start[1];
			mfcc = MFCC( tmp, valid_end[1]-valid_start[1], &frame_num );
			word = DynamicWarping( mfcc, frame_num, lightsname, sizeof(lightsname)/sizeof(char*) );
			free( mfcc );
			if( strcmp(word, "---") != 0 ) {
				int isall = 0;
				int lightlen = sizeof(lightsname)/sizeof(char*);
				for( int k=0; k<lightlen; k++ ) {
					if( isall ) {
						ConAndCmdChannel( k-1, 1 );
						continue;
					}
					if( strcmp( word, lightsname[k] ) == 0 ) {
//						if( k==0 ) isall = 1;
//						else ConAndCmdChannel( k-1, 1 );
						ConAndCmdChannel( k-1, 1 );
					}
				}
				VoicePCMPlay( fdsp, TASKOKDIR );
			}
		}
	}

	return result;
}

void GiveTime() {

    char *wday[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"};

    time_t timep;
    struct tm *p;
    time( &timep );
    p = localtime( &timep ); //取得当地时间

	char datetime[100];
	int len = sprintf( datetime, "报告，%d年，%d月，%d日，%s，%d时，%d分。。",
				1900+p->tm_year, 1+p->tm_mon, p->tm_mday,
				wday[p->tm_wday], p->tm_hour, p->tm_min );

	char *voice = malloc( strlen( datetime ) + 1 );
	strcpy( voice, datetime );

	VoicePCMCmd( voice, PLAYVOICE );
}
