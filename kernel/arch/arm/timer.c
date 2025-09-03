/* Generic ARM timer placeholder; real targets override with strong symbols.
 *
 * WEAK SYMBOL DOCUMENTATION:
 *
 * These weak implementations provide default no-op behavior for platforms
 * that don't require custom timer functionality. Real target implementations
 * should provide strong symbols that override these defaults.
 *
 * Override expectations:
 * - STM32F4 targets: See stm32f4/timer.c for strong implementation
 * - Other ARM Cortex-M targets: Must provide own timer.c with strong symbols
 * - pok_timer_init(): Should initialize platform-specific timer hardware
 * - pok_timer_handler(): Should handle timer interrupts and update
 * pok_tick_counter
 *
 * Impact if not overridden:
 * - System will compile and link successfully
 * - No timer interrupts will be generated
 * - Scheduling timeouts and delays will not work
 * - Real-time behavior will be lost
 *
 * To override: Create timer.c in your platform directory with the same function
 * signatures but without __attribute__((weak)).
 */
#include <errno.h>
#include <stdint.h>
#include <types.h>

__attribute__((weak)) pok_ret_t pok_timer_init(void) {
  /* Default: no-op timer initialization */
  return POK_ERRNO_OK;
}

__attribute__((weak)) void pok_timer_handler(void) {
  /* Default: no-op timer interrupt handler */
}
