#ifndef MOTORS_H
#define	MOTORS_H

#include "Common/LibDef.h"
#include "labhau_cfg.h"

typedef struct // Motor parameters in float value
{
    float Pole; // Number of pole pairs
    float Rs; // Per phase resistance in Ohms
    float Ld; // Direct axis inductance in Henry
    float Lq; // Quadrature axis inductance in Henry
    float Ke; // Back EMF constant in RMS volts-sec/rad
    float Imax; // Max RMS current in Ampe
    float Jm; // Motor inertia in Kg/cm^2
    float Nmax; // Maximum speed in rad/sec
    float Nrated; // Rated speed in rad/sec
} motor_fpara_t;

typedef struct // Motor parameters in integer value
{
    fraction_t Pole; // Number of pole pairs
    fraction_t Rs; // Per phase resistance in Ohms
    fraction_t Ld; // Direct axis inductance in Henry
    fraction_t Lq; // Quadrature axis inductance in Henry
    fraction_t Ke; // Back EMF constant in RMS volts-sec/rad
    fraction_t Imax; // Max RMS current in Ampe
    fraction_t Jm; // Motor inertia in Kg/cm^2
    fraction_t Nmax; // Maximum speed in rad/sec
    fraction_t Nrated; // Rated speed in rad/sec
} motor_ipara_t;

extern const motor_fpara_t MotorFParameters;
extern motor_ipara_t MotorIParameters;

void Motor_Init(void);

#endif
