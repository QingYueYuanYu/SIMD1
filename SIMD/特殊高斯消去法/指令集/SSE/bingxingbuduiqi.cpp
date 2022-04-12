#include<iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
using namespace std;
const int n = 512;
const int m = 512;
const int p = 64;
int lp(int *A)
{
	int x=n-1;
	int count=0;
	while(!A[count])
	{
		x--;
		count++;
	}
	return x;
}
int main()
{
	int R[n*n];
	int E[n*n];
	srand(time(0));
	time_t dsec=0;
	long dnsec=0;
	for(int cycle=0;cycle<5;cycle++)
	{
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			R[i*n+j] = 0;
			E[i*n+j] = 0;
		}
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{
			E[i*n+j] = rand() & 1;
		}
    for(int i=0;i<p;i++)
	{
		int NowRow = rand()%n;
		R[NowRow*n+n-NowRow-1] = 1;
		for(int j=n-NowRow;j<n;j++)
			R[NowRow*n+j] = rand() & 1;
	}

	struct timespec startTime;
    struct timespec endTime;
    timespec_get(&startTime,TIME_UTC);

	int IsNull=0;
	for(int i=0;i<m;i++)
	{
		int IsNull = 0;
		__m128i IsNullHelp=_mm_set1_epi32(0);
		__m128i IsNullHelpE;
		for(int j=0;j+4<n;j+=4)
		{
			IsNullHelpE = _mm_loadu_si128((__m128i_u*)&E[i*n+j]);
			IsNullHelp = _mm_or_si128(IsNullHelp,IsNullHelpE);
		}
		IsNullHelp = _mm_hadd_epi32(IsNullHelp,IsNullHelp);
		IsNullHelp = _mm_hadd_epi32(IsNullHelp,IsNullHelp);
		_mm_store_ss((float*)&IsNull,(__m128)IsNullHelp);
		while(IsNull)
		{
			int IsNull2 = 0;
			__m128i IsNull2Help=_mm_set1_epi32(0);
			__m128i IsNull2HelpR;
			for(int j=0;j+4<n;j+=4)
			{
				IsNull2HelpR = _mm_loadu_si128((__m128i_u*)&R[lp(&E[i*n])*n+j]);
				IsNull2Help = _mm_or_si128(IsNull2Help,IsNull2HelpR);
			}
			IsNull2Help = _mm_hadd_epi32(IsNull2Help,IsNull2Help);
		    IsNull2Help = _mm_hadd_epi32(IsNull2Help,IsNull2Help);
			_mm_store_ss((float*)&IsNull2,(__m128)IsNull2Help);
			if(IsNull2)
			{
				int FirstNum = lp(&E[i*n]);
				__m128i vtE;
				__m128i vtR;
				for(int j=0;j+4<n;j+=4)
				{
					vtE = _mm_loadu_si128((__m128i_u*)&E[i*n+j]);
					vtR = _mm_loadu_si128((__m128i_u*)&R[FirstNum*n+j]);
					vtE = _mm_xor_si128(vtE,vtR);
					_mm_storeu_si128((__m128i_u*)&E[i*n+j],vtE);
				}
			}
			else
			{
				int FirstNum = lp(&E[i*n]);
				__m128i vtE;
				__m128i vtE2 = _mm_set1_epi32(0);
				for(int j=0;j+4<n;j+=4)
				{
					vtE = _mm_loadu_si128((__m128i_u*)&E[i*n+j]);
					_mm_storeu_si128((__m128i_u*)&R[FirstNum*n+j],vtE);
					_mm_storeu_si128((__m128i_u*)&E[i*n+j],vtE2);
				}
				break;
			}

			IsNull = 0;
			IsNullHelp=_mm_set1_epi32(0);
		    for(int j=0;j+4<n;j+=4)
			{
				IsNullHelpE = _mm_loadu_si128((__m128i_u*)&E[i*n+j]);
			    IsNullHelp = _mm_or_si128(IsNullHelp,IsNullHelpE);
			}
		    IsNullHelp = _mm_hadd_epi32(IsNullHelp,IsNullHelp);
		    IsNullHelp = _mm_hadd_epi32(IsNullHelp,IsNullHelp);
			_mm_store_ss((float*)&IsNull,(__m128)IsNullHelp);
		}
	}

	timespec_get(&endTime,TIME_UTC);
    dsec+=endTime.tv_sec - startTime.tv_sec;
    dnsec+=endTime.tv_nsec-startTime.tv_nsec;
	}
    printf("%llu.%09llus\n",dsec/5,dnsec/5);
    cout<<"Hello World!"<<endl;
	return 0;
}
