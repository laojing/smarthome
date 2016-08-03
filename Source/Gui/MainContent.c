#include "MainWindow.h"
#include "../AutoFile/StateMachine.h"

/*
BOOL
Draw ( GtkWidget *widget, cairo_t *cr, gchar *id ) {
	gint width, height;
	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);


	gchar *str = g_strdup_printf ( 
			"statecount:%f, state:%d ci_open:%d ci_close:%d", 
			pSut->S_DispSC, pSut->S_DispState, 
			pSut->CI_DispOpenCmd,
			pSut->CI_DispCloseCmd );
	cairo_move_to ( cr, 15, 100);
	cairo_show_text ( cr, str );
	g_free ( str );

	str = g_strdup_printf ( 
			"statecount:%f, state:%d LightState:%d switch:%d", 
			pSut->S_LiveSC, pSut->S_LiveState, 
			pSut->CI_LiveLightState,
			pSut->CI_LiveSwitch );
	cairo_move_to ( cr, 15, 200);
	cairo_show_text ( cr, str );
	g_free ( str );


	return FALSE;
}
*/
