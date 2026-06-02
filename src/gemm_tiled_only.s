_Z10Gemm_tiledR6TensorS0_i:
.LFB3479:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA3479
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%rsi, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rdx, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$216, %rsp
	.cfi_def_cfa_offset 272
	movq	%rdi, 120(%rsp)
	movq	%rsi, %rdi
	movl	%ecx, 128(%rsp)
.LEHB6:
	call	_ZN6Tensor4dataEv@PLT
	movq	%r12, %rdi
	movq	%rax, 152(%rsp)
	call	_ZN6Tensor4dataEv@PLT
	movq	%r13, %rdi
	movq	%rax, %r15
	call	_ZN6Tensor4growEv@PLT
	movq	%r12, %rdi
	movl	%eax, %ebx
	call	_ZN6Tensor4growEv@PLT
	movq	%r13, %rdi
	movl	%eax, %ebp
	call	_ZN6Tensor4gcolEv@PLT
	movq	%r12, %rdi
	movl	%eax, 132(%rsp)
	movl	%eax, %r14d
	call	_ZN6Tensor4gcolEv@PLT
	movl	%eax, 88(%rsp)
	cmpl	%r14d, %ebp
	jne	.L113
	movq	120(%rsp), %r14
	movl	%eax, %edx
	movl	%ebx, %esi
	movl	%eax, %ebp
	movq	%r14, %rdi
	call	_ZN6TensorC1Eii@PLT
.LEHE6:
	movq	%r14, %rdi
.LEHB7:
	call	_ZN6Tensor4dataEv@PLT
.LEHE7:
	movq	%rax, %r14
	testl	%ebx, %ebx
	jle	.L80
	testl	%ebp, %ebp
	jle	.L80
	movl	132(%rsp), %eax
	testl	%eax, %eax
	jle	.L80
	movl	%eax, %esi
	salq	$2, %rax
	movl	128(%rsp), %r10d
	movl	%ebp, %ecx
	movq	%rax, 80(%rsp)
	movl	%ebp, %eax
	xorl	%r8d, %r8d
	movq	%rax, 8(%rsp)
	negq	%rax
	imull	%r10d, %ecx
	salq	$2, %rax
	imull	%r10d, %esi
	movl	$0, 160(%rsp)
	movq	%rax, 16(%rsp)
	movslq	%r10d, %rax
	movq	%rax, 136(%rsp)
	movq	%r14, %rax
	movslq	%ecx, %rdx
	subq	%r15, %rax
	movq	%rdx, 144(%rsp)
	xorl	%edx, %edx
	subq	$4, %rax
	movq	%rax, 104(%rsp)
.L83:
	movl	128(%rsp), %eax
	movl	%ebx, %edi
	addl	%r8d, %eax
	cmpl	%ebx, %eax
	cmovle	%eax, %edi
	movl	%edi, 92(%rsp)
	cmpl	%r8d, %edi
	jle	.L94
	movl	%ecx, 164(%rsp)
	movl	%eax, %edi
	movq	%r15, %r12
	movq	%r14, %rbp
	movq	$0, 48(%rsp)
	movl	%edx, 172(%rsp)
	movl	%ebx, 176(%rsp)
	movl	%esi, 168(%rsp)
	movslq	%edx, %rsi
.L95:
	movq	48(%rsp), %rcx
	movl	128(%rsp), %eax
	movl	88(%rsp), %ebx
	addl	%ecx, %eax
	movl	%ecx, %r11d
	movl	%ecx, 64(%rsp)
	movq	%rcx, %r10
	cmpl	%ebx, %eax
	movl	%ecx, 40(%rsp)
	movl	%r11d, %edx
	cmovg	%ebx, %eax
	movl	%eax, %r14d
	movl	%eax, %r15d
	subl	%ecx, %r14d
	xorl	%ecx, %ecx
	leal	-1(%r14), %eax
	movl	%eax, 32(%rsp)
	movl	%r14d, %eax
	shrl	$2, %eax
	movl	%eax, %ebx
	leal	0(,%rax,4), %r13d
	movl	%r14d, %eax
	subl	%r13d, %eax
	salq	$4, %rbx
	movl	%eax, 44(%rsp)
.L93:
	movl	128(%rsp), %eax
	movl	132(%rsp), %r11d
	addl	%ecx, %eax
	cmpl	%r11d, %eax
	cmovg	%r11d, %eax
	cmpl	%ecx, %eax
	jle	.L84
	cmpl	%edx, %r15d
	jle	.L84
	movl	%eax, %r9d
	leaq	(%rsi,%rcx), %rax
	movq	%r10, 72(%rsp)
	movq	152(%rsp), %r11
	subl	%ecx, %r9d
	movl	%r8d, 180(%rsp)
	addq	%r9, %rax
	movq	%rcx, 184(%rsp)
	leaq	(%r11,%rax,4), %rax
	movl	%edx, 192(%rsp)
	movq	%rax, (%rsp)
	movq	%r9, %rax
	movl	%r8d, %r9d
	negq	%rax
	movl	%edi, 196(%rsp)
	leaq	0(,%rax,4), %r11
	movl	160(%rsp), %eax
	movq	%rsi, 200(%rsp)
	movq	%r11, 112(%rsp)
	movl	%eax, 36(%rsp)
	movq	48(%rsp), %rax
	subq	%r10, %rax
	movq	%rax, 96(%rsp)
	.p2align 4
	.p2align 3
.L85:
	movq	(%rsp), %rax
	movq	112(%rsp), %rcx
	movl	%r9d, 68(%rsp)
	movslq	36(%rsp), %rdx
	movq	104(%rsp), %rdi
	addq	%rax, %rcx
	movq	96(%rsp), %rax
	leaq	0(%rbp,%rdx,4), %rsi
	addq	%rdx, %rax
	leaq	(%rdi,%rax,4), %r8
	movq	48(%rsp), %rax
	movq	72(%rsp), %rdi
	addq	%rdx, %rax
	leaq	-4(%rsi), %rdx
	movq	%rdx, 56(%rsp)
	movq	%rax, 24(%rsp)
	leaq	0(%rbp,%rax,4), %rax
	.p2align 4
	.p2align 3
.L92:
	movss	(%rcx), %xmm2
	movaps	%xmm2, %xmm3
	shufps	$0, %xmm3, %xmm3
	cmpl	$1, %r14d
	je	.L86
	cmpq	$8, %r8
	jbe	.L86
	cmpl	$2, 32(%rsp)
	jbe	.L98
	leaq	(%r12,%rdi,4), %r9
	movaps	%xmm3, %xmm1
	xorl	%edx, %edx
	.p2align 5
	.p2align 4
	.p2align 3
.L88:
	movups	(%r9,%rdx), %xmm0
	movups	(%rax,%rdx), %xmm4
	mulps	%xmm1, %xmm0
	addps	%xmm4, %xmm0
	movups	%xmm0, (%rax,%rdx)
	addq	$16, %rdx
	cmpq	%rdx, %rbx
	jne	.L88
	movl	%r13d, %edx
	cmpl	%r13d, %r14d
	je	.L89
	movl	40(%rsp), %r10d
	movl	44(%rsp), %r11d
	leal	0(%r13,%r10), %r9d
	movl	%r11d, %r10d
	cmpl	$1, %r11d
	je	.L90
.L87:
	movq	24(%rsp), %r11
	movq	%xmm3, %xmm3
	addq	%rdx, %r11
	addq	%rdi, %rdx
	movq	(%r12,%rdx,4), %xmm0
	movq	0(%rbp,%r11,4), %xmm1
	movl	%r10d, %edx
	andl	$-2, %edx
	andl	$1, %r10d
	mulps	%xmm3, %xmm0
	movq	%xmm0, %xmm0
	addps	%xmm1, %xmm0
	movlps	%xmm0, 0(%rbp,%r11,4)
	je	.L89
	addl	%edx, %r9d
.L90:
	movl	36(%rsp), %edx
	addl	%r9d, %edx
	movslq	%edx, %rdx
	leaq	0(%rbp,%rdx,4), %r10
	movl	%edi, %edx
	subl	64(%rsp), %edx
	addl	%r9d, %edx
	movslq	%edx, %rdx
	mulss	(%r12,%rdx,4), %xmm2
	addss	(%r10), %xmm2
	movss	%xmm2, (%r10)
.L89:
	addq	$4, %rcx
	addq	8(%rsp), %rdi
	addq	16(%rsp), %r8
	cmpq	%rcx, (%rsp)
	jne	.L92
	movl	68(%rsp), %r9d
	movl	88(%rsp), %edi
	addl	%edi, 36(%rsp)
	movq	80(%rsp), %rdi
	addl	$1, %r9d
	addq	%rdi, (%rsp)
	cmpl	92(%rsp), %r9d
	jne	.L85
	movl	180(%rsp), %r8d
	movq	72(%rsp), %r10
	movq	184(%rsp), %rcx
	movl	192(%rsp), %edx
	movl	196(%rsp), %edi
	movq	200(%rsp), %rsi
.L84:
	movq	136(%rsp), %rax
	addq	144(%rsp), %r10
	addq	%rax, %rcx
	cmpl	%ecx, 132(%rsp)
	jg	.L93
	addq	%rax, 48(%rsp)
	movq	48(%rsp), %rax
	cmpl	%eax, 88(%rsp)
	jg	.L95
	movl	164(%rsp), %ecx
	movl	%edi, %eax
	movq	%r12, %r15
	movq	%rbp, %r14
	movl	168(%rsp), %esi
	movl	172(%rsp), %edx
	movl	176(%rsp), %ebx
.L94:
	addl	%ecx, 160(%rsp)
	movl	%eax, %r8d
	addl	%esi, %edx
	cmpl	%eax, %ebx
	jg	.L83
.L80:
	movq	120(%rsp), %rax
	addq	$216, %rsp
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
.L86:
	.cfi_restore_state
	movq	56(%rsp), %r9
	movq	48(%rsp), %rdx
	subq	%r8, %r9
	.p2align 5
	.p2align 4
	.p2align 3
.L91:
	movss	(%r9,%rdx,4), %xmm0
	mulss	%xmm2, %xmm0
	addss	(%rsi,%rdx,4), %xmm0
	movss	%xmm0, (%rsi,%rdx,4)
	addq	$1, %rdx
	cmpl	%edx, %r15d
	jg	.L91
	jmp	.L89
.L98:
	movl	40(%rsp), %r9d
	movl	%r14d, %r10d
	xorl	%edx, %edx
	jmp	.L87
.L99:
	movq	%rax, %rbx
	jmp	.L96
	.section	.gcc_except_table
.LLSDA3479:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE3479-.LLSDACSB3479
.LLSDACSB3479:
	.uleb128 .LEHB6-.LFB3479
	.uleb128 .LEHE6-.LEHB6
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB7-.LFB3479
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L99-.LFB3479
	.uleb128 0
.LLSDACSE3479:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC3479
	.type	_Z10Gemm_tiledR6TensorS0_i.cold, @function
