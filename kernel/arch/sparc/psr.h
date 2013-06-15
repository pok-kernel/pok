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
 * Created by julien on Thu Jan 15 23:34:13 2009
 */

/**
 * @file
 * @author Fabien Chouteau
 * @brief  Processor State Register utils.
 */

#ifndef __POK_SPARC_PSR_H__
#define __POK_SPARC_PSR_H__

#define PSR_ET        0x20  /**< enable traps */
#define PSR_PS        0x40  /**< previous supervisor */
#define PSR_S         0x80  /**< supervisor */
#define PSR_CWP_MASK  0x1F  /**< Current Window Pointer Mask */
#define PSR_PIL(pil)  (((pil) & 0xF) << 8)  /**< Proc Interrupt Level */

static inline unsigned int psr_get(void)
{
  unsigned int psr;
  asm volatile ("rd %%psr, %0\n"
                : "=r" (psr)
                : /* no inputs */
                : "memory");

  return psr;
}

static inline void psr_set(unsigned int new_psr)
{
  asm volatile ("wr %0, 0x0, %%psr\n"
                "nop\n"
                "nop\n"
                "nop\n"
                : /* no outputs */
                : "r" (new_psr)
                : "memory", "cc");
}

static inline void psr_enable_traps(void)
{
  unsigned int psr = psr_get();
  psr |= PSR_ET;
  psr_set(psr);
}

static inline void psr_disable_traps(void)
{
  unsigned int psr = psr_get();
  psr &= ~PSR_ET;
  psr_set(psr);
}

static inline void psr_disable_interupt(void)
{
  unsigned int psr = psr_get();

  psr &= ~(0xF << 8);
  psr_set(psr | PSR_PIL(0xF));
}

static inline void psr_enable_interupt(void)
{
  unsigned int psr = psr_get();

  psr &= ~(0xF << 8);
  psr_set(psr | PSR_PIL(0x0));
}

#endif
