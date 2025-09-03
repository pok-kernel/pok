/*
 *                               POK header
 *
 * The following file is a part of the POK project. Any modification should
 * be made according to the POK licence. You CANNOT use this file or a part
 * of a file for your own project.
 *
 * For more information on the POK licence, please see our LICENCE FILE
 *
 * Please follow the coding guidelines described in doc/CODING_GUIDELINES
 *
 *                                      Copyright (c) 2007-2025 POK team
 */

#ifndef __POK_ARM_NVIC_H__
#define __POK_ARM_NVIC_H__

#include "cortex_m_config.h"
#include <errno.h>
#include <types.h>

/* System Control Block (SCB) Base Address */
#define SCB_BASE 0xE000ED00

/* NVIC Base Address */
#define NVIC_BASE 0xE000E100

/* System Control Block Registers */
#define SCB_ICSR (*((volatile uint32_t *)(SCB_BASE + 0x04)))
#define SCB_VTOR (*((volatile uint32_t *)(SCB_BASE + 0x08)))
#define SCB_AIRCR (*((volatile uint32_t *)(SCB_BASE + 0x0C)))
#define SCB_SCR (*((volatile uint32_t *)(SCB_BASE + 0x10)))
#define SCB_CCR (*((volatile uint32_t *)(SCB_BASE + 0x14)))
#define SCB_SHCSR (*((volatile uint32_t *)(SCB_BASE + 0x24)))
#define SCB_SHPR1 (*((volatile uint32_t *)(SCB_BASE + 0x18)))
#define SCB_SHPR2 (*((volatile uint32_t *)(SCB_BASE + 0x1C)))
#define SCB_SHPR3 (*((volatile uint32_t *)(SCB_BASE + 0x20)))

/* NVIC Registers */
#define NVIC_ISER ((volatile uint32_t *)(NVIC_BASE + 0x000))
#define NVIC_ICER ((volatile uint32_t *)(NVIC_BASE + 0x080))
#define NVIC_ISPR ((volatile uint32_t *)(NVIC_BASE + 0x100))
#define NVIC_ICPR ((volatile uint32_t *)(NVIC_BASE + 0x180))
#define NVIC_IPR ((volatile uint8_t *)(NVIC_BASE + 0x300))

/* Exception Numbers */
#define EXCEPTION_RESET 1
#define EXCEPTION_NMI 2
#define EXCEPTION_HARDFAULT 3
#define EXCEPTION_MEMMANAGE 4
#define EXCEPTION_BUSFAULT 5
#define EXCEPTION_USAGEFAULT 6
#define EXCEPTION_SVCALL 11
#define EXCEPTION_DEBUGMON 12
#define EXCEPTION_PENDSV 14
#define EXCEPTION_SYSTICK 15

/* External interrupt start */
#define EXCEPTION_IRQ0 16

/* System Control Register bits */
#define SCB_SCR_SLEEPONEXIT (1 << 1)
#define SCB_SCR_SLEEPDEEP (1 << 2)
#define SCB_SCR_SEVONPEND (1 << 4)

/* Interrupt Control and State Register bits */
#define SCB_ICSR_PENDSTCLR (1 << 25)
#define SCB_ICSR_PENDSTSET (1 << 26)
#define SCB_ICSR_PENDSVCLR (1 << 27)
#define SCB_ICSR_PENDSVSET (1 << 28)

/* Configuration and Control Register bits */
#define SCB_CCR_DIV_0_TRP (1 << 4)

/* System Handler Control and State Register bits */
#define SCB_SHCSR_MEMFAULTENA (1 << 16)
#define SCB_SHCSR_BUSFAULTENA (1 << 17)
#define SCB_SHCSR_USGFAULTENA (1 << 18)

/* Application Interrupt and Reset Control Register */
#define SCB_AIRCR_VECTKEY 0x05FA0000
#define SCB_AIRCR_SYSRESETREQ (1 << 2)

/* Priority levels and masks - ARM_PRIORITY_MASK defined in arch.h */

/* Priority levels */
#define NVIC_PRIORITY_HIGHEST 0
#define NVIC_PRIORITY_HIGH 1
#define NVIC_PRIORITY_NORMAL 8
#define NVIC_PRIORITY_LOW 14
#define NVIC_PRIORITY_LOWEST 15

/* NVIC vector table configuration aliases */
#define NVIC_VECTOR_COUNT CORTEX_M_NVIC_VECTOR_COUNT
#define NVIC_VECTOR_TABLE_ALIGNMENT CORTEX_M_NVIC_VECTOR_TABLE_ALIGNMENT

/* Maximum external interrupt number */
#define NVIC_MAX_IRQ 239

/* Vector table entry */
typedef void (*vector_table_entry_t)(void);

/* Function prototypes */
pok_ret_t pok_nvic_init(void);
pok_ret_t pok_nvic_set_handler(uint8_t irq, void (*handler)(void));
pok_ret_t pok_nvic_enable_irq(uint8_t irq);
pok_ret_t pok_nvic_disable_irq(uint8_t irq);
pok_ret_t pok_nvic_set_priority(uint8_t irq, uint8_t priority);
pok_ret_t pok_nvic_clear_pending(uint8_t irq);
pok_ret_t pok_nvic_set_vector_table(uint32_t offset);

/* System exception handlers */
void Reset_Handler(void);
void NMI_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/* Fault handlers implemented in exceptions.c */
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);

#endif /* !__POK_ARM_NVIC_H__ */
