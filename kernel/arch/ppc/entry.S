#include "msr.h"

        .section ".start", "ax"
#define FRAME_SIZE 80
        /* r4 is available, r1 is set, r3 contains cr,
           sprg3 contains r3 and sprg2 contains r2.  */
#define SAVE_REGS                                \
        /* Establish new frame.  */              \
        mflr    %r4;                             \
        stw     %r4,FRAME_SIZE+4(%r1);  /* lr */ \
        mfctr   %r4;                             \
        stw     %r4,64(%r1);  /* ctr */          \
        mfxer   %r4;                             \
        stw     %r4,68(%r1);  /* xer */          \
        stw     %r3,8(%r1);   /* cr */           \
        stw     %r0,12(%r1);  /* r0 */           \
        stw     %r2,16(%r1);  /* r2 */           \
        mfsprg  %r0,3;                           \
        stw     %r0,20(%r1);  /* r3 */           \
        mfsprg  %r0,2;                           \
        stw     %r0,24(%r1);  /* r4 */           \
        stw     %r5,28(%r1);  /* r5 */           \
        stw     %r6,32(%r1);  /* r6 */           \
        stw     %r7,36(%r1);  /* r7 */           \
        stw     %r8,40(%r1);  /* r8 */           \
        stw     %r9,44(%r1);  /* r9 */           \
        stw     %r10,48(%r1); /* r10 */          \
        stw     %r11,52(%r1); /* r11 */          \
        stw     %r12,56(%r1); /* r12 */          \
        stw     %r13,60(%r1); /* r13 */          \
        mfsrr0  %r3;                             \
        stw     %r3,72(%r1);  /* srr0 */         \
        mfsrr1  %r4;                             \
        stw     %r4,76(%r1)  /* srr1 */

base = .
        
reserved_0000:
        /* fails */
        b failed
        
        /* Restore registers */
        .globl pok_arch_rfi
pok_arch_rfi:
        lwz     %r13,60(%r1)
        lwz     %r12,56(%r1)
        lwz     %r11,52(%r1)
        lwz     %r10,48(%r1)
        lwz     %r9,44(%r1)
        lwz     %r8,40(%r1)
        lwz     %r7,36(%r1)
        lwz     %r6,32(%r1)
        lwz     %r5,28(%r1)
        lwz     %r4,24(%r1)
        lwz     %r3,20(%r1)
        lwz     %r2,16(%r1)
        lwz     %r0,84(%r1)
        mtlr    %r0
        lwz     %r0,64(%r1)
        mtctr   %r0
        lwz     %r0,68(%r1)
        mtxer   %r0
        lwz     %r0,72(%r1)
        mtsrr0  %r0
        lwz     %r0,76(%r1)
        mtsrr1  %r0
        andi.   %r0,%r0,MSR_PR
        beq     1f
        addi    %r0,%r1,FRAME_SIZE
        mtsprg  1,%r1
1:      lwz     %r0,8(%r1)
        mtcr    %r0
        lwz     %r0,12(%r1)
        lwz     %r1,0(%r1)
        rfi

failed: b failed

        . = base + 0x100
        .globl _pok_reset
system_reset_0100:
_pok_reset:
        /* Clear BAT */

        /* Setup stack */
        lis %r1,(pok_stack_end-4)@h
        ori %r1,%r1,(pok_stack_end-4)@l

        /* Clear back chain */
        xor %r0,%r0,%r0
        stw %r0,4(%r1)

         /* Workaround for QEMU */
         mtmsr %r0
         li  %r3, MSR_IP
         mtmsr %r3
         /* end of workaround */

        /* FIXME: copy .sdata, clear .bss */
        /* Copy .data */
        lis %r3,(__data_start-4)@h
        ori %r3,%r3,(__data_start-4)@l
        lis %r4,(__data_load-4)@h
        ori %r4,%r4,(__data_load-4)@l
        lis %r5,__data_end@h
        ori %r5,%r5,__data_end@l
        cmplw %r3,%r5
        bge 2f
1:      lwzu %r6,4(%r4)
        stwu %r6,4(%r3)
        cmplw %r3,%r5
        blt 1b
2:

        /* Call C function */
        bl pok_boot
1:      b  1b

        . = base + 0x200
machine_check_0200:
        /* fails */
        b reserved_2F00

        . = base + 0x300
dsi_0300:
        mtsprg  3,%r3
        mtsprg  2,%r4
        mfcr    %r3
        mfsrr1  %r4
        andi.   %r4,%r4,MSR_PR
        beq     failed /* DSI musn't appear in supervisor mode.  */

        /* Switch to kernel stack.  */
        mr      %r4,%r1
        mfsprg  %r1,1
        stwu    %r4,-FRAME_SIZE(%r1)

        SAVE_REGS
        mfdar   %r3
        mfdsisr %r4

        bl      pok_arch_dsi_int
        b       pok_arch_rfi

        . = base + 0x400
isi_0400:
        mtsprg  3,%r3
        mtsprg  2,%r4
        mfcr    %r3
        mfsrr1  %r4
        andi.   %r4,%r4,MSR_PR
        beq     failed /* ISI musn't appear in supervisor mode.  */

        /* Switch to kernel stack.  */
        mr      %r4,%r1
        mfsprg  %r1,1
        stwu    %r4,-FRAME_SIZE(%r1)

        SAVE_REGS

        bl      pok_arch_isi_int
        b       pok_arch_rfi

        . = base + 0x500
ext_interrupt_0500:
        b reserved_2F00

        . = base + 0x600
alignment_0600:
        b reserved_2F00

        . = base + 0x700
program_0700:
        b reserved_2F00

        . = base + 0x800
fp_unavailable_0800:
        b reserved_2F00

        . = base + 0x900
decrementer_0900:
        /* PC saved in srr0,
           MSR saved in srr1.  */
        /* Can use sprg.  */
        mtsprg  3,%r3
        mtsprg  2,%r4
        mfcr    %r3
        mfsrr1  %r4
        andi.   %r4,%r4,MSR_PR
        bne     failed /* Need to switch to system stack */

        stwu    %r1,-FRAME_SIZE(%r1)
        
        SAVE_REGS

        bl      pok_arch_decr_int
        b       pok_arch_rfi

        . = base + 0xa00
reserved_0A00:
        b reserved_2F00

        . = base + 0xb00
reserved_0B00:
        b reserved_2F00

        . = base + 0xc00
system_call_0C00:
        mtsprg  3,%r3
        mtsprg  2,%r4
        mfcr    %r3
        mfsrr1  %r4
        andi.   %r4,%r4,MSR_PR
        beq     failed /* SC musn't appear in supervisor mode.  */

        /* Switch to kernel stack.  */
        mr      %r4,%r1
        mfsprg  %r1,1
        stwu    %r4,-FRAME_SIZE(%r1)

        SAVE_REGS
        lwz     %r3,20(%r1)
        lwz     %r4,24(%r1)
        lwz     %r5,28(%r1)
        lwz     %r6,32(%r1)
        lwz     %r7,36(%r1)
        lwz     %r8,40(%r1)

        /* Enable interrupt during syscalls.  */
        mfmsr   %r0
        ori     %r0,%r0,MSR_EE
        mtmsr   %r0

        bl      pok_arch_sc_int
        stw     %r3,20(%r1)
        
        b       pok_arch_rfi

        . = base + 0xd00
trace_0D00:
        b reserved_2F00

        . = base + 0xe00
reserved_0E00:
        b reserved_2F00

        . = base + 0xf00
reserved_0F00:
        b reserved_2F00

         .p2align 8
reserved_1000:
        b reserved_2F00

         .p2align 8
reserved_1100:
        b reserved_2F00

         .p2align 8
reserved_1200:
        b reserved_2F00

         .p2align 8
reserved_1300:
        b reserved_2F00

         .p2align 8
reserved_1400:
        b reserved_2F00

         .p2align 8
reserved_1500:
        b reserved_2F00

         .p2align 8
reserved_1600:
        b reserved_2F00

         .p2align 8
reserved_1700:
        b reserved_2F00

         .p2align 8
reserved_1800:
        b reserved_2F00

         .p2align 8
reserved_1900:
        b reserved_2F00

         .p2align 8
reserved_1A00:
        b reserved_2F00

         .p2align 8
reserved_1B00:
        b reserved_2F00

         .p2align 8
reserved_1C00:
        b reserved_2F00

         .p2align 8
reserved_1D00:
        b reserved_2F00

         .p2align 8
reserved_1E00:
        b reserved_2F00

         .p2align 8
reserved_1F00:
        b reserved_2F00

         .p2align 8
reserved_2000:
        b reserved_2F00

         .p2align 8
reserved_2100:
        b reserved_2F00

         .p2align 8
reserved_2200:
        b reserved_2F00

         .p2align 8
reserved_2300:
        b reserved_2F00

         .p2align 8
reserved_2400:
        b reserved_2F00

         .p2align 8
reserved_2500:
        b reserved_2F00

         .p2align 8
reserved_2600:
        b reserved_2F00

         .p2align 8
reserved_2700:
        b reserved_2F00

         .p2align 8
reserved_2800:
        b reserved_2F00

         .p2align 8
reserved_2900:
        b reserved_2F00

         .p2align 8
reserved_2A00:
        b reserved_2F00

         .p2align 8
reserved_2B00:
        b reserved_2F00

         .p2align 8
reserved_2C00:
        b reserved_2F00

         .p2align 8
reserved_2D00:
        b reserved_2F00

         .p2align 8
reserved_2E00:
        b reserved_2F00

         .p2align 8
reserved_2F00:
        b reserved_2F00

         .p2align 8
reserved_end:

        .section ".bss", "aw"
pok_stack:
        .space 8 * 1024
pok_stack_end:

pok_save_area:
        .space 4 * 8

        .section ".reset", "ax"
reset_FFFC:
        b system_reset_0100
