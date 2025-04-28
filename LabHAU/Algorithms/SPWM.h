#ifndef SPWM_H
#define	SPWM_H

#include <stdint.h>

void SPWM_Init(void (*FncCb)(void*));
void SPWM_Process(void);

#endif
