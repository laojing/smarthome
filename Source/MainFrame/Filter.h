#ifndef FILTER_H_
#define FILTER_H_

#ifndef pi
    #define pi 3.141592653f
#endif

#ifndef false
    #define false 0
#endif

#ifndef true
    #define true !false
#endif

#define BOOL unsigned char
#define U1 unsigned char
#define S8 signed char
#define U8 unsigned char
#define S16 signed int
#define U16 unsigned int
#define S32 signed long
#define U32 unsigned long
#define REAL float

#define U16_MAX          65536

// 上升沿、下降沿
typedef struct {
    U1 last;
} tPosEdge, tNegEdge;
void InitPosEdge(tPosEdge * A_pPosEdge, U1 A_bIn);
U1 PosEdge(tPosEdge * A_pPosEdge, U1 A_bIn);
void InitNegEdge(tNegEdge * A_pNegEdge, U1 A_bIn);
U1 NegEdge(tNegEdge * A_pNegEdge, U1 A_bIn);

/// 平均值
typedef struct {
	REAL timeConstant;
	REAL movAve;
} tMovAve;
void InitMovAve(tMovAve * A_pMovAve, REAL A_fSampleTime, REAL A_fTimeConstant, REAL A_fIn);
REAL MovAve(tMovAve * A_pMovAve, REAL A_fIn);

/// 条件真持续时间
typedef struct {
    U16 trueSamples;
    REAL sampleTime;
} tTrueDuration;
void InitTrueDuration(tTrueDuration * A_pTrueDuration, REAL A_fSampleTime, U1 A_bIn);
REAL TrueDuration(tTrueDuration * A_pTrueDuration, U1 A_bIn);

/// 双脉冲计数
#define AllFalse         0
#define AllTrue          1
#define ATBF             2
#define AFBT             3
#define CwInc            4
#define CcwInc           5
typedef struct {
	U8 oldstate;
	U8 state;
	S16 start;
	S16 span;
    REAL step;
    REAL value;
} tCountDualPulse;
void InitCountDualPulse(tCountDualPulse * A_pCountDualPulse, REAL Step, REAL Init, U1 A, U1 B);
REAL CountDualPulse(tCountDualPulse * A_pCountDualPulse, U1 A, U1 B);

REAL Wrap2PI ( REAL angle );
REAL Wrap22PI ( REAL angle );

#endif  /*FILTER_H_*/
