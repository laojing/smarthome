#include "StateMachine.h"


// 阳台灯

U16 m_uiSunsSC;
SunsStates m_eSunsLastState = -1;
void InitSunsStateMachine ( ) {
    pSut->S_SunsState = 0;
    m_uiSunsSC = StartStepTimer();
    m_eSunsLastState = -1;
};
void UpdateSunsStateMachine ( ) {
    SunsStates curState;
    curState = pSut->S_SunsState;
    if ( m_eSunsLastState != curState ) {
        m_uiSunsSC = StartStepTimer();
        pSut->S_SunsSC = 0;
        m_eSunsLastState = curState;
    } else {
        pSut->S_SunsSC = GetStepTimerDuration ( m_uiSunsSC );
    }
    switch ( curState ) {
    case Sunsidle:
        if ( ( ! pSut->CI_SunsLightState )
             && ( pSut->D_SunsPulseOff 
             || pSut->D_SunsPulseOn ) ) {
            pSut->S_SunsState = Sunsopenin;
        } else if ( pSut->CI_SunsLightState 
             && ( pSut->D_SunsPulseOn 
             || pSut->D_SunsPulseOff ) ) {
            pSut->S_SunsState = Sunsclosein;
        } else if ( ( ! pSut->CI_SunsLightState )
             && pSut->CI_SunsLightCmd  ) {
            pSut->S_SunsState = Sunsopenin;
        } else if ( pSut->CI_SunsLightState 
             && pSut->CI_SunsLightCmd  ) {
            pSut->S_SunsState = Sunsclosein;
        }
        break;
    case Sunsopenin:
        pSut->S_SunsUpdate = true;
        pSut->CO_SunsLight = true;
        pSut->CI_SunsLightCmd = false;
        if ( true  ) {
            pSut->S_SunsState = Sunsopening;
        }
        break;
    case Sunsopening:
        if ( ! pSut->S_SunsUpdate  ) {
            pSut->S_SunsState = Sunsidle;
        }
        break;
    case Sunsclosein:
        pSut->S_SunsUpdate = true;
        pSut->CO_SunsLight = false;
        pSut->CI_SunsLightCmd = false;
        if ( true  ) {
            pSut->S_SunsState = Sunsclosing;
        }
        break;
    case Sunsclosing:
        if ( ! pSut->S_SunsUpdate  ) {
            pSut->S_SunsState = Sunsidle;
        }
        break;
    }
}


// 灯光

U16 m_uiDengSC;
DengStates m_eDengLastState = -1;
void InitDengStateMachine ( ) {
    pSut->S_DengState = 0;
    m_uiDengSC = StartStepTimer();
    m_eDengLastState = -1;
};
void UpdateDengStateMachine ( ) {
    DengStates curState;
    curState = pSut->S_DengState;
    if ( m_eDengLastState != curState ) {
        m_uiDengSC = StartStepTimer();
        pSut->S_DengSC = 0;
        m_eDengLastState = curState;
    } else {
        pSut->S_DengSC = GetStepTimerDuration ( m_uiDengSC );
    }
    switch ( curState ) {
    case Dengidle:
        if ( pSut->D_HumanBookOn  ) {
            pSut->S_DengState = Dengbookopenin;
        }
        break;
    case Dengbookopenin:
        pSut->S_DengState = true;
        pSut->CO_BookLight = true;
        if ( true  ) {
            pSut->S_DengState = Dengbookopening;
        }
        break;
    case Dengbookopening:
        if ( ! pSut->S_DengState  ) {
            pSut->S_DengState = Dengidle;
        }
        break;
    }
}


// 人体感应

U16 m_uiHumaSC;
HumaStates m_eHumaLastState = -1;
void InitHumaStateMachine ( ) {
    pSut->S_HumaState = 0;
    m_uiHumaSC = StartStepTimer();
    m_eHumaLastState = -1;
};
void UpdateHumaStateMachine ( ) {
    HumaStates curState;
    curState = pSut->S_HumaState;
    if ( m_eHumaLastState != curState ) {
        m_uiHumaSC = StartStepTimer();
        pSut->S_HumaSC = 0;
        m_eHumaLastState = curState;
    } else {
        pSut->S_HumaSC = GetStepTimerDuration ( m_uiHumaSC );
    }
    switch ( curState ) {
    case Humaidle:
        if ( ( pSut->S_DayTime > pSut->P_PowerOnTime )
             && ( pSut->S_DayTime < 
             pSut->P_PowerOnTime + pSut->P_PowerTimeSpan )
             && ( ! pSut->CI_HumanPowerState ) ) {
            pSut->S_HumaState = Humatimeopen;
        } else if ( ( pSut->S_DayTime > pSut->P_PowerOffTime )
             && ( pSut->S_DayTime < 
             pSut->P_PowerOffTime + pSut->P_PowerTimeSpan )
             && pSut->CI_HumanPowerState  ) {
            pSut->S_HumaState = Humatimeclose;
        } else if ( pSut->P_ActiveHuman 
             && ( ! pSut->CI_HumanPowerState ) ) {
            pSut->S_HumaState = Humacmdopen;
        } else if ( pSut->P_DeactiveHuman 
             && pSut->CI_HumanPowerState  ) {
            pSut->S_HumaState = Humacmdclose;
        }
        break;
    case Humatimeopen:
        pSut->S_HumanUpdate = true;
        pSut->CO_HumanPower = true;
        if ( true  ) {
            pSut->S_HumaState = Humaopen;
        }
        break;
    case Humacmdopen:
        pSut->S_HumanUpdate = true;
        pSut->CO_HumanPower = true;
        pSut->P_ActiveHuman = false;
        if ( true  ) {
            pSut->S_HumaState = Humaopen;
        }
        break;
    case Humaopen:
        if ( ! pSut->S_HumanUpdate  ) {
            pSut->S_HumaState = Humaidle;
        }
        break;
    case Humatimeclose:
        pSut->S_HumanUpdate = true;
        pSut->CO_HumanPower = false;
        if ( true  ) {
            pSut->S_HumaState = Humaclose;
        }
        break;
    case Humacmdclose:
        pSut->S_HumanUpdate = true;
        pSut->CO_HumanPower = false;
        pSut->P_DeactiveHuman = false;
        if ( true  ) {
            pSut->S_HumaState = Humaclose;
        }
        break;
    case Humaclose:
        if ( ! pSut->S_HumanUpdate  ) {
            pSut->S_HumaState = Humaidle;
        }
        break;
    }
}


// 感应响应

U16 m_uiRespSC;
RespStates m_eRespLastState = -1;
void InitRespStateMachine ( ) {
    pSut->S_RespState = 0;
    m_uiRespSC = StartStepTimer();
    m_eRespLastState = -1;
};
void UpdateRespStateMachine ( ) {
    RespStates curState;
    curState = pSut->S_RespState;
    if ( m_eRespLastState != curState ) {
        m_uiRespSC = StartStepTimer();
        pSut->S_RespSC = 0;
        m_eRespLastState = curState;
    } else {
        pSut->S_RespSC = GetStepTimerDuration ( m_uiRespSC );
    }
    switch ( curState ) {
    case Respidle:
        if ( pSut->D_HumanDoorOn  ) {
            pSut->S_RespState = Resphumanon;
        }
        break;
    case Resphumanon:
        HumanDoorOn();
        if ( true  ) {
            pSut->S_RespState = Respidle;
        }
        break;
    }
}
void InitStateMachines () {
    InitSunsStateMachine ();
    InitDengStateMachine ();
    InitHumaStateMachine ();
    InitRespStateMachine ();
}
void UpdateStateMachines () {
    UpdateSunsStateMachine ();
    UpdateDengStateMachine ();
    UpdateHumaStateMachine ();
    UpdateRespStateMachine ();
}
