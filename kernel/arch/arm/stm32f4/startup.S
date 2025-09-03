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

/**
 * \file    arch/arm/stm32f4/startup.S
 * \author  POK team
 * \brief   STM32F4 startup code and vector table
 */

.syntax unified
.cpu cortex-m4
.thumb

/* Stack top provided by linker script - no duplicate allocation */
.extern _estack  /* Use linker-provided stack top symbol */

/* Vector table - must be power-of-two aligned for VTOR */
.section ".isr_vector", "a"
.align 9  /* 2^9 = 512 bytes, required for 98-vector table */
.globl vector_table
vector_table:
    .word   _estack                     /* Initial Stack Pointer from linker */
    .word   Reset_Handler               /* Reset Handler */
    .word   NMI_Handler                 /* NMI Handler */
    .word   HardFault_Handler           /* Hard Fault Handler */
    .word   MemManage_Handler           /* Memory Management Fault Handler */
    .word   BusFault_Handler            /* Bus Fault Handler */
    .word   UsageFault_Handler          /* Usage Fault Handler */
    .word   0                           /* Reserved */
    .word   0                           /* Reserved */
    .word   0                           /* Reserved */
    .word   0                           /* Reserved */
    .word   SVC_Handler                 /* SVCall Handler */
    .word   DebugMon_Handler            /* Debug Monitor Handler */
    .word   0                           /* Reserved */
    .word   PendSV_Handler              /* PendSV Handler */
    .word   SysTick_Handler             /* SysTick Handler */

    /* External interrupts (STM32F4 specific) */
    .word   WWDG_IRQHandler                 /* Window WatchDog */
    .word   PVD_IRQHandler                  /* PVD through EXTI Line detection */
    .word   TAMP_STAMP_IRQHandler           /* Tamper and TimeStamps through the EXTI line */
    .word   RTC_WKUP_IRQHandler             /* RTC Wakeup through the EXTI line */
    .word   FLASH_IRQHandler                /* FLASH */
    .word   RCC_IRQHandler                  /* RCC */
    .word   EXTI0_IRQHandler                /* EXTI Line0 */
    .word   EXTI1_IRQHandler                /* EXTI Line1 */
    .word   EXTI2_IRQHandler                /* EXTI Line2 */
    .word   EXTI3_IRQHandler                /* EXTI Line3 */
    .word   EXTI4_IRQHandler                /* EXTI Line4 */
    .word   DMA1_Stream0_IRQHandler         /* DMA1 Stream 0 */
    .word   DMA1_Stream1_IRQHandler         /* DMA1 Stream 1 */
    .word   DMA1_Stream2_IRQHandler         /* DMA1 Stream 2 */
    .word   DMA1_Stream3_IRQHandler         /* DMA1 Stream 3 */
    .word   DMA1_Stream4_IRQHandler         /* DMA1 Stream 4 */
    .word   DMA1_Stream5_IRQHandler         /* DMA1 Stream 5 */
    .word   DMA1_Stream6_IRQHandler         /* DMA1 Stream 6 */
    .word   ADC_IRQHandler                  /* ADC1, ADC2 and ADC3s */
    .word   CAN1_TX_IRQHandler              /* CAN1 TX */
    .word   CAN1_RX0_IRQHandler             /* CAN1 RX0 */
    .word   CAN1_RX1_IRQHandler             /* CAN1 RX1 */
    .word   CAN1_SCE_IRQHandler             /* CAN1 SCE */
    .word   EXTI9_5_IRQHandler              /* External Line[9:5]s */
    .word   TIM1_BRK_TIM9_IRQHandler        /* TIM1 Break and TIM9 */
    .word   TIM1_UP_TIM10_IRQHandler        /* TIM1 Update and TIM10 */
    .word   TIM1_TRG_COM_TIM11_IRQHandler   /* TIM1 Trigger and Commutation and TIM11 */
    .word   TIM1_CC_IRQHandler              /* TIM1 Capture Compare */
    .word   TIM2_IRQHandler                 /* TIM2 */
    .word   TIM3_IRQHandler                 /* TIM3 */
    .word   TIM4_IRQHandler                 /* TIM4 */
    .word   I2C1_EV_IRQHandler              /* I2C1 Event */
    .word   I2C1_ER_IRQHandler              /* I2C1 Error */
    .word   I2C2_EV_IRQHandler              /* I2C2 Event */
    .word   I2C2_ER_IRQHandler              /* I2C2 Error */
    .word   SPI1_IRQHandler                 /* SPI1 */
    .word   SPI2_IRQHandler                 /* SPI2 */
    .word   USART1_IRQHandler               /* USART1 */
    .word   USART2_IRQHandler               /* USART2 */
    .word   USART3_IRQHandler               /* USART3 */
    .word   EXTI15_10_IRQHandler            /* External Line[15:10]s */
    .word   RTC_Alarm_IRQHandler            /* RTC Alarm (A and B) through EXTI Line */
    .word   OTG_FS_WKUP_IRQHandler          /* USB OTG FS Wakeup through EXTI line */
    .word   TIM8_BRK_TIM12_IRQHandler       /* TIM8 Break and TIM12 */
    .word   TIM8_UP_TIM13_IRQHandler        /* TIM8 Update and TIM13 */
    .word   TIM8_TRG_COM_TIM14_IRQHandler   /* TIM8 Trigger and Commutation and TIM14 */
    .word   TIM8_CC_IRQHandler              /* TIM8 Capture Compare */
    .word   DMA1_Stream7_IRQHandler         /* DMA1 Stream7 */
    .word   FSMC_IRQHandler                 /* FSMC */
    .word   SDIO_IRQHandler                 /* SDIO */
    .word   TIM5_IRQHandler                 /* TIM5 */
    .word   SPI3_IRQHandler                 /* SPI3 */
    .word   UART4_IRQHandler                /* UART4 */
    .word   UART5_IRQHandler                /* UART5 */
    .word   TIM6_DAC_IRQHandler             /* TIM6 and DAC1&2 underrun errors */
    .word   TIM7_IRQHandler                 /* TIM7 */
    .word   DMA2_Stream0_IRQHandler         /* DMA2 Stream 0 */
    .word   DMA2_Stream1_IRQHandler         /* DMA2 Stream 1 */
    .word   DMA2_Stream2_IRQHandler         /* DMA2 Stream 2 */
    .word   DMA2_Stream3_IRQHandler         /* DMA2 Stream 3 */
    .word   DMA2_Stream4_IRQHandler         /* DMA2 Stream 4 */
    .word   ETH_IRQHandler                  /* Ethernet */
    .word   ETH_WKUP_IRQHandler             /* Ethernet Wakeup through EXTI line */
    .word   CAN2_TX_IRQHandler              /* CAN2 TX */
    .word   CAN2_RX0_IRQHandler             /* CAN2 RX0 */
    .word   CAN2_RX1_IRQHandler             /* CAN2 RX1 */
    .word   CAN2_SCE_IRQHandler             /* CAN2 SCE */
    .word   OTG_FS_IRQHandler               /* USB OTG FS */
    .word   DMA2_Stream5_IRQHandler         /* DMA2 Stream 5 */
    .word   DMA2_Stream6_IRQHandler         /* DMA2 Stream 6 */
    .word   DMA2_Stream7_IRQHandler         /* DMA2 Stream 7 */
    .word   USART6_IRQHandler               /* USART6 */
    .word   I2C3_EV_IRQHandler              /* I2C3 event */
    .word   I2C3_ER_IRQHandler              /* I2C3 error */
    .word   OTG_HS_EP1_OUT_IRQHandler       /* USB OTG HS End Point 1 Out */
    .word   OTG_HS_EP1_IN_IRQHandler        /* USB OTG HS End Point 1 In */
    .word   OTG_HS_WKUP_IRQHandler          /* USB OTG HS Wakeup through EXTI */
    .word   OTG_HS_IRQHandler               /* USB OTG HS */
    .word   DCMI_IRQHandler                 /* DCMI */
    .word   CRYP_IRQHandler                 /* CRYP crypto */
    .word   HASH_RNG_IRQHandler             /* Hash and Rng */
    .word   FPU_IRQHandler                  /* FPU */
    .word   UART7_IRQHandler                /* UART7 */
    .word   UART8_IRQHandler                /* UART8 */
    .word   SPI4_IRQHandler                 /* SPI4 */
    .word   SPI5_IRQHandler                 /* SPI5 */
    .word   SPI6_IRQHandler                 /* SPI6 */
    .word   SAI1_IRQHandler                 /* SAI1 */
    .word   LTDC_IRQHandler                 /* LTDC */
    .word   LTDC_ER_IRQHandler              /* LTDC error */
    .word   DMA2D_IRQHandler                /* DMA2D */

.text
.thumb
.align 2

/* Reset handler */
.globl Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
    /* Set stack pointer */
    ldr r0, =_estack
    mov sp, r0

    /* Point VTOR to our vector table */
    ldr r0, =0xE000ED08       /* SCB->VTOR */
    ldr r1, =vector_table     /* must be 512-byte aligned */
    str r1, [r0]
    dsb
    isb

    /* Initialize data section */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    bl  copy_data

    /* Initialize BSS section */
    ldr r0, =_sbss
    ldr r1, =_ebss
    bl  clear_bss

    /* Call main POK kernel initialization */
    bl  pok_boot

    /* Should never reach here */
hang:
    b   hang

/* Copy data section from flash to RAM */
copy_data:
    cmp r0, r1            /* Compare start and end */
    bge copy_data_done    /* If start >= end, skip */
copy_data_loop:
    ldr r3, [r2]          /* Load from flash */
    str r3, [r0]          /* Store to RAM */
    adds r0, r0, #4       /* Increment RAM pointer */
    adds r2, r2, #4       /* Increment flash pointer */
    cmp r0, r1            /* Compare with end address */
    blt copy_data_loop    /* Continue if not at end */
copy_data_done:
    bx lr

/* Clear BSS section */
clear_bss:
    movs r2, #0           /* Clear value */
    cmp r0, r1            /* Compare start and end */
    bge clear_bss_done    /* If start >= end, skip */
clear_bss_loop:
    str r2, [r0]          /* Clear word */
    adds r0, r0, #4       /* Increment pointer */
    cmp r0, r1            /* Compare with end address */
    blt clear_bss_loop    /* Continue if not at end */
clear_bss_done:
    bx lr

/* Default handler for unused interrupts */
.globl Default_Handler
.type Default_Handler, %function
Default_Handler:
    b Default_Handler

/* Weak aliases for exception handlers */
.weak NMI_Handler
.thumb_set NMI_Handler, Default_Handler

.weak HardFault_Handler
.thumb_set HardFault_Handler, Default_Handler

.weak MemManage_Handler
.thumb_set MemManage_Handler, Default_Handler

.weak BusFault_Handler
.thumb_set BusFault_Handler, Default_Handler

.weak UsageFault_Handler
.thumb_set UsageFault_Handler, Default_Handler

.weak SVC_Handler
.thumb_set SVC_Handler, Default_Handler

.weak DebugMon_Handler
.thumb_set DebugMon_Handler, Default_Handler

.weak PendSV_Handler
.thumb_set PendSV_Handler, Default_Handler

.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler

/* External interrupt handlers - weak symbols can be overridden */
.weak WWDG_IRQHandler
.thumb_set WWDG_IRQHandler, Default_Handler
.weak PVD_IRQHandler
.thumb_set PVD_IRQHandler, Default_Handler
.weak TAMP_STAMP_IRQHandler
.thumb_set TAMP_STAMP_IRQHandler, Default_Handler
.weak RTC_WKUP_IRQHandler
.thumb_set RTC_WKUP_IRQHandler, Default_Handler
.weak FLASH_IRQHandler
.thumb_set FLASH_IRQHandler, Default_Handler
.weak RCC_IRQHandler
.thumb_set RCC_IRQHandler, Default_Handler
.weak EXTI0_IRQHandler
.thumb_set EXTI0_IRQHandler, Default_Handler
.weak EXTI1_IRQHandler
.thumb_set EXTI1_IRQHandler, Default_Handler
.weak EXTI2_IRQHandler
.thumb_set EXTI2_IRQHandler, Default_Handler
.weak EXTI3_IRQHandler
.thumb_set EXTI3_IRQHandler, Default_Handler
.weak EXTI4_IRQHandler
.thumb_set EXTI4_IRQHandler, Default_Handler
.weak DMA1_Stream0_IRQHandler
.thumb_set DMA1_Stream0_IRQHandler, Default_Handler
.weak DMA1_Stream1_IRQHandler
.thumb_set DMA1_Stream1_IRQHandler, Default_Handler
.weak DMA1_Stream2_IRQHandler
.thumb_set DMA1_Stream2_IRQHandler, Default_Handler
.weak DMA1_Stream3_IRQHandler
.thumb_set DMA1_Stream3_IRQHandler, Default_Handler
.weak DMA1_Stream4_IRQHandler
.thumb_set DMA1_Stream4_IRQHandler, Default_Handler
.weak DMA1_Stream5_IRQHandler
.thumb_set DMA1_Stream5_IRQHandler, Default_Handler
.weak DMA1_Stream6_IRQHandler
.thumb_set DMA1_Stream6_IRQHandler, Default_Handler
.weak ADC_IRQHandler
.thumb_set ADC_IRQHandler, Default_Handler
.weak CAN1_TX_IRQHandler
.thumb_set CAN1_TX_IRQHandler, Default_Handler
.weak CAN1_RX0_IRQHandler
.thumb_set CAN1_RX0_IRQHandler, Default_Handler
.weak CAN1_RX1_IRQHandler
.thumb_set CAN1_RX1_IRQHandler, Default_Handler
.weak CAN1_SCE_IRQHandler
.thumb_set CAN1_SCE_IRQHandler, Default_Handler
.weak EXTI9_5_IRQHandler
.thumb_set EXTI9_5_IRQHandler, Default_Handler
.weak TIM1_BRK_TIM9_IRQHandler
.thumb_set TIM1_BRK_TIM9_IRQHandler, Default_Handler
.weak TIM1_UP_TIM10_IRQHandler
.thumb_set TIM1_UP_TIM10_IRQHandler, Default_Handler
.weak TIM1_TRG_COM_TIM11_IRQHandler
.thumb_set TIM1_TRG_COM_TIM11_IRQHandler, Default_Handler
.weak TIM1_CC_IRQHandler
.thumb_set TIM1_CC_IRQHandler, Default_Handler
.weak TIM2_IRQHandler
.thumb_set TIM2_IRQHandler, Default_Handler
.weak TIM3_IRQHandler
.thumb_set TIM3_IRQHandler, Default_Handler
.weak TIM4_IRQHandler
.thumb_set TIM4_IRQHandler, Default_Handler
.weak I2C1_EV_IRQHandler
.thumb_set I2C1_EV_IRQHandler, Default_Handler
.weak I2C1_ER_IRQHandler
.thumb_set I2C1_ER_IRQHandler, Default_Handler
.weak I2C2_EV_IRQHandler
.thumb_set I2C2_EV_IRQHandler, Default_Handler
.weak I2C2_ER_IRQHandler
.thumb_set I2C2_ER_IRQHandler, Default_Handler
.weak SPI1_IRQHandler
.thumb_set SPI1_IRQHandler, Default_Handler
.weak SPI2_IRQHandler
.thumb_set SPI2_IRQHandler, Default_Handler
.weak USART1_IRQHandler
.thumb_set USART1_IRQHandler, Default_Handler
.weak USART2_IRQHandler
.thumb_set USART2_IRQHandler, Default_Handler
.weak USART3_IRQHandler
.thumb_set USART3_IRQHandler, Default_Handler
.weak EXTI15_10_IRQHandler
.thumb_set EXTI15_10_IRQHandler, Default_Handler
.weak RTC_Alarm_IRQHandler
.thumb_set RTC_Alarm_IRQHandler, Default_Handler
.weak OTG_FS_WKUP_IRQHandler
.thumb_set OTG_FS_WKUP_IRQHandler, Default_Handler
.weak TIM8_BRK_TIM12_IRQHandler
.thumb_set TIM8_BRK_TIM12_IRQHandler, Default_Handler
.weak TIM8_UP_TIM13_IRQHandler
.thumb_set TIM8_UP_TIM13_IRQHandler, Default_Handler
.weak TIM8_TRG_COM_TIM14_IRQHandler
.thumb_set TIM8_TRG_COM_TIM14_IRQHandler, Default_Handler
.weak TIM8_CC_IRQHandler
.thumb_set TIM8_CC_IRQHandler, Default_Handler
.weak DMA1_Stream7_IRQHandler
.thumb_set DMA1_Stream7_IRQHandler, Default_Handler
.weak FSMC_IRQHandler
.thumb_set FSMC_IRQHandler, Default_Handler
.weak SDIO_IRQHandler
.thumb_set SDIO_IRQHandler, Default_Handler
.weak TIM5_IRQHandler
.thumb_set TIM5_IRQHandler, Default_Handler
.weak SPI3_IRQHandler
.thumb_set SPI3_IRQHandler, Default_Handler
.weak UART4_IRQHandler
.thumb_set UART4_IRQHandler, Default_Handler
.weak UART5_IRQHandler
.thumb_set UART5_IRQHandler, Default_Handler
.weak TIM6_DAC_IRQHandler
.thumb_set TIM6_DAC_IRQHandler, Default_Handler
.weak TIM7_IRQHandler
.thumb_set TIM7_IRQHandler, Default_Handler
.weak DMA2_Stream0_IRQHandler
.thumb_set DMA2_Stream0_IRQHandler, Default_Handler
.weak DMA2_Stream1_IRQHandler
.thumb_set DMA2_Stream1_IRQHandler, Default_Handler
.weak DMA2_Stream2_IRQHandler
.thumb_set DMA2_Stream2_IRQHandler, Default_Handler
.weak DMA2_Stream3_IRQHandler
.thumb_set DMA2_Stream3_IRQHandler, Default_Handler
.weak DMA2_Stream4_IRQHandler
.thumb_set DMA2_Stream4_IRQHandler, Default_Handler
.weak ETH_IRQHandler
.thumb_set ETH_IRQHandler, Default_Handler
.weak ETH_WKUP_IRQHandler
.thumb_set ETH_WKUP_IRQHandler, Default_Handler
.weak CAN2_TX_IRQHandler
.thumb_set CAN2_TX_IRQHandler, Default_Handler
.weak CAN2_RX0_IRQHandler
.thumb_set CAN2_RX0_IRQHandler, Default_Handler
.weak CAN2_RX1_IRQHandler
.thumb_set CAN2_RX1_IRQHandler, Default_Handler
.weak CAN2_SCE_IRQHandler
.thumb_set CAN2_SCE_IRQHandler, Default_Handler
.weak OTG_FS_IRQHandler
.thumb_set OTG_FS_IRQHandler, Default_Handler
.weak DMA2_Stream5_IRQHandler
.thumb_set DMA2_Stream5_IRQHandler, Default_Handler
.weak DMA2_Stream6_IRQHandler
.thumb_set DMA2_Stream6_IRQHandler, Default_Handler
.weak DMA2_Stream7_IRQHandler
.thumb_set DMA2_Stream7_IRQHandler, Default_Handler
.weak USART6_IRQHandler
.thumb_set USART6_IRQHandler, Default_Handler
.weak I2C3_EV_IRQHandler
.thumb_set I2C3_EV_IRQHandler, Default_Handler
.weak I2C3_ER_IRQHandler
.thumb_set I2C3_ER_IRQHandler, Default_Handler
.weak OTG_HS_EP1_OUT_IRQHandler
.thumb_set OTG_HS_EP1_OUT_IRQHandler, Default_Handler
.weak OTG_HS_EP1_IN_IRQHandler
.thumb_set OTG_HS_EP1_IN_IRQHandler, Default_Handler
.weak OTG_HS_WKUP_IRQHandler
.thumb_set OTG_HS_WKUP_IRQHandler, Default_Handler
.weak OTG_HS_IRQHandler
.thumb_set OTG_HS_IRQHandler, Default_Handler
.weak DCMI_IRQHandler
.thumb_set DCMI_IRQHandler, Default_Handler
.weak CRYP_IRQHandler
.thumb_set CRYP_IRQHandler, Default_Handler
.weak HASH_RNG_IRQHandler
.thumb_set HASH_RNG_IRQHandler, Default_Handler
.weak FPU_IRQHandler
.thumb_set FPU_IRQHandler, Default_Handler
.weak UART7_IRQHandler
.thumb_set UART7_IRQHandler, Default_Handler
.weak UART8_IRQHandler
.thumb_set UART8_IRQHandler, Default_Handler
.weak SPI4_IRQHandler
.thumb_set SPI4_IRQHandler, Default_Handler
.weak SPI5_IRQHandler
.thumb_set SPI5_IRQHandler, Default_Handler
.weak SPI6_IRQHandler
.thumb_set SPI6_IRQHandler, Default_Handler
.weak SAI1_IRQHandler
.thumb_set SAI1_IRQHandler, Default_Handler
.weak LTDC_IRQHandler
.thumb_set LTDC_IRQHandler, Default_Handler
.weak LTDC_ER_IRQHandler
.thumb_set LTDC_ER_IRQHandler, Default_Handler
.weak DMA2D_IRQHandler
.thumb_set DMA2D_IRQHandler, Default_Handler

.end
