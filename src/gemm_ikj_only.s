_Z4GemmR6TensorS0_:
.LFB3449:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3449
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movq	%rdx, %r15
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	movq	%rdi, 72(%rsp)
	movq	%rsi, %rdi
.LEHB0:
	call	_ZN6Tensor4dataEv@PLT
	movq	%r15, %rdi
	movq	%rax, %r12
	call	_ZN6Tensor4dataEv@PLT
	movq	%rbp, %rdi
	movq	%rax, %rbx
	call	_ZN6Tensor4growEv@PLT
	movq	%rbp, %rdi
	movl	%eax, %r13d
	call	_ZN6Tensor4gcolEv@PLT
	movq	%r15, %rdi
	movl	%eax, %r14d
	call	_ZN6Tensor4growEv@PLT
	movq	%r15, %rdi
	movl	%eax, %ebp
	call	_ZN6Tensor4gcolEv@PLT
	cmpl	%ebp, %r14d
	jne	.L33
	movq	72(%rsp), %rbp
	movl	%eax, %edx
	movl	%r13d, %esi
	movl	%eax, %r15d
	movq	%rbp, %rdi
	call	_ZN6TensorC1Eii@PLT
.LEHE0:
	movq	%rbp, %rdi
.LEHB1:
	call	_ZN6Tensor4dataEv@PLT
.LEHE1:
	movq	%rax, %rbp
	testl	%r13d, %r13d
	jle	.L1
	testl	%r14d, %r14d
	jle	.L1
	testl	%r15d, %r15d
	jle	.L1
	leal	-1(%r15), %eax
	movl	%r15d, %r9d
	xorl	%r8d, %r8d
	movl	$0, 36(%rsp)
	shrl	$2, %r9d
	movl	%eax, 32(%rsp)
	leaq	0(,%r14,4), %r11
	movl	%r15d, %eax
	leaq	0(,%rax,4), %r10
	movl	%r9d, %edx
	movq	%rax, 16(%rsp)
	leaq	(%r12,%r11), %r14
	sall	$2, %r9d
	movq	%rbp, %r12
	movl	%r15d, %eax
	movq	%r10, 40(%rsp)
	subq	%rbx, %r12
	subl	%r9d, %eax
	salq	$4, %rdx
	movl	%eax, 48(%rsp)
	movq	%r12, 64(%rsp)
	.p2align 4
	.p2align 3
.L4:
	movslq	36(%rsp), %rax
	movq	64(%rsp), %r10
	movq	%r14, %rdi
	xorl	%ecx, %ecx
	movl	%r8d, 52(%rsp)
	subq	%r11, %rdi
	leaq	0(,%rax,4), %rsi
	movq	%rax, 24(%rsp)
	leaq	0(%rbp,%rsi), %rax
	leaq	-4(%rsi,%r10), %rsi
	movq	%r11, 56(%rsp)
	movl	%r13d, %r11d
	movq	%rsi, 8(%rsp)
.L17:
	movss	(%rdi), %xmm2
	movl	%ecx, %r13d
	cmpl	$1, %r15d
	je	.L43
.L6:
	movq	8(%rsp), %rsi
	leaq	0(,%rcx,4), %r8
	subq	%r8, %rsi
	cmpq	$8, %rsi
	jbe	.L5
	cmpl	$2, 32(%rsp)
	jbe	.L20
	movaps	%xmm2, %xmm1
	addq	%rbx, %r8
	xorl	%esi, %esi
	shufps	$0, %xmm1, %xmm1
	.p2align 5
	.p2align 4
	.p2align 3
.L8:
	movups	(%r8,%rsi), %xmm0
	movups	(%rax,%rsi), %xmm3
	mulps	%xmm1, %xmm0
	addps	%xmm3, %xmm0
	movups	%xmm0, (%rax,%rsi)
	addq	$16, %rsi
	cmpq	%rsi, %rdx
	jne	.L8
	movl	%r9d, %esi
	cmpl	%r9d, %r15d
	je	.L11
	movl	48(%rsp), %r12d
	movl	%r9d, %r8d
	movl	%r12d, %r10d
	cmpl	$1, %r12d
	je	.L10
.L7:
	movq	24(%rsp), %r12
	movaps	%xmm2, %xmm1
	shufps	$0xe0, %xmm1, %xmm1
	movq	%xmm1, %xmm1
	addq	%rsi, %r12
	addq	%rcx, %rsi
	movq	(%rbx,%rsi,4), %xmm0
	movl	%r10d, %esi
	andl	$-2, %esi
	andl	$1, %r10d
	mulps	%xmm1, %xmm0
	movq	0(%rbp,%r12,4), %xmm1
	movq	%xmm0, %xmm0
	addps	%xmm1, %xmm0
	movlps	%xmm0, 0(%rbp,%r12,4)
	je	.L11
	addl	%esi, %r8d
.L10:
	movl	36(%rsp), %esi
	addq	$4, %rdi
	addl	%r8d, %esi
	addl	%r13d, %r8d
	movl	%r8d, %r8d
	movl	%esi, %esi
	mulss	(%rbx,%r8,4), %xmm2
	addss	0(%rbp,%rsi,4), %xmm2
	movss	%xmm2, 0(%rbp,%rsi,4)
	cmpq	%r14, %rdi
	je	.L38
.L40:
	addq	16(%rsp), %rcx
	movss	(%rdi), %xmm2
	movl	%ecx, %r13d
	jmp	.L6
	.p2align 4,,10
	.p2align 3
.L11:
	addq	$4, %rdi
	cmpq	%r14, %rdi
	jne	.L40
.L38:
	movl	52(%rsp), %r8d
	movl	%r11d, %r13d
	movq	56(%rsp), %r11
	addl	%r15d, 36(%rsp)
	addl	$1, %r8d
	addq	%r11, %r14
	cmpl	%r8d, %r13d
	jne	.L4
.L1:
	movq	72(%rsp), %rax
	addq	$88, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L43:
	.cfi_restore_state
	leaq	0(,%rcx,4), %r8
.L5:
	movq	40(%rsp), %r10
	addq	%rbx, %r8
	xorl	%esi, %esi
	.p2align 5
	.p2align 4
	.p2align 3
.L14:
	movss	(%r8,%rsi), %xmm0
	mulss	%xmm2, %xmm0
	addss	(%rax,%rsi), %xmm0
	movss	%xmm0, (%rax,%rsi)
	addq	$4, %rsi
	cmpq	%r10, %rsi
	jne	.L14
	addq	$4, %rdi
	addq	16(%rsp), %rcx
	cmpq	%rdi, %r14
	jne	.L17
	jmp	.L38
.L20:
	movl	%r15d, %r10d
	xorl	%esi, %esi
	xorl	%r8d, %r8d
	jmp	.L7
.L21:
	movq	%rax, %rbx
	jmp	.L18
	.section	.gcc_except_table,"a",@progbits
.LLSDA3449:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3449-.LLSDACSB3449
.LLSDACSB3449:
	.uleb128 .LEHB0-.LFB3449
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB3449
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L21-.LFB3449
	.uleb128 0
.LLSDACSE3449:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3449
