#include <pmmintrin.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
using namespace std;
const int n=256;
int main()
{
    float A[n*n];
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
        __m128 vt;
        vt=_mm_set1_ps(A[k*n+k]);
        for(int j=k+1;j+4<n;j+=4)
            {
                __m128 va;
                va = _mm_loadu_ps(&A[k*n+j]);
                va = _mm_div_ps(va,vt);
                _mm_storeu_ps(&A[k*n+j],va);
            }
        A[k*n+k] = 1.0;
        for(int i=k+1;i<n;i++)
        {
            __m128 vaik;
            vaik = _mm_set1_ps(A[i*n+k]);
            for(int j=k+1;j+4<n;j+=4)
            {
                __m128 vakj;
                __m128 vaij;
                __m128 vx;
                vakj = _mm_loadu_ps(&A[k*n+j]);
                vaij = _mm_loadu_ps(&A[i*n+j]);
                vx = _mm_mul_ps(vakj,vaik);
                vaij = _mm_sub_ps(vaij,vx);
                _mm_storeu_ps(&A[i*n+j],vaij);
            }
        A[i*n+k] = 0;
        }
    }
    timespec_get(&endTime,TIME_UTC);
    time_t dsec=endTime.tv_sec - startTime.tv_sec;
    long dnsec=endTime.tv_nsec-startTime.tv_nsec;
    printf("%llu.%09llus\n",dsec,dnsec);
    cout<<endl;
    return 0;
}
