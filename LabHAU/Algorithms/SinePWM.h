#ifndef SINEPWM_H
#define	SINEPWM_H

#include <stdint.h>

void SinePWM_Init(uint32_t freq, void (*FncCb)(void*));
void SinePWM_Process(void);

#endif
