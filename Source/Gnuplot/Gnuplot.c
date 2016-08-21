#include "Gnuplot.h"

void
PlotXY( double *x, double *y, int len ) {

	FILE *fo = fopen( "/home/laojing/tmp.dat", "w" );
	for( int i=0; i<len; i++ ) {
		fprintf( fo, "%f %f\n", x[i], y[i] );
	}
	fclose( fo );


	FILE *gp = popen( "gnuplot -persist", "w" );

	fprintf( gp, "set title '背景'\n" );
	fprintf( gp, "set xlabel 'xxx'\n" );
	fprintf( gp, "set ylabel 'xxx'\n" );

	fprintf( gp, "plot '/home/laojing/tmp.dat' using 1:2 with line\n" );

	fprintf( gp, "pause -1\n" );

	pclose( gp );

}

void
PlotY( double *y, int len ) {

	FILE *fo = fopen( "/home/laojing/tmp.dat", "w" );
	for( int i=0; i<len; i++ ) {
		fprintf( fo, "%f\n", y[i] );
	}
	fclose( fo );


	FILE *gp = popen( "gnuplot -persist", "w" );

	fprintf( gp, "set title '背景'\n" );
	fprintf( gp, "set xlabel 'xxx'\n" );
	fprintf( gp, "set ylabel 'xxx'\n" );

	fprintf( gp, "plot '/home/laojing/tmp.dat' using 1 with line\n" );

	fprintf( gp, "pause -1\n" );

	pclose( gp );

}
