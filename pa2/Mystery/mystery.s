	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 12
	.globl	_add
	.p2align	4, 0x90
_add:                                   ## @add
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	12(%ebp), %eax
	movl	8(%ebp), %ecx
	movl	%ecx, -4(%ebp)
	movl	%eax, -8(%ebp)
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	addl	$8, %esp
	popl	%ebp
	retl

	.globl	_dothething
	.p2align	4, 0x90
_dothething:                            ## @dothething
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$36, %esp
	calll	L1$pb
L1$pb:
	popl	%eax
	movl	8(%ebp), %ecx
	movl	L_num$non_lazy_ptr-L1$pb(%eax), %edx
	movl	%ecx, -8(%ebp)
	movl	-8(%ebp), %ecx
	cmpl	$-1, (%edx,%ecx,4)
	movl	%eax, -20(%ebp)         ## 4-byte Spill
	je	LBB1_2
## BB#1:
	movl	-20(%ebp), %eax         ## 4-byte Reload
	movl	L_num$non_lazy_ptr-L1$pb(%eax), %ecx
	movl	-8(%ebp), %edx
	movl	(%ecx,%edx,4), %ecx
	movl	%ecx, -16(%ebp)
	jmp	LBB1_11
LBB1_2:
	movl	$-1, -12(%ebp)
	cmpl	$0, -8(%ebp)
	jne	LBB1_4
## BB#3:
	movl	$0, -12(%ebp)
	jmp	LBB1_8
LBB1_4:
	cmpl	$1, -8(%ebp)
	jne	LBB1_6
## BB#5:
	movl	$1, -12(%ebp)
	jmp	LBB1_7
LBB1_6:
	movl	-8(%ebp), %eax
	subl	$1, %eax
	movl	%eax, (%esp)
	calll	_dothething
	movl	-8(%ebp), %ecx
	subl	$2, %ecx
	movl	%ecx, (%esp)
	movl	%eax, -24(%ebp)         ## 4-byte Spill
	calll	_dothething
	movl	-24(%ebp), %ecx         ## 4-byte Reload
	movl	%ecx, (%esp)
	movl	%eax, 4(%esp)
	calll	_add
	movl	%eax, -12(%ebp)
LBB1_7:
	jmp	LBB1_8
LBB1_8:
	movl	-20(%ebp), %eax         ## 4-byte Reload
	movl	L_num$non_lazy_ptr-L1$pb(%eax), %ecx
	movl	-8(%ebp), %edx
	cmpl	$-1, (%ecx,%edx,4)
	jne	LBB1_10
## BB#9:
	movl	-20(%ebp), %eax         ## 4-byte Reload
	movl	L_num$non_lazy_ptr-L1$pb(%eax), %ecx
	movl	-12(%ebp), %edx
	movl	-8(%ebp), %esi
	movl	%edx, (%ecx,%esi,4)
LBB1_10:
	movl	-20(%ebp), %eax         ## 4-byte Reload
	movl	L_num$non_lazy_ptr-L1$pb(%eax), %ecx
	movl	-8(%ebp), %edx
	movl	(%ecx,%edx,4), %ecx
	movl	%ecx, -16(%ebp)
LBB1_11:
	movl	-16(%ebp), %eax
	addl	$36, %esp
	popl	%esi
	popl	%ebp
	retl

	.globl	_main
	.p2align	4, 0x90
_main:                                  ## @main
## BB#0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$40, %esp
	calll	L2$pb
L2$pb:
	popl	%eax
	movl	12(%ebp), %ecx
	movl	8(%ebp), %edx
	movl	$0, -4(%ebp)
	movl	%edx, -8(%ebp)
	movl	%ecx, -12(%ebp)
	movl	-12(%ebp), %ecx
	movl	4(%ecx), %ecx
	movl	%ecx, (%esp)
	movl	%eax, -24(%ebp)         ## 4-byte Spill
	calll	_atoi
	movl	%eax, -16(%ebp)
	movl	$0, -20(%ebp)
LBB2_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$200, -20(%ebp)
	jge	LBB2_4
## BB#2:                                ##   in Loop: Header=BB2_1 Depth=1
	movl	-24(%ebp), %eax         ## 4-byte Reload
	movl	L_num$non_lazy_ptr-L2$pb(%eax), %ecx
	movl	-20(%ebp), %edx
	movl	$-1, (%ecx,%edx,4)
## BB#3:                                ##   in Loop: Header=BB2_1 Depth=1
	movl	-20(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -20(%ebp)
	jmp	LBB2_1
LBB2_4:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	calll	_dothething
	movl	-24(%ebp), %ecx         ## 4-byte Reload
	leal	L_.str-L2$pb(%ecx), %edx
	movl	%edx, (%esp)
	movl	%eax, 4(%esp)
	calll	_printf
	xorl	%ecx, %ecx
	movl	%eax, -28(%ebp)         ## 4-byte Spill
	movl	%ecx, %eax
	addl	$40, %esp
	popl	%ebp
	retl

	.comm	_num,800,2              ## @num
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"Value: %ld\n"


	.section	__IMPORT,__pointers,non_lazy_symbol_pointers
L_num$non_lazy_ptr:
	.indirect_symbol	_num
	.long	0

.subsections_via_symbols
