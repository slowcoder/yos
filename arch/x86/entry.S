/* entry.S - Handover from bootloader to kernel */
/* Copyright (C) 2007 James Jacobsson */

#define STACK_SIZE 0x4000
		
	.text

	.globl	start, _start
start:
_start:
	/* Initialize the stack pointer.  */
	movl	$(stack + STACK_SIZE), %esp

	/* Reset EFLAGS.  */
	pushl	$0
	popf

	mov	$69,%al
	mov	$0xe9,%dx
	out	%al,%dx
	
//	mov	%ebx,(mbi_addr)
//	mov	%eax,(mbi_magic)
	
	/* Now enter the C main function...  */
	/*	call	EXT_C(kernel_entry) */
	call	kernel_entry

loop:
	hlt
	jmp	loop

	
	/* Our stack area.  */
.comm	stack, STACK_SIZE
