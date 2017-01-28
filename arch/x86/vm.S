	.file	"vm.c"
	.section	.rodata
.LC0:
	.string	"(phy & (VM_PAGESIZE-1)) == 0"
.LC1:
	.string	"arch/x86/vm.c"
.LC2:
	.string	"ASSERT FAILED (%s:%d): %s\n"
.LC3:
	.string	"(virt & (VM_PAGESIZE-1)) == 0"
.LC4:
	.string	"PageDir for task: 0x%x\n"
.LC5:
	.string	"EndPDir for task: 0x%x\n"
.LC6:
	.string	"PT = 0x%x\n"
.LC7:
	.string	"PD %u: 0x%x\n"
	.text
.globl Xarch_vm_map_page
	.type	Xarch_vm_map_page, @function
Xarch_vm_map_page:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	12(%ebp), %eax
	andl	$4095, %eax
	testl	%eax, %eax
	je	.L2
	pushl	$.LC0
	pushl	$11
	pushl	$.LC1
	pushl	$.LC2
	call	kprintf
	addl	$16, %esp
.L4:
	jmp	.L4
.L2:
	movl	16(%ebp), %eax
	andl	$4095, %eax
	testl	%eax, %eax
	je	.L5
	pushl	$.LC3
	pushl	$12
	pushl	$.LC1
	pushl	$.LC2
	call	kprintf
	addl	$16, %esp
.L7:
	jmp	.L7
.L5:
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC4
	call	kprintf
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %eax
	addl	$4092, %eax
	subl	$8, %esp
	pushl	%eax
	pushl	$.LC5
	call	kprintf
	addl	$16, %esp
	movl	16(%ebp), %eax
	shrl	$22, %eax
	movl	%eax, -4(%ebp)
	subl	$12, %esp
	pushl	$1
	call	pmm_alloc_pages
	addl	$16, %esp
	movl	%eax, -12(%ebp)
	subl	$8, %esp
	pushl	-12(%ebp)
	pushl	$.LC6
	call	kprintf
	addl	$16, %esp
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-4(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	movl	-12(%ebp), %eax
	movl	%eax, (%edx)
	movl	$0, -12(%ebp)
	jmp	.L8
.L9:
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	sall	$2, %eax
	leal	(%edx,%eax), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	je	.L10
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-12(%ebp), %eax
	sall	$2, %eax
	leal	(%edx,%eax), %eax
	movl	(%eax), %eax
	subl	$4, %esp
	pushl	%eax
	pushl	-12(%ebp)
	pushl	$.LC7
	call	kprintf
	addl	$16, %esp
.L10:
	incl	-12(%ebp)
.L8:
	cmpl	$1023, -12(%ebp)
	jbe	.L9
	leave
	ret
	.size	Xarch_vm_map_page, .-Xarch_vm_map_page
.globl arch_vm_map_page
	.type	arch_vm_map_page, @function
arch_vm_map_page:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	movl	12(%ebp), %eax
	andl	$4095, %eax
	testl	%eax, %eax
	je	.L15
	pushl	$.LC0
	pushl	$30
	pushl	$.LC1
	pushl	$.LC2
	call	kprintf
	addl	$16, %esp
.L17:
	jmp	.L17
.L15:
	movl	16(%ebp), %eax
	andl	$4095, %eax
	testl	%eax, %eax
	je	.L18
	pushl	$.LC3
	pushl	$31
	pushl	$.LC1
	pushl	$.LC2
	call	kprintf
	addl	$16, %esp
.L20:
	jmp	.L20
.L18:
	movl	16(%ebp), %eax
	shrl	$22, %eax
	movl	%eax, -16(%ebp)
	movl	16(%ebp), %eax
	andl	$4194303, %eax
	shrl	$12, %eax
	movl	%eax, -12(%ebp)
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	sall	$2, %eax
	leal	(%edx,%eax), %eax
	movl	(%eax), %eax
	testl	%eax, %eax
	jne	.L21
	subl	$12, %esp
	pushl	$1
	call	pmm_alloc_pages
	addl	$16, %esp
	movl	%eax, -4(%ebp)
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	sall	$2, %eax
	addl	%eax, %edx
	movl	-4(%ebp), %eax
	movl	%eax, (%edx)
	movl	$0, -8(%ebp)
	jmp	.L23
.L24:
	movl	-8(%ebp), %eax
	sall	$2, %eax
	addl	-4(%ebp), %eax
	movl	$0, (%eax)
	incl	-8(%ebp)
.L23:
	cmpl	$1023, -8(%ebp)
	jbe	.L24
.L21:
	movl	8(%ebp), %eax
	movl	20(%eax), %eax
	movl	(%eax), %edx
	movl	-16(%ebp), %eax
	sall	$2, %eax
	leal	(%edx,%eax), %eax
	movl	(%eax), %eax
	movl	%eax, -4(%ebp)
	movl	-12(%ebp), %eax
	sall	$2, %eax
	movl	%eax, %edx
	addl	-4(%ebp), %edx
	movl	16(%ebp), %eax
	orl	$3, %eax
	movl	%eax, (%edx)
	leave
	ret
	.size	arch_vm_map_page, .-arch_vm_map_page
	.ident	"GCC: (GNU) 4.1.0"
