#ifndef SVPWM_H
#define	SVPWM_H

#include <stdint.h>

void SVPWM_Init(void (*FncCb)(void*));
void SVPWM_Process(void);

#endif
