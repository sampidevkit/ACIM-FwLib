#ifndef TICK_H
#define TICK_H

#include "../Common/LibDef.h"
#include "config/labhau/labhau_cfg.h"

#ifndef TICK_PER_SEC
#define TICK_PER_SEC 60000000 // Clock source of core timer
#endif

#ifndef TICK_PER_MS
#define TICK_PER_MS  60000
#endif

#ifndef TICK_PER_US
#define TICK_PER_US  60
#endif

#define tick_t  uint32_t

typedef enum {
    US,
    MS,
    SEC
} tick_type_t;

typedef struct {
    tick_t Start;
    tick_t Duration;
} __attribute_packed__ elapse_t;

typedef struct {
    volatile bool Timeout;
    tick_t Start;
    tick_t Duration;
} __attribute_packed__ tick_cxt_t;

public void Tick_SetFncCallInDelay(simple_fnc_t pFnc);
public void Delay(tick_t Time, tick_type_t TickType); // Do not use in multi-task mode
public bool Tick_Is_Over(tick_cxt_t *pTick, tick_t Time, tick_type_t TickType);

static inline void Tick_Reset(tick_cxt_t *pTick) {
    pTick->Timeout = 1;
}

static inline tick_t Tick_Get_TickVal(void) {
    return (tick_t) _CP0_GET_COUNT();
}

#define Tick_Init(cb)                   Tick_SetFncCallInDelay(cb)
#define Delay_Us(Time)                  Delay(Time, US)
#define delay_us(t)                     Delay_Us(t)
#define __delay_us(t)                   Delay_Us(t)
#define Tick_GetTimeUs()                (Tick_Get_TickVal() / TICK_PER_US)
#define Tick_Is_Over_Us(pTick, Time)    Tick_Is_Over(&pTick, Time, US)

#define Delay_Ms(Time)                  Delay(Time, MS)
#define Delay_Sec(Time)                 Delay(Time, SEC)
#define delay_ms(t)                     Delay_Ms(t)
#define delay_sec(t)                    Delay_Sec(t)
#define __delay_ms(t)                   Delay_Ms(t)
#define __delay_sec(t)                  Delay_Sec(t)

#define Tick_Read()                     (_CP0_GET_COUNT())
#define Tick_GetTimeMs()                (Tick_Get_TickVal() / TICK_PER_MS)
#define Tick_GetTimeSec()               (Tick_Get_TickVal() / TICK_PER_SEC)
#define Tick_Is_Over_Ms(pTick, Time)    Tick_Is_Over(&pTick, Time, MS)
#define Tick_Is_Over_Sec(pTick, Time)   Tick_Is_Over(&pTick, Time, SEC)

#define Tick_Get()                      (_CP0_GET_COUNT())
#define Tick_GetUs(ms)                  (ms*TICK_PER_US)
#define Tick_Dif_Us(tk1, tk2)           (((tk1)-(tk2))/TICK_PER_US)

#define Tick_GetMs(ms)                  (ms*TICK_PER_MS)
#define Tick_GetSec(ms)                 (ms*TICK_PER_SEC)
#define Tick_Dif(tk1, tk2)              (tick_t)((tk1)-(tk2))
#define Tick_Dif_Ms(tk1, tk2)           (tick_t)(((tk1)-(tk2))/TICK_PER_MS)
#define Tick_Dif_Sec(tk1, tk2)          (tick_t)(((tk1)-(tk2))/TICK_PER_SEC)

#endif