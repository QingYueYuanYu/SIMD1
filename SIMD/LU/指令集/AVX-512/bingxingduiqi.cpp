#include <immintrin.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <memory>
using namespace std;
const int n=256;
void* aligned_malloc(size_t size, size_t alignment)
{
	size_t offset = alignment - 1 + sizeof(void*);
	void * originalP = malloc(size + offset);
	size_t originalLocation = reinterpret_cast<size_t>(originalP);
	size_t realLocation = (originalLocation + offset) & ~(alignment - 1);
	void * realP = reinterpret_cast<void*>(realLocation);
	size_t originalPStorage = realLocation - sizeof(void*);
	*reinterpret_cast<void**>(originalPStorage) = originalP;
	return realP;
}

void aligned_free(void* p)
{
	size_t originalPStorage = reinterpret_cast<size_t>(p) - sizeof(void*);
	free(*reinterpret_cast<void**>(originalPStorage));
}
int main()
{
    float *A = (float*) aligned_malloc(64*n*n,64);
    cout<<A<<endl;
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<i;j++)
            A[i*n+j] = 0;
        A[i*n+i] = 1;
        for(int j=i+1;j<n;j++)
            A[i*n+j] = double(rand());

    }
    for(int k=0;k<n;k++)
        for(int i=k+1;i<n;i++)
        for(int j=0;j<n;j++)
        A[i*n+j] += A[k*n+j];
    struct timespec startTime;
    struct timespec endTime;
    timespec_get(&startTime,TIME_UTC);
    for(int k=0;k<n;k++)
    {
        __m512 vt;
        vt=_mm512_set1_ps(A[k*n+k]);
        if(k%16==15)
        {
            for(int j=k+1;j+16<n;j+=16)
            {
                __m512 va;
                va = _mm512_load_ps(&A[k*n+j]);
                va = _mm512_div_ps(va,vt);
                _mm512_store_ps(&A[k*n+j],va);
            }
            A[k*n+k] = 1.0;
            for(int i=k+1;i<n;i++)
            {
                __m512 vaik;
                vaik = _mm512_set1_ps(A[i*n+k]);
                for(int j=k+1;j+16<n;j+=16)
                {
                    __m512 vakj;
                    __m512 vaij;
                    __m512 vx;
                    vakj = _mm512_load_ps(&A[k*n+j]);
                    vaij = _mm512_load_ps(&A[i*n+j]);
                    vx = _mm512_mul_ps(vakj,vaik);
                    vaij = _mm512_sub_ps(vaij,vx);
                    _mm512_store_ps(&A[i*n+j],vaij);
                }
            A[i*n+k] = 0;
            }
        }
        else
        {
            for(int j=k+1;j+16<n;j+=16)
            {
                __m512 va;
                va = _mm512_loadu_ps(&A[k*n+j]);
                va = _mm512_div_ps(va,vt);
                _mm512_storeu_ps(&A[k*n+j],va);
            }
            A[k*n+k] = 1.0;
            for(int i=k+1;i<n;i++)
            {
                __m512 vaik;
                vaik = _mm512_set1_ps(A[i*n+k]);
                for(int j=k+1;j+16<n;j+=16)
                {
                    __m512 vakj;
                    __m512 vaij;
                    __m512 vx;
                    vakj = _mm512_loadu_ps(&A[k*n+j]);
                    vaij = _mm512_loadu_ps(&A[i*n+j]);
                    vx = _mm512_mul_ps(vakj,vaik);
                    vaij = _mm512_sub_ps(vaij,vx);
                    _mm512_storeu_ps(&A[i*n+j],vaij);
                }
            A[i*n+k] = 0;
            }
        }
    }
    timespec_get(&endTime,TIME_UTC);
    time_t dsec=endTime.tv_sec - startTime.tv_sec;
    long dnsec=endTime.tv_nsec-startTime.tv_nsec;
    printf("%llu.%09llus\n",dsec,dnsec);
    cout<<endl;
    aligned_free(A);
    return 0;
}

