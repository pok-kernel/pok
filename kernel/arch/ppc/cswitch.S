        .text
        .globl pok_context_switch
pok_context_switch:
        /* r3: *old_sp, r4: new_sp */

        /* Save registers.  */
        mflr    %r0
        stw     %r0,4(%r1)
        stwu    %r1,-96(%r1)
        mfcr    %r0
        stw     %r0,8(%r1)
        stw     %r2,12(%r1)
        stw     %r13,16(%r1)
        stw     %r14,20(%r1)
        stw     %r15,24(%r1)
        stw     %r16,28(%r1)
        stw     %r17,32(%r1)
        stw     %r18,36(%r1)
        stw     %r19,40(%r1)
        stw     %r20,44(%r1)
        stw     %r21,48(%r1)
        stw     %r22,52(%r1)
        stw     %r23,56(%r1)
        stw     %r24,60(%r1)
        stw     %r25,64(%r1)
        stw     %r26,68(%r1)
        stw     %r27,72(%r1)
        stw     %r28,76(%r1)
        stw     %r29,80(%r1)
        stw     %r30,84(%r1)
        stw     %r31,88(%r1)

        /* Switch stack. */
        stw     %r1,0(%r3)
        mr      %r1,%r4
        
        /* Restore registers.  */
        lwz     %r31,88(%r1)
        lwz     %r30,84(%r1)
        lwz     %r29,80(%r1)
        lwz     %r28,76(%r1)
        lwz     %r27,72(%r1)
        lwz     %r26,68(%r1)
        lwz     %r25,64(%r1)
        lwz     %r24,60(%r1)
        lwz     %r23,56(%r1)
        lwz     %r22,52(%r1)
        lwz     %r21,48(%r1)
        lwz     %r20,44(%r1)
        lwz     %r19,40(%r1)
        lwz     %r18,36(%r1)
        lwz     %r17,32(%r1)
        lwz     %r16,28(%r1)
        lwz     %r15,24(%r1)
        lwz     %r14,20(%r1)
        lwz     %r13,16(%r1)
        lwz     %r2,12(%r1)
        lwz     %r0,8(%r1)
        mtcr    %r0
        lwz     %r1,0(%r1)
        lwz     %r0,4(%r1)
        mtlr    %r0
        blr

        .size pok_context_switch, . - pok_context_switch

        .globl pok_arch_thread_start
pok_arch_thread_start:
        mr      %r3, %r14
        mr      %r4, %r15
        
        xor     %r0,%r0,%r0
        stw     %r0,4(%r1)
        bl      pok_thread_start

        .size pol_arch_thread_start, . - pok_arch_thread_start
