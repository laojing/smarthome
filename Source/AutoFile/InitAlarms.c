#include <stdlib.h> 			
#include "ChannelDefs.h" 			
#include "../MainFrame/Main.h"

U1 *m_AlarmState[ALARMSNUMBER]; 			
U8 m_AlarmConseq[ALARMSNUMBER]; 			
U8 m_AlarmReset[ALARMSNUMBER]; 			
U8 m_AlarmCategory[ALARMSNUMBER]; 			

U8 
GetAlarmConseq ( U8 i ) 
{ 			
	return m_AlarmConseq[i]; 
}			

U8 
GetAlarmResetType ( U8 i ) 
{ 			
	return m_AlarmReset[i]; 
} 			

U8 
GetAlarmCategory ( U8 i ) 
{ 			
	return m_AlarmCategory[i]; 
} 			

U1 
GetAlarmState ( U8 i ) 
{ 			
	return (*m_AlarmState[i]); 
} 			

void 
SetAlarmState ( U8 i, U1 b ) 
{ 			
	(*m_AlarmState[i]) = b; 
} 			

void 
InitAlarmConseq () 
{ 
}
