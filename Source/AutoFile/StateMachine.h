#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <math.h> 			
#include "ChannelDefs.h" 			
#include "StateFilter.h" 			
#include "../MainFrame/Main.h" 
typedef enum {
    Sunsidle = 0,
    Sunsopenin = 1,
    Sunsopening = 2,
    Sunsclosein = 3,
    Sunsclosing = 4,
} SunsStates;
typedef enum {
    SunsidleCat,
    SunsopenCat,
    SunscloseCat,
} SunsStateCategories;
void InitSunsStateMachine ();
void UpdateSunsStateMachine ();
typedef enum {
    Dengidle = 0,
    Dengbookopenin = 1,
    Dengbookopening = 2,
} DengStates;
typedef enum {
    DengidleCat,
    DengbookopenCat,
} DengStateCategories;
void InitDengStateMachine ();
void UpdateDengStateMachine ();
typedef enum {
    Humaidle = 0,
    Humatimeopen = 1,
    Humacmdopen = 2,
    Humaopen = 3,
    Humatimeclose = 4,
    Humacmdclose = 5,
    Humaclose = 6,
} HumaStates;
typedef enum {
    HumaidleCat,
    HumaopenCat,
    HumacloseCat,
} HumaStateCategories;
void InitHumaStateMachine ();
void UpdateHumaStateMachine ();
typedef enum {
    Respidle = 0,
    Resphumanon = 1,
} RespStates;
typedef enum {
    RespidleCat,
    RespchangCat,
} RespStateCategories;
void InitRespStateMachine ();
void UpdateRespStateMachine ();

#endif
