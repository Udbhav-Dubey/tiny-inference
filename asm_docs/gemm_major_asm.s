//after march=native

//❯ grep -i ymm GEMM.s

//❯ grep -i ymm GEMM_native.txt
	vbroadcastss	%xmm2, %ymm1
	vmovups	(%rsi,%rcx), %ymm0
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vmovups	%ymm0, (%rax,%rcx)
	vbroadcastss	%xmm2, %ymm3
	vmovaps	%ymm3, %ymm1
	vmovups	(%rdx,%rcx), %ymm0
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vmovups	%ymm0, (%rax,%rcx)
	
	
//now gcc has recognised that we cache ak and it broadcasting direcly and fma appears which directly does multipli and add in one single instruction and its ps so so much faster , so on native machine rather than genric it recognised the importance and did the work 

//❯ grep -i xmm GEMM.s

	vmovss	(%r10), %xmm2
	vbroadcastss	%xmm2, %ymm1
	vbroadcastss	%xmm2, %xmm0
	vmovups	0(%r13,%r12,4), %xmm3
	vfmadd132ps	(%rbx,%rsi,4), %xmm3, %xmm0
	vmovups	%xmm0, 0(%r13,%r12,4)
	vmovss	(%rbx,%rdi,4), %xmm0
	vfmadd213ss	0(%r13,%rsi,4), %xmm2, %xmm0
	vmovss	%xmm0, 0(%r13,%rsi,4)
	vmovss	(%rbx,%rsi,4), %xmm0
	vfmadd213ss	0(%r13,%rdi,4), %xmm2, %xmm0
	vmovss	%xmm0, 0(%r13,%rdi,4)
	vmovss	0(%r13,%rsi,4), %xmm4
	vfmadd132ss	(%rbx,%rcx,4), %xmm4, %xmm2
	vmovss	%xmm2, 0(%r13,%rsi,4)
	vmovss	(%r10), %xmm2
	vmovss	(%rdi,%rcx), %xmm0
	vfmadd213ss	(%rax,%rcx), %xmm2, %xmm0
	vmovss	%xmm0, (%rax,%rcx)
	vxorps	%xmm1, %xmm1, %xmm1
	vmovss	(%rax), %xmm0
	vmovq	(%rsi), %xmm2
	vinsertps	$0x10, (%rax,%rcx,4), %xmm0, %xmm0
	vmovq	%xmm0, %xmm0
	vmulps	%xmm2, %xmm0, %xmm0
	vaddss	%xmm0, %xmm1, %xmm1
	vmovshdup	%xmm0, %xmm0
	vaddss	%xmm1, %xmm0, %xmm1
	vmovss	(%r12,%rsi,4), %xmm3
	vfmadd231ss	0(%rbp,%rax,4), %xmm3, %xmm1
	vmovss	%xmm1, 0(%r13,%r9,4)
	vxorps	%xmm1, %xmm1, %xmm1
	vmovss	%xmm1, 0(%r13,%r9,4)
	vmovss	(%rdi), %xmm2
	vbroadcastss	%xmm2, %ymm3
	vmovups	(%r12,%r15,4), %xmm4
	vfmadd132ps	(%rax,%rsi,4), %xmm4, %xmm3
	vmovups	%xmm3, (%r12,%r15,4)
	vmovss	(%rax,%rsi,4), %xmm0
	vfmadd213ss	(%r15), %xmm2, %xmm0
	vmovss	%xmm0, (%r15)
	vmovss	(%rax,%rsi,4), %xmm0
	vfmadd213ss	(%r12,%r15,4), %xmm2, %xmm0
	vmovss	%xmm0, (%r12,%r15,4)
	vmovss	(%r12,%rsi,4), %xmm5
	vfmadd132ss	(%rax,%r10,4), %xmm5, %xmm2
	vmovss	%xmm2, (%r12,%rsi,4)
	vmovss	(%rdx,%rcx), %xmm0
	vfmadd213ss	(%rax,%rcx), %xmm2, %xmm0
	vmovss	%xmm0, (%rax,%rcx)
	
	
//	this one is doing reduction aswell 
	
//❯ grep -i ymm GEMM.s

	vbroadcastss	%xmm2, %ymm1
	vmovups	(%rsi,%rcx), %ymm0
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vmovups	%ymm0, (%rax,%rcx)
	vbroadcastss	%xmm2, %ymm3
	vmovaps	%ymm3, %ymm1
	vmovups	(%rdx,%rcx), %ymm0
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vmovups	%ymm0, (%rax,%rcx)
	
//	this again shows gcc recognised and did broadcast fma store 8 floats
	
	
//❯ grep -i vfmadd GEMM.s
//nothing
//gcc is not even doing fused multiplication-add it should do it imagine the performance 
//❯ grep -i mulss GEMM.s
	mulss	(%rbx,%r8,4), %xmm2
	mulss	%xmm2, %xmm0
	mulss	(%r12,%r14,4), %xmm0
	mulss	(%r12,%r10,4), %xmm1
	mulss	(%r15,%rdi,4), %xmm0
	mulss	(%r12,%rdx,4), %xmm2
	mulss	%xmm2, %xmm0
	
//❯ grep -i vmul GEMM.s
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vfmadd132ps	(%rbx,%rsi,4), %xmm3, %xmm0
	vfmadd213ss	0(%r13,%rsi,4), %xmm2, %xmm0
	vfmadd213ss	0(%r13,%rdi,4), %xmm2, %xmm0
	vfmadd132ss	(%rbx,%rcx,4), %xmm4, %xmm2
	vfmadd213ss	(%rax,%rcx), %xmm2, %xmm0
	vfmadd231ss	0(%rbp,%rax,4), %xmm3, %xmm1
	vfmadd213ps	(%rax,%rcx), %ymm1, %ymm0
	vfmadd132ps	(%rax,%rsi,4), %xmm4, %xmm3
	vfmadd213ss	(%r15), %xmm2, %xmm0
	vfmadd213ss	(%r12,%r15,4), %xmm2, %xmm0
	vfmadd132ss	(%rax,%r10,4), %xmm5, %xmm2
	vfmadd213ss	(%rax,%rcx), %xmm2, %xmm0
	

/*Before -march=native:
- No FMA instructions observed.

After -march=native:
- Multiple vfmadd instructions observed.
- GCC successfully fused multiply-add operations.
- AVX2 + FMA code generation confirmed.

Conclusion:
GCC recognized GEMM's C += A*B pattern and emitted hardware FMA instructions automatically.*/


