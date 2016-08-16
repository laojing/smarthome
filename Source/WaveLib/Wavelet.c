#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WaveLib.h"

int comp( const void *a, const void *b ) {
	return fabs(*(double*)a) - fabs(*(double*)b);
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
		qsort( dtmp + start, len, sizeof(double), comp );

		double fuck = 0;
		int len2 = len/2;
		if( len2*2 == len ) {
			fuck = fabs((dtmp+start)[len2])/0.6745;
		} else {
			fuck = (fabs((dtmp+start)[len2]) + fabs((dtmp+start)[len2+1]))/2/0.6745;
		}
		double fuck2 = thr*fuck;
		for( int j=0; j<len; j++ ) {
			if( wt->output[start+j] <= fuck2 ) 
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
