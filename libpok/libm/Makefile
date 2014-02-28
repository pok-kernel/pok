TOPDIR=		../../

include $(TOPDIR)/misc/mk/config.mk
-include $(TOPDIR)/misc/mk/common-$(ARCH).mk

LO_TARGET=libm.lo

LO_OBJS=	acos.o \
			acosf.o \
			acosh.o \
			acoshf.o \
			asinh.o \
         asinhf.o \
			atanh.o \
			atanhf.o \
			asin.o \
			asinf.o \
			atan2.o \
			atan2f.o \
         atan.o \
         atanf.o \
         cbrt.o \
         cbrtf.o \
         ceil.o \
         ceilf.o \
         copysign.o \
         copysignf.o \
         cos.o \
         cosf.o \
			cosh.o \
			coshf.o \
			drem.o \
			dremf.o \
			exp.o \
			expf.o \
			e_acos.o \
			e_acosf.o \
			e_acosh.o \
			e_acoshf.o \
			e_asin.o \
			e_asinf.o \
			e_atan2.o \
			e_atan2f.o \
			e_atanh.o \
			e_atanhf.o \
			e_cosh.o \
			e_coshf.o \
			e_exp.o \
			e_expf.o \
			e_fmod.o \
			e_fmodf.o \
			e_hypot.o \
			e_hypotf.o \
			e_j0.o \
			e_j0f.o \
			e_j1.o \
			e_j1f.o \
			e_jn.o \
			e_jnf.o \
			e_lgamma_r.o \
			e_lgammaf_r.o \
			e_log.o \
			e_log10.o \
			e_log10f.o \
			e_log2.o \
			e_log2f.o \
			e_logf.o \
			e_pow.o \
			e_powf.o \
			e_rem_pio2.o \
			e_rem_pio2f.o \
			e_remainder.o \
			e_remainderf.o \
			e_scalb.o \
			e_scalbf.o \
			e_sinh.o \
			e_sinhf.o \
			e_sqrt.o \
			e_sqrtf.o \
			k_cos.o \
			k_cosf.o \
			k_rem_pio2.o \
			k_rem_pio2f.o \
			k_sin.o \
			k_sinf.o \
			k_standard.o \
			k_tan.o \
			k_tanf.o \
			erf.o \
			erff.o \
			expm1.o \
			expm1f.o \
			fabs.o \
			fabsf.o \
			finite.o \
			finitef.o \
			floor.o \
			floorf.o \
			fmod.o \
			fmodf.o \
			fpclassify.o \
			frexp.o \
			frexpf.o \
			gamma.o \
			gammaf.o \
			gamma_r.o \
			gammaf_r.o \
			hypot.o \
			hypotf.o \
			infinity.o \
			ilogb.o \
			ilogbf.o \
			isnan.o \
			isnanf.o \
			j0.o \
			j0f.o \
			j1.o \
			j1f.o \
			jn.o \
			jnf.o \
			ldexp.o \
			ldexpf.o \
			log.o \
			logf.o \
			log2.o \
			log2f.o \
			log1p.o \
			log1pf.o \
			lgamma.o \
			lgammaf.o \
			lgamma_r.o \
			lgammaf_r.o \
			log10.o \
			log10f.o \
			matherr.o \
			modf.o \
			modff.o \
			nextafter.o \
			nextafterf.o \
			pow.o \
			powf.o \
			remainder.o \
			remainderf.o \
			rint.o \
			rintf.o \
			round.o \
			roundf.o \
			scalbf.o \
			scalb.o \
			scalbn.o \
			scalbnf.o \
			signgam.o \
			significand.o \
			significandf.o \
			sin.o \
			sinf.o \
			sinh.o \
			sinhf.o \
         sqrt.o \
         sqrtf.o \
         tan.o \
         tanf.o \
         tanh.o \
         tanhf.o \
			trunc.o \
			truncf.o

LO_DEPS=

all: $(LO_TARGET)

.PHONY: clean distclean depend all

clean:
	$(RM) $(LO_OBJS)

distclean:
	$(RM) $(LO_OBJS) $(LO_TARGET)
	$(RM) .depend.mk

depend:
	$(if $(LO_OBJS), $(CC) $(CFLAGS) -MM $(wildcard *.c) $(wildcard *.S) > .depend.mk,)

include $(TOPDIR)/misc/mk/rules-common.mk
include $(TOPDIR)/misc/mk/rules-partition.mk

