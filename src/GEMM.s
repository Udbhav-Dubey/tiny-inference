	.file	"GEMM.cpp"
	.text
#APP
	.globl _ZSt21ios_base_library_initv
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Tensor Gemm(Tensor&, Tensor&)"
.LC1:
	.string	"GEMM.cpp"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"a_col==b_row&&\"need to get a.col==b.row equal for matrix multiply\""
#NO_APP
	.section	.text.unlikely,"ax",@progbits
.LCOLDB3:
	.text
.LHOTB3:
	.p2align 4
	.globl	_Z4GemmR6TensorS0_
	.type	_Z4GemmR6TensorS0_, @function
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
	.type	_Z4GemmR6TensorS0_.cold, @function
_Z4GemmR6TensorS0_.cold:
.LFSB3449:
.L33:
	.cfi_def_cfa_offset 144
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	leaq	.LC0(%rip), %rcx
	movl	$13, %edx
	leaq	.LC1(%rip), %rsi
	leaq	.LC2(%rip), %rdi
	call	__assert_fail@PLT
.L18:
	movq	72(%rsp), %rax
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.L19
	movq	16(%rax), %rsi
	subq	%rdi, %rsi
	call	_ZdlPvm@PLT
.L19:
	movq	%rbx, %rdi
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
	.cfi_endproc
.LFE3449:
	.section	.gcc_except_table
.LLSDAC3449:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3449-.LLSDACSBC3449
.LLSDACSBC3449:
	.uleb128 .LEHB2-.LCOLDB3
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC3449:
	.section	.text.unlikely
	.text
	.size	_Z4GemmR6TensorS0_, .-_Z4GemmR6TensorS0_
	.section	.text.unlikely
	.size	_Z4GemmR6TensorS0_.cold, .-_Z4GemmR6TensorS0_.cold
.LCOLDE3:
	.text
.LHOTE3:
	.section	.rodata.str1.8
	.align 8
.LC5:
	.string	"Tensor Gemm_ijk(Tensor&, Tensor&)"
	.section	.text.unlikely
.LCOLDB6:
	.text
.LHOTB6:
	.p2align 4
	.globl	_Z8Gemm_ijkR6TensorS0_
	.type	_Z8Gemm_ijkR6TensorS0_, @function
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
_Z8Gemm_ijkR6TensorS0_.cold:
.LFSB3478:
.L68:
	.cfi_def_cfa_offset 128
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	leaq	.LC5(%rip), %rcx
	movl	$35, %edx
	leaq	.LC1(%rip), %rsi
	movq	%r13, 96(%rsp)
	.cfi_offset 13, -32
	leaq	.LC2(%rip), %rdi
	call	__assert_fail@PLT
.L58:
	movq	56(%rsp), %rax
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.L59
	movq	16(%rax), %rsi
	subq	%rdi, %rsi
	call	_ZdlPvm@PLT
.L59:
	movq	%rbx, %rdi
.LEHB5:
	call	_Unwind_Resume@PLT
.LEHE5:
	.cfi_endproc
.LFE3478:
	.section	.gcc_except_table
.LLSDAC3478:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3478-.LLSDACSBC3478
.LLSDACSBC3478:
	.uleb128 .LEHB5-.LCOLDB6
	.uleb128 .LEHE5-.LEHB5
	.uleb128 0
	.uleb128 0
.LLSDACSEC3478:
	.section	.text.unlikely
	.text
	.size	_Z8Gemm_ijkR6TensorS0_, .-_Z8Gemm_ijkR6TensorS0_
	.section	.text.unlikely
	.size	_Z8Gemm_ijkR6TensorS0_.cold, .-_Z8Gemm_ijkR6TensorS0_.cold
.LCOLDE6:
	.text
.LHOTE6:
	.section	.rodata.str1.8
	.align 8
.LC7:
	.string	"Tensor Gemm_tiled(Tensor&, Tensor&, int)"
	.section	.text.unlikely
.LCOLDB8:
	.text
.LHOTB8:
	.p2align 4
	.globl	_Z10Gemm_tiledR6TensorS0_i
	.type	_Z10Gemm_tiledR6TensorS0_i, @function
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
_Z10Gemm_tiledR6TensorS0_i.cold:
.LFSB3479:
.L113:
	.cfi_def_cfa_offset 272
	.cfi_offset 3, -56
	.cfi_offset 6, -48
	.cfi_offset 12, -40
	.cfi_offset 13, -32
	.cfi_offset 14, -24
	.cfi_offset 15, -16
	leaq	.LC7(%rip), %rcx
	movl	$56, %edx
	leaq	.LC1(%rip), %rsi
	leaq	.LC2(%rip), %rdi
	call	__assert_fail@PLT
.L96:
	movq	120(%rsp), %rax
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.L97
	movq	16(%rax), %rsi
	subq	%rdi, %rsi
	call	_ZdlPvm@PLT
.L97:
	movq	%rbx, %rdi
.LEHB8:
	call	_Unwind_Resume@PLT
.LEHE8:
	.cfi_endproc
.LFE3479:
	.section	.gcc_except_table
.LLSDAC3479:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC3479-.LLSDACSBC3479
.LLSDACSBC3479:
	.uleb128 .LEHB8-.LCOLDB8
	.uleb128 .LEHE8-.LEHB8
	.uleb128 0
	.uleb128 0
.LLSDACSEC3479:
	.section	.text.unlikely
	.text
	.size	_Z10Gemm_tiledR6TensorS0_i, .-_Z10Gemm_tiledR6TensorS0_i
	.section	.text.unlikely
	.size	_Z10Gemm_tiledR6TensorS0_i.cold, .-_Z10Gemm_tiledR6TensorS0_i.cold
.LCOLDE8:
	.text
.LHOTE8:
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.globl	__gxx_personality_v0
	.ident	"GCC: (GNU) 16.1.1 20260430"
	.section	.note.GNU-stack,"",@progbits
