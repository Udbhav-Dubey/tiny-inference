movss   (%rcx), %xmm2
movaps  %xmm2, %xmm3
shufps  $0, %xmm3, %xmm3

.L88:
movups  (%r9,%rdx), %xmm0
movups  (%rax,%rdx), %xmm4

mulps   %xmm1, %xmm0
addps   %xmm4, %xmm0

movups  %xmm0, (%rax,%rdx)

addq    $16, %rdx
cmpq    %rdx, %rbx
jne     .L88

/*Broadcast ak
Vectorized inner j loop
4-float SIMD update*/
