#include <arm_neon.h>
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
    float *A = (float*) aligned_malloc(16*n*n,16);
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
        float32x4_t vt=vdupq_n_f32(A[k*n+k]);
        for(int j=k+1;j+4<n;j+=4)
        {
            float32x4_t va=vld1q_f32(&A[k*n+j]);
            va=vdivq_f32(va,vt);
            vst1q_f32(&A[k*n+j],va);
        }
        A[k*n+k] = 1.0;
        for(int i=k+1;i<n;i++)
        {
            float32x4_t vaik=vdupq_n_f32(A[i*n+k]);
            for(int j=k+1;j+4<n;j+=4)
            {
                float32x4_t vakj=vld1q_f32(&A[k*n+j]);
                float32x4_t vaij=vld1q_f32(&A[i*n+j]);
                float32x4_t vx;
                vx=vmulq_f32(vakj,vaik);
                vaij=vsubq_f32(vaij,vx);
                vst1q_f32(&A[i*n+j],vaij);
            }
        A[i*n+k] = 0;
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
