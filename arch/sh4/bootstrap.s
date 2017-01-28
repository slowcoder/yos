	.globl	start,_arch_tns,arch_int_disable_interrupts,arch_int_enable_interrupts

	.text

start:
	mov.l	_initial_sr,r0	! Priv-mode, Bank0, Exceptions blocked, IMASK=F
	ldc	r0,sr
	
	! Okay, set up a usermode-stack
	mov.l	_userm_stack_addr,r0
	ldc	r0,dbr
	
        ! First thing, make sure we're using Bank0 r0-r7
        !mov.l   _sr_bankmask,r1
        !stc     sr,r0
        !and     r1,r0
        !ldc     r0,sr

	! Okay, now, set up a kernel-stack
	mov.l	_kernel_stack_addr,r15

cached_addr:
	
	mov.l	kernel_entry_addr,r1
	jmp	@r1
	nop

.align 4
/* int test_and_set(int *val, int set_to, int test_val) */
_arch_tns:
        mov.l   r8,@-r15
        sts.l   pr,@-r15

        /* disable interrupts */
        mov.l   disable_interrupts_addr,r1
        jsr     @r1
        nop

        /* load the value, save it, and store the new value */
        mov.l   @r4,r8         /* load the dest, it will be the return value */
        cmp/eq  r8,r6          /* compare against the test_val */
        bf      _not_equal

        mov.l   r5,@r4         /* put the set_to value into the target */

_not_equal:
        /* restore interrupts */
        mov.l   restore_interrupts_addr,r1
        jsr     @r1
        mov     r0,r4

        /* return value will be old value */
        mov     r8,r0

        /* restore the stack */
        lds.l   @r15+,pr
        rts
        mov.l   @r15+,r8

.align 2
disable_interrupts_addr:        .long   arch_int_disable_interrupts
restore_interrupts_addr:        .long   arch_int_restore_interrupts

arch_int_restore_interrupts:	
        mov.l   inverse_imask_bit_mask,r0
        stc     sr,r1                   /* get the sr register */
        and     r0,r1                   /* zero out the imask part */
        or      r4,r1                   /* or in the passed in imask, should only contain imask bits */
        ldc     r1,sr                   /* put the new status into the sr register */
        rts
        nop

arch_int_enable_interrupts:
        mov.l   inverse_imask_bit_mask,r0
        stc     sr,r1                   /* load the sr register */
        and     r0,r1                   /* set the imask to 0 */
        ldc     r1,sr                   /* put the new status into the sr register */
        rts
        nop

.align 2
inverse_imask_bit_mask: .long   0xffffff0f

arch_int_disable_interrupts:	
        mov.l   imask_bit_mask,r2
        stc     sr,r1                   /* load the sr register */
        mov     r1,r0                   /* save the old sr register */
        or      r2,r1                   /* or in 0xf for the imask */
        ldc     r1,sr                   /* set the new sr register with the interrupts masked */
        rts
        and     r2,r0                   /* make sure the return value contains only the imask part */

.align 2
imask_bit_mask: .long   0x000000f0

		
.align 4
_initial_sr:
	.long	0x500000F0	! Priv-mode, Bank0, Exceptions blocked, IMASK=F
kernel_entry_addr:
	.long	_kernel_entry
_sr_bankmask:
        .long   0xDFFFFFFF
_userm_stack_addr:
	.long	_userm_stack_end - 4
_kernel_stack_addr:
	.long	_kernel_stack_end - 4
_kernel_stack:
	.rep	0x1000
	.byte	0
	.endr
_kernel_stack_end:
_userm_stack:
	.rep	0x1000
	.byte	0
	.endr
_userm_stack_end:
