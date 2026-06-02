movaps  %xmm2, %xmm1
shufps  $0, %xmm1, %xmm1

.L8:
movups  (%r8,%rsi), %xmm0
movups  (%rax,%rsi), %xmm3

mulps   %xmm1, %xmm0
addps   %xmm3, %xmm0

movups  %xmm0, (%rax,%rsi)

addq    $16, %rsi
cmpq    %rsi, %rdx
jne     .L8

/*
Broadcast ak
Load 4 B values
Load 4 C values
Multiply 4 floats
Add 4 floats
Store 4 floats */

.L14:
movss   (%r8,%rsi), %xmm0
mulss   %xmm2, %xmm0
addss   (%rax,%rsi), %xmm0
movss   %xmm0, (%rax,%rsi)

//Remaining elements handled scalar

