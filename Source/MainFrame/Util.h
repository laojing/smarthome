// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// 
//  项目名    ：实验室小模型
// 
//  文件描述  ：一些常用函数声明。
// 
//  修改记录  ：
// 
//  创建日期  ：2012年11月21日
// 
//  作者      ：沈阳工业大学 老井 laojingsut@gmail.com
// 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef UTIL_H_
#define UTIL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef pi
    #define pi 3.141592653f
#endif

#ifndef false
    #define false 0
#endif

#ifndef true
    #define true !false
#endif

#ifndef bool
    #define bool unsigned char
#endif

#ifndef MAX_PATH
    #define MAX_PATH 256
#endif

#ifndef toupper
#define toupper(c)    ((('a' <= (c))&&((c) <= 'z')) ? ((c) - 'a' + 'A') : (c))
#endif
#ifndef tolower
#define tolower(c)    ((('A' <= (c))&&((c) <= 'Z')) ? ((c) - 'A' + 'a') : (c))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX max
#endif
#ifndef MIN
#define MIN min
#endif
#ifndef ROLLOVER_INCREMENT
#define ROLLOVER_INCREMENT(a, upperbound) (((((a)+1) >= (upperbound)) ? 0 : (a)+1))
#endif
#ifndef ROLLOVER_DECREMENT
#define ROLLOVER_DECREMENT(a, upperbound) ((((a) <= 0) ? ((upperbound)-1) : (a)-1))
#endif
#ifndef ROLLOVER_ADD
#define ROLLOVER_ADD(a, b, upperbound) (((a)+(b)) >= (upperbound)) ? (((a)+(b))-(upperbound)) : ((a)+(b))
#endif

#define ushort unsigned short
#define uint unsigned int


#define LIMIT(x,a,b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define NINT(a) ((a) >= 0.0 ? (int)((a)+0.5) : (int)((a)-0.5))
#define LINELENGTH 1024
#define MAXTLOOKUPLENGTH 10
#define TRACE printf
#define TRACE_ERROR printf
#define TRACE1 printf
#define TRACE2 printf
#define TRACE3 printf
#define TRACE4 printf

typedef union
{
  bool b;
  short s;
  unsigned short us;
  int i;
  unsigned int ui;
  float f;
} TypeConverter;



#define BIT_00 0x01
#define BIT_01 0x02
#define BIT_02 0x04
#define BIT_03 0x08
#define BIT_04 0x10
#define BIT_05 0x20
#define BIT_06 0x40
#define BIT_07 0x80
#define BIT_08 0x100
#define BIT_09 0x200
#define BIT_10 0x400
#define BIT_11 0x800
#define BIT_12 0x1000
#define BIT_13 0x2000
#define BIT_14 0x4000
#define BIT_15 0x8000
#define BIT_16 0x10000
#define BIT_17 0x20000
#define BIT_18 0x40000
#define BIT_19 0x80000
#define BIT_20 0x100000
#define BIT_21 0x200000
#define BIT_22 0x400000
#define BIT_23 0x800000
#define BIT_24 0x1000000
#define BIT_25 0x2000000
#define BIT_26 0x4000000
#define BIT_27 0x8000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

#define SWAP_SHORT(_a) (((_a)>>8)&0xFF) | (((_a)&0xFF)<<8)

short SwapShort(short a);
unsigned short SwapUshort(unsigned short a);
void Reverse(void *pData, size_t len);
bool TolEq(float A_fVal1, float A_fVal2, float A_fTolerance);
bool TolGt(float A_fVal1, float A_fVal2, float A_fTolerance);
bool TolLt(float A_fVal1, float A_fVal2, float A_fTolerance);
bool Ramp(float A_fAt, float A_fCycleTime, float A_fTo, float A_fCurrent, float * A_fpNew);

#endif
