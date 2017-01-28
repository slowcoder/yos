.data
        
.text

#define TRAP_ERRC(name, vector) \
.globl name; \
.align 8; \
name: \
	pushl   $vector; \
	jmp	int_bottom


#define TRAP(name, vector) \
.globl name; \
.align 8; \
name: \
	pushl	$0; \
	pushl   $vector; \
	jmp     int_bottom

TRAP(trap0, 0)
TRAP(trap1, 1)
TRAP(trap2, 2)
TRAP(trap3, 3)
TRAP(trap4, 4)
TRAP(trap5, 5)
TRAP(trap6, 6)
TRAP(trap7, 7)
TRAP_ERRC(trap8, 8)
TRAP(trap9, 9)
TRAP_ERRC(trapA, 0xA)
TRAP_ERRC(trapB,0xB)
TRAP_ERRC(trapC,0xC)
TRAP_ERRC(trapD,0xD)
TRAP_ERRC(trapE,0xE)
TRAP(trapF,0xF)
TRAP(trap10, 0x10)
TRAP_ERRC(trap11, 0x11)
TRAP(trap12, 0x12)
TRAP(trap13, 0x13)

TRAP(trap60, 0x60)
TRAP(trap61, 0x61)
TRAP(trap62, 0x62)
TRAP(trap63, 0x63)
TRAP(trap64, 0x64)
TRAP(trap65, 0x65)
TRAP(trap66, 0x66)
TRAP(trap67, 0x67)
TRAP(trap68, 0x68)
TRAP(trap69, 0x69)
TRAP(trap6A, 0x6A)
TRAP(trap6B, 0x6B)
TRAP(trap6C, 0x6C)
TRAP(trap6D, 0x6D)
TRAP(trap6E, 0x6E)
TRAP(trap6F, 0x6F)

TRAP(trapFE, 0xFE)
	
int_bottom:
	//mov	%dr3,%esp
        pushal

	pushl	%ss
        pushl   %ds
        pushl   %es
        pushl   %fs
        pushl   %gs

	// Okay, make sure we're running in DPL0 (CS is already set by the interrupt-gate)
	mov	$0x10,%eax // KERNEL_DS
	mov	%eax,%ds
	mov	%eax,%es

	// Okay, the arch_handle_trap takes one argument, a pointer to the iframe
	// Since the i386 ABI says that the pointers are pushed to stack, we don't
	// really need to do anything, since our stack is our stack.. :	) 

	call	interrupt_handle_trap
	
	// Okay, our new stack-pointer is returned in %eax (if it's actually different)
	mov	%eax,%esp

        popl    %gs
        popl    %fs             
        popl    %es
        popl    %ds
	popl	%ss
        popal

        addl    $8,%esp	// This skips the vector and the error-code

        iret
