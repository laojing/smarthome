#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fftw3.h>

#include "MFCC.h"
#include "VoiceCMD.h"

double *MFCC( float *signal, int len, int *frame_num ) {

	float u = 0.95;                      // 预加重系数

	fftw_plan p = NULL;
	double pTemp[FRAME_LEN];
	double y[FRAME_LEN];
	double banktmp[24];
	fftw_complex *out = (fftw_complex*)fftw_malloc( sizeof(fftw_complex)*FRAME_LEN );

	*frame_num = 0;
	for( int h=0; h<len-FRAME_LEN; h+=FRAME_MOV ) {
		(*frame_num)++;
	}
	printf( "frame_num:%d\n", *frame_num );
	fflush( stdout );
	double *m = malloc( sizeof(double)*12*(*frame_num) );
	int index = 0;

	for( int h=0; h<len-FRAME_LEN; h+=FRAME_MOV ) {
		for( int k=1; k<=FRAME_LEN; k++ ) {
			float temp = signal[h+k] - u*signal[h+k-1];
			pTemp[k-1] = temp * hamm[k-1];
		}

		p = fftw_plan_dft_r2c_1d( FRAME_LEN, pTemp, out, FFTW_ESTIMATE );
		fftw_execute( p );
		for( int i=0; i<FRAME_LEN; i++ ) {
			double tmp = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1]);
			y[i] = tmp*tmp;
		}

		for( int i=0; i<24; i++ ) {
			banktmp[i] = 0;
			for( int j=0; j<81; j++ ) {
				banktmp[i] += bank[i][j]*y[j];
			}
			banktmp[i] = log(banktmp[i]);
		}

		for( int i=0; i<12; i++ ) {
			double tmp = 0;;
			for( int j=0; j<24; j++ ) {
				tmp += dctt[i][j]*banktmp[j];
			}
			m[index*12 + i ] = tmp*w[i];
		}

		index++;
	}

	if( p != NULL ) fftw_destroy_plan( p );
	fftw_free( out );

	return m;
}

