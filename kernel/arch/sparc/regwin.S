#include "sparc_conf.h"

/* from RTEMS window.S
 *  COPYRIGHT:
 *
 *  This file includes the window overflow and underflow handlers from
 *  the file srt0.s provided with the binary distribution of the SPARC
 *  Instruction Simulator (SIS) found at
 *  ftp://ftp.estec.esa.nl/pub/ws/wsd/erc32.
 *
 *  COPYRIGHT (c) 1995. European Space Agency.
 */

.global window_overflow_trap_handler
window_overflow_trap_handler:

        /*
         *  Calculate new WIM by "rotating" the valid bits in the WIM right
         *  by one position.  The following shows how the bits move for a SPARC
         *  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
         *
         *    OLD WIM = 76543210
         *    NEW WIM = 07654321
         *
         *  NOTE: New WIM must be stored in a global register since the
         *        "save" instruction just prior to the load of the wim 
         *        register will result in the local register set changing.
         */
        mov  %wim, %l3                   ! l3 = WIM
        mov  %g1, %l7                    ! save g1
        srl  %l3, 1, %g1                 ! g1 = WIM >> 1
        sll  %l3, WINDOWS_NBR-1 , %l4
                                         ! l4 = WIM << (Number Windows - 1)
        or   %l4, %g1, %g1               ! g1 = (WIM >> 1) | 
                                         !      (WIM << (Number Windows - 1))

        save                             ! Get into window to be saved.
        mov  %g1, %wim                   ! load new WIM
        nop; nop; nop                    ! 3 slot delay
        std  %l0, [%sp + 0x00]           ! save local register set
        std  %l2, [%sp + 0x08]
        std  %l4, [%sp + 0x10]
        std  %l6, [%sp + 0x18]
        std  %i0, [%sp + 0x20]           ! save input register set
        std  %i2, [%sp + 0x28]
        std  %i4, [%sp + 0x30]
        std  %i6, [%sp + 0x38]
        restore                          ! Go back to trap window.
        mov  %l7, %g1                    ! restore g1
        jmp  %l1                         ! Re-execute save.
        rett %l2

        /*
         *  Window underflow trap handler.
         *
         *  On entry:
         *
         *    l0 = psr (from trap table)
         *    l1 = pc
         *    l2 = npc
         */

.global window_underflow_trap_handler
window_underflow_trap_handler:

        /*
         *  Calculate new WIM by "rotating" the valid bits in the WIM left 
         *  by one position.  The following shows how the bits move for a SPARC
         *  cpu implementation where SPARC_NUMBER_OF_REGISTER_WINDOWS is 8.
         *
         *    OLD WIM = 76543210
         *    NEW WIM = 07654321
         *
         *  NOTE: New WIM must be stored in a global register since the
         *        "save" instruction just prior to the load of the wim 
         *        register will result in the local register set changing.
         */
        mov  %wim, %l3                  ! Calculate new WIM
        sll  %l3, 1, %l4                ! l4 = WIM << 1
        srl  %l3, WINDOWS_NBR-1, %l5
                                        ! l5 = WIM >> (Number Windows-1)
        or   %l5, %l4, %l5              ! l5 = (WIM << 1) |
                                        !      (WIM >> (Number Windows-1))
        mov  %l5, %wim                  ! load the new WIM
        nop; nop; nop
        restore                         ! Two restores to get into the
        restore                         ! window to restore
        ldd  [%sp + 0x00], %l0          ! First the local register set
        ldd  [%sp + 0x08], %l2
        ldd  [%sp + 0x10], %l4
        ldd  [%sp + 0x18], %l6
        ldd  [%sp + 0x20], %i0          ! Then the input registers
        ldd  [%sp + 0x28], %i2
        ldd  [%sp + 0x30], %i4
        ldd  [%sp + 0x38], %i6
        save                            ! Get back to the trap window.
        save
        jmp  %l1                        ! Re-execute restore.
        rett  %l2
