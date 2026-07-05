#include <immintrin.h>
void add(const float*a,const float*b,float*out){
    __m256va=_mm256_loadu_ps(a);
    __m256va=_mm256_loadu_ps(b);
    __m256vc=_mm256_add_ps(va,vb);
    __m256_storeu_ps(out,vc);
}
void multiply(const float*a,const float*b,float*out){
    __m256va=_mm256_loadu_ps(a);
    __m256vb=_mm256_loadu_ps(b);
    __m256vc=_mm256_mul_ps(va,vb);
    __m256_storeu_ps(out,vc);
}
void scaler(const float*a,float*out,float k){
    __m256va=_mm256_loadu_ps(a);
    __m256vk=_mm256_set1_ps(k);
    __m256vc=__m256_mul_ps(va,vk);
    __m256_storeu_ps(out,vc);
}
