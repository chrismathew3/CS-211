.globl	nCr
	.type	nCr, @function
nCr:
.LFB0:
	
	pushq	%rbp
	
	movq	%rsp, %rbp
	
	pushq	%r12
	pushq	%rbx
	subq	$32, %rsp
	
	movl	%edi, -36(%rbp)
	movl	%esi, -40(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, %edi
	call	Factorial
	movq	%rax, %rbx
	movl	-40(%rbp), %eax
	movl	%eax, %edi
	call	Factorial
	movq	%rax, %r12
	movl	-40(%rbp), %eax
	movl	-36(%rbp), %edx
	subl	%eax, %edx
	movl	%edx, %eax
	movl	%eax, %edi
	call	Factorial
	movq	%r12, %rdx
	imulq	%rax, %rdx
	movq	%rdx, %rcx
	movq	%rbx, %rax
	cqto
	idivq	%rcx
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	addq	$32, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	
	ret
.LFE0:
	.size	nCr, .-nCr
.globl	Factorial
	.type	Factorial, @function
Factorial:
.LFB1:
	
	pushq	%rbp
	
	movq	%rsp, %rbp
	
	movl	%edi, -20(%rbp)
	movq	$1, -16(%rbp)
	movl	$1, -4(%rbp)
	jmp	.L4
.L5:
	movl	-4(%rbp), %eax
	cltq
	movq	-16(%rbp), %rdx
	imulq	%rdx, %rax
	movq	%rax, -16(%rbp)
	addl	$1, -4(%rbp)
.L4:
	movl	-4(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jle	.L5
	movq	-16(%rbp), %rax
	popq	%rbp

	ret

.LFE1:
	.size	Factorial, .-Factorial
	