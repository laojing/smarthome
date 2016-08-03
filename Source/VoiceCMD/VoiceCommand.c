#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VoiceCMD.h"
#include "../VoicePCM/VoicePCM.h"

void VoiceModeCommand( int fdsp, int *voicemode ) {
	signed char *buf = VoicePCMRecord( fdsp, CMDSECOND );
	char *result = Voice2Text( buf, CMDSECOND*SECONDSIZE );

	if( result == NULL ) return;

	if( strstr( result, "下" ) ) { 
		VoicePCMPlay( fdsp, QUITDIR );
		*voicemode = 0; 
	}

	if( strstr( result, "滚" ) ) { 
		VoicePCMPlay( fdsp, QUITDIR );
		*voicemode = 0; 
	}

	if( strstr( result, "打开" ) ) { 
		if( strstr( result, "客厅" ) ) { 
//			ConAndCmdChannel( DOORLIGHT, 1 );
			VoicePCMPlay( fdsp, TASKOKDIR );
		}
	}

	if( strstr( result, "关闭" ) ) { 
		if( strstr( result, "客厅" ) ) { 
//			ConAndCmdChannel( DOORLIGHT, 0 );
			VoicePCMPlay( fdsp, TASKOKDIR );
		}
	}
}
