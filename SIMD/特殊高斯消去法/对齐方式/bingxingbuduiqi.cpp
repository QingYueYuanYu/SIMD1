#include<iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <arm_neon.h>
using namespace std;
const int n = 512;//列数
const int m = 512;//被消元行行数
const int p = 64;//消元行行数
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
	//初始化
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

	//特殊高斯消去法
	struct timespec startTime;
    struct timespec endTime;
    timespec_get(&startTime,TIME_UTC);

	int IsNull=0;
	for(int i=0;i<m;i++)
	{
		int IsNull = 0;
		int32x4_t IsNullHelp=vdupq_n_s32(0);
		int32x4_t IsNullHelpE; 
		for(int j=0;j+4<n;j+=4)
		{
			IsNullHelpE = vld1q_s32(&E[i*n+j]);
			IsNullHelp = vorrq_s32(IsNullHelp,IsNullHelpE);
		}
		IsNullHelp = vpaddq_s32(IsNullHelp,IsNullHelp);
		IsNullHelp = vpaddq_s32(IsNullHelp,IsNullHelp);
		IsNull = vgetq_lane_s32(IsNullHelp,0);
		while(IsNull)
		{
			int IsNull2 = 0;
			int32x4_t IsNull2Help=vdupq_n_s32(0);
			int32x4_t IsNull2HelpR;
			for(int j=0;j+4<n;j+=4)
			{
				IsNull2HelpR = vld1q_s32(&R[lp(&E[i*n])*n+j]);
				IsNull2Help = vorrq_s32(IsNull2Help,IsNull2HelpR);
			}
			IsNull2Help = vpaddq_s32(IsNull2Help,IsNull2Help);
		    IsNull2Help = vpaddq_s32(IsNull2Help,IsNull2Help);
		    IsNull2 = vgetq_lane_s32(IsNull2Help,0);
			if(IsNull2)
			{
				int FirstNum = lp(&E[i*n]);
				int32x4_t vtE;
				int32x4_t vtR;
				for(int j=0;j+4<n;j+=4)
				{
					vtE = vld1q_s32(&E[i*n+j]);
					vtR = vld1q_s32(&R[FirstNum*n+j]);
					vtE = veorq_s32(vtE,vtR);
					vst1q_s32(&E[i*n+j],vtE);
				}
			}
			else
			{
				int FirstNum = lp(&E[i*n]);
				int32x4_t vtE;
				int32x4_t vtE2 = vdupq_n_s32(0);
				for(int j=0;j+4<n;j+=4)
				{
					vtE = vld1q_s32(&E[i*n+j]);
					vst1q_s32(&R[FirstNum*n+j],vtE);
					vst1q_s32(&E[i*n+j],vtE2);
				}
				break;
			}

			IsNull = 0;
			IsNullHelp=vdupq_n_s32(0);
		    for(int j=0;j+4<n;j+=4)
			{
				IsNullHelpE = vld1q_s32(&E[i*n+j]);
			    IsNullHelp = vorrq_s32(IsNullHelp,IsNullHelpE);
			}
		    IsNullHelp = vpaddq_s32(IsNullHelp,IsNullHelp);
		    IsNullHelp = vpaddq_s32(IsNullHelp,IsNullHelp);
		    IsNull = vgetq_lane_s32(IsNullHelp,0);
		}
	}

	timespec_get(&endTime,TIME_UTC);
    dsec+=endTime.tv_sec - startTime.tv_sec;
    dnsec+=endTime.tv_nsec-startTime.tv_nsec;
	}
    printf("%llu.%09llus\n",dsec/5,dnsec/5);
    cout<<endl;
	return 0;
}