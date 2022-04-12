#include<iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <pmmintrin.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <immintrin.h>
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
		__m512i IsNullHelp=_mm512_set1_epi32(0);
		__m512i IsNullHelpE;
		__m128i IsNullHelp1;
		__m128i IsNullHelp2;
		__m128i IsNullHelp3;
		__m128i IsNullHelp4;
		for(int j=0;j+16<n;j+=16)
		{
			IsNullHelpE = _mm512_loadu_si512((__m512i_u*)&E[i*n+j]);
			IsNullHelp = _mm512_or_si512(IsNullHelp,IsNullHelpE);
		}
		IsNullHelp1 = _mm512_extracti64x2_epi64(IsNullHelp,0);
		IsNullHelp2 = _mm512_extracti64x2_epi64(IsNullHelp,1);
		IsNullHelp3 = _mm512_extracti64x2_epi64(IsNullHelp,2);
		IsNullHelp4 = _mm512_extracti64x2_epi64(IsNullHelp,3);
		IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp2);
		IsNullHelp3 = _mm_hadd_epi32(IsNullHelp3,IsNullHelp4);
		IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp3);
		IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp1);
		IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp1);
		_mm_store_ss((float*)&IsNull,(__m128)IsNullHelp1);
		while(IsNull)
		{
			int IsNull2 = 0;
			__m512i IsNull2Help=_mm512_set1_epi32(0);
			__m512i IsNull2HelpR;
			__m128i IsNull2Help1;
			__m128i IsNull2Help2;
			__m128i IsNull2Help3;
			__m128i IsNull2Help4;
			for(int j=0;j+16<n;j+=16)
			{
				IsNull2HelpR = _mm512_loadu_si512((__m512i_u*)&R[lp(&E[i*n])*n+j]);
				IsNull2Help = _mm512_or_si512(IsNull2Help,IsNull2HelpR);
			}
			IsNull2Help1 = _mm512_extracti64x2_epi64(IsNull2Help,0);
		    IsNull2Help2 = _mm512_extracti64x2_epi64(IsNull2Help,1);
		    IsNull2Help3 = _mm512_extracti64x2_epi64(IsNull2Help,2);
		    IsNull2Help4 = _mm512_extracti64x2_epi64(IsNull2Help,3);
		    IsNull2Help1 = _mm_hadd_epi32(IsNull2Help1,IsNull2Help2);
		    IsNull2Help3 = _mm_hadd_epi32(IsNull2Help3,IsNull2Help4);
		    IsNull2Help1 = _mm_hadd_epi32(IsNull2Help1,IsNull2Help3);
		    IsNull2Help1 = _mm_hadd_epi32(IsNull2Help1,IsNull2Help1);
		    IsNull2Help1 = _mm_hadd_epi32(IsNull2Help1,IsNull2Help1);
		    _mm_store_ss((float*)&IsNull2,(__m128)IsNull2Help1);
			if(IsNull2)
			{
				int FirstNum = lp(&E[i*n]);
				__m512i vtE;
				__m512i vtR;
				for(int j=0;j+16<n;j+=16)
				{
					vtE = _mm512_loadu_si512((__m512i_u*)&E[i*n+j]);
					vtR = _mm512_loadu_si512((__m512i_u*)&R[FirstNum*n+j]);
					vtE = _mm512_xor_si512(vtE,vtR);
					_mm512_storeu_si512((__m512i_u*)&E[i*n+j],vtE);
				}
			}
			else
			{
				int FirstNum = lp(&E[i*n]);
				__m512i vtE;
				__m512i vtE2 = _mm512_set1_epi32(0);
				for(int j=0;j+16<n;j+=16)
				{
					vtE = _mm512_loadu_si512((__m512i_u*)&E[i*n+j]);
					_mm512_storeu_si512((__m512i_u*)&R[FirstNum*n+j],vtE);
					_mm512_storeu_si512((__m512i_u*)&E[i*n+j],vtE2);
				}
				break;
			}

			IsNull = 0;
			IsNullHelp=_mm512_set1_epi32(0);
		    for(int j=0;j+16<n;j+=16)
			{
				IsNullHelpE = _mm512_loadu_si512((__m512i_u*)&E[i*n+j]);
			    IsNullHelp = _mm512_or_si512(IsNullHelp,IsNullHelpE);
			}
		    IsNullHelp1 = _mm512_extracti64x2_epi64(IsNullHelp,0);
		    IsNullHelp2 = _mm512_extracti64x2_epi64(IsNullHelp,1);
		    IsNullHelp3 = _mm512_extracti64x2_epi64(IsNullHelp,2);
		    IsNullHelp4 = _mm512_extracti64x2_epi64(IsNullHelp,3);
		    IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp2);
		    IsNullHelp3 = _mm_hadd_epi32(IsNullHelp3,IsNullHelp4);
		    IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp3);
		    IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp1);
		    IsNullHelp1 = _mm_hadd_epi32(IsNullHelp1,IsNullHelp1);
		    _mm_store_ss((float*)&IsNull,(__m128)IsNullHelp1);
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
