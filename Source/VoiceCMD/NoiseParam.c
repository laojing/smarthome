#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "VoiceCMD.h"

double NoiseZero( double *signal, int len ) {

	double noise_sum = 0;
	int noise_sum_num = 0;

	int noise_number = 0;

	for( int i=0; i<len-FRAME_LEN; i+=FRAME_MOV ) {
		double tmp = 0;
		for( int j=i; j<i+FRAME_LEN; j++ ) {
			tmp += fabs( signal[j] );
		}

    	int noiseisend = 0;
		if ( tmp < NOISE_SUM_LIMIT ) {
			noise_number++;
			if( i + FRAME_MOV >= len-FRAME_LEN && noise_number > NOISE_VALID_NUM )
				noiseisend = 1;
		} else {
			if( noise_number > NOISE_VALID_NUM )
				noiseisend = 1;
			else
				noise_number = 0;
		}
		
		
		if( noiseisend == 1 ) {
			for( int j=i-noise_number*FRAME_MOV; j<i; j++ ) {
				noise_sum += signal[j];
				noise_sum_num++;
			}
			noise_number = 0;    
		}
	}

	if( noise_sum_num > 0 )
		noise_sum /= noise_sum_num;

	return noise_sum;
}

void NoiseLimit( double *signal, int len, double *noise_sum_max, double *noise_zero_max ) {

	int noise_number = 0;

	for( int i=0; i<len-FRAME_LEN; i+=FRAME_MOV ) {
		double tmp = 0;
		for( int j=i; j<i+FRAME_LEN; j++ ) {
			tmp += fabs( signal[j] );
		}

    	int noiseisend = 0;
		if ( tmp < NOISE_SUM_LIMIT ) {
			noise_number++;
			if( i + FRAME_MOV >= len-FRAME_LEN && noise_number > NOISE_VALID_NUM )
				noiseisend = 1;
		} else {
			if( noise_number > NOISE_VALID_NUM )
				noiseisend = 1;
			else
				noise_number = 0;
		}
		
		
		if( noiseisend == 1 ) {
			for( int n = i-noise_number*FRAME_MOV; n < i; n += FRAME_MOV ) {
				double tmp = 0;
				for( int j=n; j<n+FRAME_LEN; j++ ) {
					tmp += fabs( signal[j] );
				}

				if( tmp > *noise_sum_max ) *noise_sum_max = tmp;

				int Z = 0;
				for( int h = n; h < n+FRAME_LEN-2; h++ ) {
					int last_sig = 0;
					if( signal[h] >= 0 ) last_sig = 1;
					else last_sig = -1;
					
					if( last_sig == -1 ) { if( signal[h+1] >= 0 ) Z++; }
					else { if( signal[h+1] < 0 ) Z++; }
				}

				if( Z > *noise_zero_max ) {
					*noise_zero_max = Z;
				}
			}

			noise_number = 0;    

		}
	}
}

