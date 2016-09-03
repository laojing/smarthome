#include <pthread.h>

#include "home.h"
#include "Gnuplot/Gnuplot.h"

#include "VoiceCMD/VoiceCMD.h"
#include "WaveLib/WaveLib.h"

#define BUFSIZE 48000

signed char inbuf[BUFSIZE];
double xtime[BUFSIZE/2];
double signal[BUFSIZE/2];

void Calc( char *name ) {
	FILE *fi = fopen( name, "rb" );
	fread( inbuf, 1, BUFSIZE, fi );
	for( int i=0; i<BUFSIZE; i+=2 ) {
		signal[i/2] = (inbuf[i+1]*0xff + inbuf[i])/32768.0;
		xtime[i/2] = i/2*0.000125;
	}
	fclose( fi );

//	PlotXY( xtime, signal, BUFSIZE/2 );
 	Wavelet( signal, BUFSIZE/2, 7 );
//	PlotY( signal, BUFSIZE/2 );

	double *pSignal = signal + FRAME_MOV*50;
	int len = BUFSIZE/2 - 100*FRAME_MOV;

	double noise_zero = NoiseZero( pSignal, len );
//	printf( "zero:%f\n", noise_zero );
	for( int i=0; i<len; i++ ) {
		pSignal[i] -= noise_zero;
	}

	double noise_sum_max = 0;
	double noise_zero_max = 0;
	NoiseLimit( pSignal, len, &noise_sum_max, &noise_zero_max );

	int valid_start[VALID_MAX];
	int valid_end[VALID_MAX];

	int valid_con = EndPoint( pSignal, len, noise_sum_max, noise_zero_max, valid_start, valid_end );

//	PlotY( pSignal, len );
	printf( "valid_con:%d, %d\n", valid_con, valid_end[0], valid_start[0] );

};


int main() {

	Calc( "/home/laojing/arm11/xiaoqu/00.pcm" );
	Calc( "/home/laojing/arm11/xiaoqu/01.pcm" );
	Calc( "/home/laojing/arm11/xiaoqu/02.pcm" );
	Calc( "/home/laojing/arm11/xiaoqu/03.pcm" );
	Calc( "/home/laojing/arm11/xiaoqu/04.pcm" );



	/*
	int endSignal = 0;
    pthread_t threadVoicePCM;                               // PCM声卡控制线程

	pthread_create ( &threadVoicePCM, NULL, VoicePCMThread, (void *)&endSignal );

	getchar();
	endSignal = 1;

	pthread_join ( threadVoicePCM, NULL );
	*/

	return 0;
}

