#include <iostream>#include <iomanip>#include <stdlib.h>#include <time.h>using namespace std;const int n=256;int main(){    float A[n*n];    for(int i=0;i<n;i++)    {        for(int j=0;j<i;j++)            A[i*n+j] = 0;        A[i*n+i] = 1;        for(int j=i+1;j<n;j++)            A[i*n+j] = double(rand());    }    for(int k=0;k<n;k++)        for(int i=k+1;i<n;i++)        for(int j=0;j<n;j++)        A[i*n+j] += A[k*n+j];    struct timespec startTime;
    struct timespec endTime;
    timespec_get(&startTime,TIME_UTC);    for(int k=0;k<n;k++)    {        for(int j=k+1;j<n;j++)            A[k*n+j] = A[k*n+j]/A[k*n+k];        A[k*n+k] = 1.0;        for(int i=k+1;i<n;i++)        {            for(int j=k+1;j<n;j++)            A[i*n+j] = A[i*n+j] - A[i*n+k]*A[k*n+j];        A[i*n+k] = 0;        }    }    timespec_get(&endTime,TIME_UTC);
    time_t dsec=endTime.tv_sec - startTime.tv_sec;
    long dnsec=endTime.tv_nsec-startTime.tv_nsec;
    printf("%llu.%09llus\n",dsec,dnsec);
    cout<<endl;    return 0;}