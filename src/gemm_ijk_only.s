_Z8Gemm_ijkR6TensorS0_:
.LFB3478:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3478
	subq	$120, %rsp
	.cfi_def_cfa_offset 128
	movq	%r12, 88(%rsp)
	.cfi_offset 12, -40
	movq	%rdx, %r12
	movq	%rdi, 56(%rsp)
	movq	%rsi, %rdi
	movq	%rbx, 72(%rsp)
	.cfi_offset 3, -56
	movq	%rsi, %rbx
	movq	%rbp, 80(%rsp)
	movq	%r14, 104(%rsp)
	movq	%r15, 112(%rsp)
.LEHB3:
	.cfi_offset 6, -48
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	call	_ZN6Tensor4dataEv@PLT
	movq	%r12, %rdi
	movq	%rax, %r15
	call	_ZN6Tensor4dataEv@PLT
	movq	%rbx, %rdi
	movq	%rax, 16(%rsp)
	call	_ZN6Tensor4growEv@PLT
	movq	%rbx, %rdi
	movl	%eax, %r14d
	call	_ZN6Tensor4gcolEv@PLT
	movq	%r12, %rdi
	movl	%eax, %ebp
	call	_ZN6Tensor4growEv@PLT
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	_ZN6Tensor4gcolEv@PLT
	cmpl	%ebx, %ebp
	jne	.L68
	movq	56(%rsp), %rbx
	movl	%eax, %edx
	movl	%r14d, %esi
	movl	%eax, %r12d
	movq	%rbx, %rdi
	call	_ZN6TensorC1Eii@PLT
.LEHE3:
	movq	%rbx, %rdi
.LEHB4:
	call	_ZN6Tensor4dataEv@PLT
.LEHE4:
	testl	%r14d, %r14d
	jle	.L44
	testl	%r12d, %r12d
	jle	.L44
	movl	%ebp, %edx
	leal	-1(%rbp), %ebx
	movq	%rax, 8(%rsp)
	movl	%r12d, %eax
	shrl	$2, %edx
	movl	%ebx, 28(%rsp)
	movq	%rax, %rsi
	leaq	(%rax,%rax,2), %rcx
	movl	%edx, %edi
	leaq	0(,%rax,4), %r10
	leal	0(,%rdx,4), %ebx
	movl	$0, 40(%rsp)
	salq	$4, %rdi
	movl	%ebx, 24(%rsp)
	salq	$4, %rsi
	movq	%rdi, 48(%rsp)
	salq	$2, %rcx
	movq	%r10, 32(%rsp)
	movl	%r14d, 44(%rsp)
	movl	%r12d, 4(%rsp)
	movq	16(%rsp), %r12
	movq	%r13, 96(%rsp)
	.cfi_offset 13, -32
	xorl	%r13d, %r13d
	.p2align 4
	.p2align 3
.L47:
	testl	%ebp, %ebp
	jle	.L61
	movq	48(%rsp), %rdi
	movslq	%r13d, %rdx
	movq	%r12, %r11
	xorl	%r8d, %r8d
	leaq	(%r15,%rdx,4), %rdx
	xorl	%ebx, %ebx
	movq	%rdx, 16(%rsp)
	addq	%rdi, %rdx
	cmpl	$2, 28(%rsp)
	jbe	.L60
.L52:
	movq	16(%rsp), %r9
	movq	%r11, %rdi
	pxor	%xmm1, %xmm1
	.p2align 4
	.p2align 3
.L49:
	movss	(%rdi,%rcx), %xmm0
	movss	(%rdi,%rax,8), %xmm2
	addq	$16, %r9
	movss	(%rdi,%rax,4), %xmm3
	movups	-16(%r9), %xmm4
	unpcklps	%xmm0, %xmm2
	movss	(%rdi), %xmm0
	addq	%rsi, %rdi
	unpcklps	%xmm3, %xmm0
	movlhps	%xmm2, %xmm0
	mulps	%xmm4, %xmm0
	addss	%xmm0, %xmm1
	movaps	%xmm0, %xmm2
	shufps	$85, %xmm0, %xmm2
	addss	%xmm1, %xmm2
	movaps	%xmm0, %xmm1
	unpckhps	%xmm0, %xmm1
	shufps	$255, %xmm0, %xmm0
	addss	%xmm2, %xmm1
	addss	%xmm0, %xmm1
	cmpq	%r9, %rdx
	jne	.L49
	movl	24(%rsp), %edi
	cmpl	%ebp, %edi
	je	.L79
.L48:
	movl	4(%rsp), %r9d
	leal	0(%r13,%rdi), %r10d
	movss	(%r15,%r10,4), %xmm0
	leal	1(%rdi), %r10d
	imull	%edi, %r9d
	leal	(%r9,%rbx), %r14d
	mulss	(%r12,%r14,4), %xmm0
	addss	%xmm1, %xmm0
	cmpl	%r10d, %ebp
	jle	.L78
	addl	4(%rsp), %r9d
	leal	0(%r13,%r10), %r14d
	addl	$2, %edi
	leal	(%r9,%rbx), %r10d
	movss	(%r15,%r14,4), %xmm1
	mulss	(%r12,%r10,4), %xmm1
	addss	%xmm0, %xmm1
	cmpl	%edi, %ebp
	jle	.L55
	addl	4(%rsp), %r9d
	addl	%r13d, %edi
	addl	%ebx, %r9d
	movl	%edi, %edi
	movl	%r9d, %r9d
	movss	(%r12,%r9,4), %xmm0
	mulss	(%r15,%rdi,4), %xmm0
	addss	%xmm1, %xmm0
.L78:
	movq	8(%rsp), %rbx
	movss	%xmm0, (%rbx,%r8,4)
	addq	$1, %r8
	cmpq	%r8, %rax
	je	.L51
.L76:
	addq	$4, %r11
	movl	%r8d, %ebx
	cmpl	$2, 28(%rsp)
	ja	.L52
.L60:
	xorl	%edi, %edi
	pxor	%xmm1, %xmm1
	jmp	.L48
	.p2align 4,,10
	.p2align 3
.L79:
	movq	8(%rsp), %rbx
	movss	%xmm1, (%rbx,%r8,4)
	addq	$1, %r8
	cmpq	%r8, %rax
	je	.L51
	addq	$4, %r11
	movl	%r8d, %ebx
	jmp	.L52
.L61:
	movq	8(%rsp), %r8
	xorl	%edx, %edx
.L56:
	leaq	1(%rdx), %rdi
	movl	$0x00000000, (%r8,%rdx,4)
	cmpq	%rdi, %rax
	je	.L51
	addq	$2, %rdx
	movl	$0x00000000, (%r8,%rdi,4)
	cmpq	%rdx, %rax
	jne	.L56
	.p2align 4
	.p2align 3
.L51:
	addl	$1, 40(%rsp)
	movq	32(%rsp), %rdi
	addl	%ebp, %r13d
	addq	%rdi, 8(%rsp)
	movl	40(%rsp), %ebx
	cmpl	%ebx, 44(%rsp)
	jne	.L47
	movq	96(%rsp), %r13
	.cfi_restore 13
.L44:
	movq	56(%rsp), %rax
	movq	72(%rsp), %rbx
	movq	80(%rsp), %rbp
	movq	88(%rsp), %r12
	movq	104(%rsp), %r14
	movq	112(%rsp), %r15
	addq	$120, %rsp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L55:
	.cfi_def_cfa_offset 128
	.cfi_offset 13, -32
	movq	8(%rsp), %rbx
	movss	%xmm1, (%rbx,%r8,4)
	addq	$1, %r8
	cmpq	%r8, %rax
	jne	.L76
	jmp	.L51
.L62:
	.cfi_restore 13
	movq	%r13, 96(%rsp)
	movq	%rax, %rbx
	.cfi_offset 13, -32
	jmp	.L58
	.section	.gcc_except_table
.LLSDA3478:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3478-.LLSDACSB3478
.LLSDACSB3478:
	.uleb128 .LEHB3-.LFB3478
	.uleb128 .LEHE3-.LEHB3
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB4-.LFB3478
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L62-.LFB3478
	.uleb128 0
.LLSDACSE3478:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3478
	.type	_Z8Gemm_ijkR6TensorS0_.cold, @function
