/*
 *                               POK header
 * 
 * The following file is a part of the POK project. Any modification should
 * made according to the POK licence. You CANNOT use this file or a part of
 * this file is this part of a file for your own project
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2009 POK team 
 *
 * Created by julien on Fri Jan 30 13:44:27 2009 
 */


#ifdef POK_NEEDS_LIBMATH

#ifndef __POK_LIBM_H__
#define __POK_LIBM_H__


#include <types.h>

struct exception {
        int type;
        char *name;
        double arg1;
        double arg2;
        double retval;
};

#define FP_NAN          1
#define FP_INFINITE     2
#define FP_NORMAL       3
#define FP_SUBNORMAL    4
#define FP_ZERO         5

#define DOMAIN          1
#define SING            2
#define OVERFLOW        3
#define UNDERFLOW       4
#define TLOSS           5
#define PLOSS           6


#define fpclassify(x) (sizeof (x) == sizeof(float )  ?  __fpclassifyf((float)(x)) : __fpclassifyd((double)(x)))  

extern int __fpclassifyf(float      );
extern int __fpclassifyd(double     );
extern int __fpclassify (long double);


double   acos(double x);
float    acosf(float x);
double   acosh(double x);
float    acoshf(float x);
double   asin(double x);
float    asinf(float x);
double   asinh(double x);
float    asinhf(float x);
double   atan(double x);
float    atanf(float x);
double   atan2(double y, double x);
float    atan2f(float y, float x);
double   atanh(double x);
float    atanhf(float x);
double   cbrt(double x);
float    cbrtf(float x);
double   ceil(double x);
float    ceilf(float x);
double   copysign(double x, double y);
float    copysignf(float x, float y);
double   cos(double x);
float    cosf(float x);
double   cosh(double x);
float    coshf(float x);
double   drem(double x, double y);
float    dremf(float x, float y);
double   erf(double x);
float    erff(float x);
double   exp(double x);
float    expf(float x);
double   expm1(double x);
float    expm1f(float x);
double   fabs(double x);
float    fabsf(float x);
int      finite(double x);
int      finitef(float x);
double   floor(double x);
float    floorf(float x);
double   frexp(double x, int *eptr);
float    frexpf(float x, int *eptr);
double   gamma(double x);
float    gammaf(float x);
double   gamma_r(double x, int *signgamp);
float    gammaf_r(float x, int *signgamp);
double   hypot(double x, double y);
float    hypotf(float x, float y);
int      ilogb(double x);
int      ilogbf(float x);
int      isinf(double x);
int      isinff(float x);
int      isnan(double x);
int      isnanf(float x);
double   j0(double x);
float    j0f(float x);
double   j1(double x);
float    j1f(float x);
double   jn(int n, double x);
float    jnf(int n, float x);
double   ldexp(double value, int exp0);
float    ldexpf(float value, int exp0);
double   lgamma(double x);
float    lgammaf(float x);
double   lgamma_r(double x, int *signgamp);
float    lgammaf_r(float x, int *signgamp);
double   log(double x);
float    logf(float x);
double   log10(double x);
float    log10f(float x);
double   log2(double x);
float    log2f(float x);
double   logb(double x);
float    logbf(float x);
double   log1p(double x);
float    log1pf(float x);
double   ldexp(double value, int exp0);
float    ldexpf(float value, int exp0);
int      matherr(struct exception *x);
float    modff(float x, float *iptr);
double   modf(double x, double *iptr);
double   nextafter(double x, double y);
float    nextafterf(float x, float y);
double   pow(double x, double y);
float    powf(float x, float y);
double   remainder(double x, double y);
float    remainderf(float x, float y);
#ifdef _SCALB_INT
double   scalb(double x, int fn);
#else
double   scalb(double x, double fn);
#endif
#ifdef _SCALB_INT
float    scalbf(float x, int fn);
#else
float    scalbf(float x, float fn);
#endif
double   rint(double x);
float    rintf(float x);
double   round(double x);
float    roundf(float x);
double   scalbn(double x, int n);
float    scalbnf(float x, int n);
double   significand(double x);
float    significandf(float x);
double   sin(double x);
float    sinf(float x);
double   sinh(double x);
float    sinhf(float x);
double   sqrt(double x);
float    sqrtf(float x);
double   tan(double x);
float    tanf(float x);
double   tanh(double x);
float    tanhf(float x);
double   trunc(double x);
float    truncf(float x);

#endif

#endif /* POK_NEEDS_LIBMATH */
