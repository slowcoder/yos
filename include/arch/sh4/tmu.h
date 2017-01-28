#ifndef _TMU_H_
#define _TMU_H_

#include "yos.h"

/*
 * Some handy #defines to the different registers in the TMU
 */
#define TMU_TOCR  0xFFD80000
#define TMU_TSTR  0xFFD80004

#define TMU_TCOR0 0xFFD80008
#define TMU_TCNT0 0xFFD8000C
#define TMU_TCR0  0xFFD80010
#define TMU_TCOR1 0xFFD80014
#define TMU_TCNT1 0xFFD80018
#define TMU_TCR1  0xFFD8001C
#define TMU_TCOR2 0xFFD80020
#define TMU_TCNT2 0xFFD80024
#define TMU_TCR2  0xFFD80028
#define TMU_TCPR2 0xFFD8002C

void TMU_initialize(void);
void TMU_start_timer(uint8 timer);
void TMU_stop_timer(uint8 timer);

#endif
