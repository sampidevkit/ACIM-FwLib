#ifndef FTDI_SERIALPORT_H
#define FTDI_SERIALPORT_H

#include <libftdi1/ftdi.h>
#include "Common/LibDef.h"
#include "labhau_cfg.h"

typedef struct {
    int vid;
    int pid;
    int baudrate;
    int interface;
} ftdi_sp_cxt_t;

extern ftdi_sp_cxt_t FtdiSpCxt; // See in FTDI_SerialPort_Cfg.c

bool FTDI_SerialPort_Init(void);
void FTDI_SerialPort_Deinit(void);

#endif
