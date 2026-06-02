movups  -16(%r9), %xmm4

mulps   %xmm4, %xmm0

addss   %xmm0, %xmm1

movaps  %xmm0, %xmm2
shufps  $85, %xmm0, %xmm2
addss   %xmm1, %xmm2

movaps  %xmm0, %xmm1
unpckhps %xmm0, %xmm1

shufps  $255, %xmm0, %xmm0

addss   %xmm2, %xmm1
addss   %xmm0, %xmm1

/*SIMD multiply
Horizontal reduction
Back to scalar sum*/
