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
 * @brief  SPARC "ioports".
 * Use MMU bypass to access IO memory.
 */


#ifndef __POK_SPARC_LEON3_IOPORTS_H__
# define __POK_SPARC_LEON3_IOPORTS_H__

# include <types.h>
# include "sparc_conf.h"

static inline void outb(uint32_t addr, uint8_t data)
{
  asm volatile ("sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (data), "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
}

static inline void outw(uint32_t addr, uint32_t data)
{
  asm volatile ("sta %0, [%1] %2;\n"
                : /* no output */
                : "r" (data), "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
}

static inline uint8_t inb(uint32_t addr)
{
  uint8_t value = 0;
  asm volatile ("lduba [%1] %2, %0;\n"
                : "=r" (value)
                : "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
  return value;
}

static inline uint32_t inw(uint32_t addr)
{
  uint32_t value = 0;
  asm volatile ("lda [%1] %2, %0;\n"
                : "=r" (value)
                : "r" (addr), "i" (ASI_MMU_BYPASS)
                : "memory");
  return value;
}

#endif /* __POK_SPARC_LEON3_IOPORTS_H__ */
