#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WaveLib.h"

#include "../Gnuplot/Gnuplot.h"

int comp( const void *a, const void *b ) {
	if( (*(double*)a - *(double*)b) > 0.00001 ) return 1;
	else if( (*(double*)a - *(double*)b) < -0.00001 ) return -1;
	else return 0;
}

void Wavelet( double *inp, int N, int J ) {
	wave_object obj;
	wt_object wt;
	double *out;
	int i;

	obj = wave_init("haar");

	out = (double*)malloc(sizeof(double)* N);

	wt = wt_init(obj, "dwt", N, J);
	setDWTExtension(wt, "sym");
	setWTConv(wt, "direct");
	
	dwt(wt, inp);
	
	double *dtmp = (double*)malloc(sizeof(double)* wt->outlength);
	double thr = sqrt(2*log(wt->outlength*1.0));

	memcpy( dtmp, wt->output, sizeof(double)*wt->outlength );

	int start = wt->length[0];
	for( int i=1; i<=wt->J; i++ ) {
		int len = wt->length[i];
		for( int j=0; j<len; j++ ) {
			*(dtmp+start+j) = fabs(*(dtmp+start+j));
		}
		qsort( dtmp + start, len, sizeof(double), comp );


		double fuck = 0;
		int len2 = len/2;
		if( len2*2 == len ) {
			fuck = (dtmp+start)[len2]/0.6745;
		} else {
			fuck = ((dtmp+start)[len2] + (dtmp+start)[len2+1])/2/0.6745;
		}
		double fuck2 = thr*fuck;
//		printf( "start:%d, len:%d, thr:%f, fuck:%f, %f\n", start, len, thr, fuck, fuck2 );
		for( int j=0; j<len; j++ ) {
			if( wt->output[start+j] <= fuck2 
			&& wt->output[start+j] >= -fuck2 ) 
				wt->output[start+j] = 0.0;
		}

		start += len;
	}
	free( dtmp );

	idwt(wt, out);
	
	wave_free(obj);
	wt_free(wt);

	memcpy( inp, out, N*sizeof(double) );

	free(out);
	return;
}
