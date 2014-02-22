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


#include <errno.h>
#include <bsp.h>
#include <core/time.h>
#include <core/sched.h>

/* From platform.  */
#define BUS_FREQ (100 * 1000000U)

#define FREQ_DIV 40

/* Last time when decr was set.  */
static unsigned long long time_last;

/* Decrementer optimal value.  */
static unsigned int time_inter;

/* Correctly get time base - handle carry.  */
static inline unsigned long long get_ppc_tb (void)
{
  unsigned int u;
  unsigned int l;
  unsigned int u1;

  asm ("1:\n\t"
       "mftbu %0\n\t"
       "mftb  %1\n\t"
       "mftbu %2\n\t"
       "cmpw  %2,%0\n\t"
       "bne   1b\n"
       : "=r"(u), "=r"(l), "=r"(u1));
  return (((unsigned long long)u) << 32) | l;
}

/* Compute new value for the decrementer.  If the value is in the future,
   sets the decrementer else returns an error.  */
static int pok_arch_set_decr (void)
{
  unsigned long long time_new = time_last + time_inter;
  unsigned long long time_cur = get_ppc_tb();
  int delta = time_new - time_cur;

  time_last = time_new;

  if (delta < 0)
  {
    return POK_ERRNO_EINVAL;
  }
  else
  {
    asm volatile ("mtdec %0" : : "r"(delta));
    return POK_ERRNO_OK;
  }
}

/* Called by the interrupt handled.  */
void pok_arch_decr_int (void)
{
  int err;

  do
  {
    err = pok_arch_set_decr();

    pok_tick_counter += FREQ_DIV;
  } while (err != POK_ERRNO_OK);

  pok_sched ();
}

pok_ret_t pok_bsp_time_init ()
{
  time_inter = (BUS_FREQ * FREQ_DIV) / POK_TIMER_FREQUENCY;
  time_last = get_ppc_tb ();
  pok_arch_set_decr();

  return (POK_ERRNO_OK);
}
