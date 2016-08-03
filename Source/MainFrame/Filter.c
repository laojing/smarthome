#include "Filter.h"

 
//====================================================================
// 初始化上升沿滤波。
//====================================================================
void 
InitPosEdge ( tPosEdge * A_pPosEdge, 
		U1 A_bIn )
{
    A_pPosEdge->last = A_bIn;
}
 
//====================================================================
// 更新上升沿滤波。
//====================================================================
U1 
PosEdge ( tPosEdge * A_pPosEdge, 
		U1 A_bIn )
{
    U1 isPosEdge = (!A_pPosEdge->last && A_bIn);
    A_pPosEdge->last = A_bIn;
    return isPosEdge;
}

//====================================================================
// 初始化下降沿滤波。
//====================================================================
void 
InitNegEdge ( tNegEdge * A_pNegEdge, 
		U1 A_bIn )
{
    A_pNegEdge->last = A_bIn;
}

//====================================================================
// 更新下降沿滤波。
//====================================================================
U1 
NegEdge ( tNegEdge * A_pNegEdge, 
		U1 A_bIn )
{
    U1 isNegEdge = (A_pNegEdge->last && !A_bIn);
    A_pNegEdge->last = A_bIn;
    return isNegEdge;
}

//====================================================================
// 初始化平均值滤波。
//====================================================================
void 
InitMovAve ( tMovAve *A_pMovAve, 
		REAL A_fSampleTime,
		REAL A_fTimeConstant, 
		REAL A_fIn )
{
    A_pMovAve->timeConstant = A_fSampleTime / A_fTimeConstant;
    A_pMovAve->movAve = A_fIn;
}
 
//====================================================================
// 更新平均值滤波。
//====================================================================
REAL MovAve ( tMovAve *A_pMovAve, 
		REAL A_fIn )
{
	A_pMovAve->movAve = A_fIn * A_pMovAve->timeConstant +
         A_pMovAve->movAve * (1.0 - A_pMovAve->timeConstant);
	return A_pMovAve->movAve;
}
 
//====================================================================
// 初始化真持续时间滤波。
//====================================================================
void 
InitTrueDuration ( tTrueDuration *A_pTrueDuration, 
		REAL A_fSampleTime, 
		U1 A_bIn)
{
    A_pTrueDuration->sampleTime = A_fSampleTime;
    A_pTrueDuration->trueSamples = 0;
}

//====================================================================
// 更新真持续时间滤波。
//====================================================================
REAL 
TrueDuration ( tTrueDuration *A_pTrueDuration, 
		U1 A_bIn)
{
    if ( !A_bIn ) {
        A_pTrueDuration->trueSamples = 0;
        return 0;
    } else {
        if (A_pTrueDuration->trueSamples < U16_MAX) {
            A_pTrueDuration->trueSamples++;
        }
        return A_pTrueDuration->sampleTime * A_pTrueDuration->trueSamples;
    }
}

//====================================================================
// 初始化双脉冲计数。
//====================================================================
void 
InitCountDualPulse ( tCountDualPulse * A_pCountDualPulse, 
		REAL Step, 
		REAL Init, 
		U1 A,
		U1 B)
{
	A_pCountDualPulse->step = Step;
	A_pCountDualPulse->value = Init;
	if ( A && B ) {
		A_pCountDualPulse->state = AllTrue;
	} else if ( A && ( !B ) ) {
		A_pCountDualPulse->state = ATBF;
	} else if ( ( !A ) && B ) {
		A_pCountDualPulse->state = AFBT;
	} else if ( ( !A ) && ( !B ) ) {
		A_pCountDualPulse->state = AllFalse;
	}
	A_pCountDualPulse->oldstate = A_pCountDualPulse->state;
	A_pCountDualPulse->span = 0;
}

//====================================================================
// 初始化双脉冲计数。
//====================================================================
REAL CountDualPulse(tCountDualPulse * A_pCountDualPulse, 
		U1 A,
		U1 B)
{
	A_pCountDualPulse->span += 1;
	if ( A_pCountDualPulse->span >= 1000 ) A_pCountDualPulse->span = 1000;
	if ( A_pCountDualPulse->oldstate == A_pCountDualPulse->state ) {
		A_pCountDualPulse->oldstate = A_pCountDualPulse->state;
		A_pCountDualPulse->step = 10.0/1000;;
		A_pCountDualPulse->span = 0;
	}

    switch ( A_pCountDualPulse->state ) {
    case AllFalse:
        if ( A && ( !B ) ) {
            A_pCountDualPulse->state = CwInc;
        } else if ( ( !A ) && B  ) {
            A_pCountDualPulse->state = CcwInc;
        }
        break;
    case ATBF:
        if ( A && B  ) {
            A_pCountDualPulse->state = CwInc;
        } else if ( ( ! A ) && ( ! B ) ) {
            A_pCountDualPulse->state = CcwInc;
        }
        break;
    case AllTrue:
        if ( ( ! A ) && B  ) {
            A_pCountDualPulse->state = CwInc;
        } else if ( A && ( ! B ) ) {
            A_pCountDualPulse->state = CcwInc;
        }
        break;
    case AFBT:
        if ( ( ! A ) && ( ! B ) ) {
            A_pCountDualPulse->state = CwInc;
        } else if ( A && B  ) {
            A_pCountDualPulse->state = CcwInc;
        }
        break;
    case CwInc:
        A_pCountDualPulse->value += A_pCountDualPulse->step;
        if ( A && B  ) {
            A_pCountDualPulse->state = AllTrue;
        } else if ( ( ! A ) && ( ! B ) ) {
            A_pCountDualPulse->state = AllFalse;
        } else if ( A && ( ! B ) ) {
            A_pCountDualPulse->state = ATBF;
        } else if ( ( ! A ) && B  ) {
            A_pCountDualPulse->state = AFBT;
        }
        break;
    case CcwInc:
        A_pCountDualPulse->value -= A_pCountDualPulse->step;
        if ( A && B  ) {
            A_pCountDualPulse->state = AllTrue;
        } else if ( ( ! A ) && ( ! B ) ) {
            A_pCountDualPulse->state = AllFalse;
        } else if ( A && ( ! B ) ) {
            A_pCountDualPulse->state = ATBF;
        } else if ( ( ! A ) && B  ) {
            A_pCountDualPulse->state = AFBT;
        }
        break;
    }
	return A_pCountDualPulse->value;
}

REAL 
Wrap2PI ( REAL angle )
{
	while ( angle > pi ) angle = angle - 2*pi;
	while ( angle < -pi ) angle = angle + 2*pi;
	return angle;
}


REAL 
Wrap22PI ( REAL angle )
{
	while ( angle > 2*pi ) angle = angle - 2*pi;
	while ( angle < 0 ) angle = angle + 2*pi;
	return angle;
}


