#include<iostream>
#include <iomanip>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
using namespace std;
const int n = 256;//列数
const int m = 256;//被消元行行数
const int p = 32;//消元行行数
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
	int R[n][n];
	int E[n][n];
	//初始化
	srand(time(0));
	time_t dsec=0;
	long dnsec=0;
	for(int cycle=0;cycle<5;cycle++)
	{
	for(int i=0;i<n;i++)
		for(int j=0;j<n;j++)
		{
			R[i][j] = 0;
			E[i][j] = 0;
		}
	for(int i=0;i<m;i++)
		for(int j=0;j<n;j++)
		{
			E[i][j] = rand() & 1;
		}
    for(int i=0;i<p;i++)
	{
		int NowRow = rand()%n;
		R[NowRow][n-NowRow-1] = 1;
		for(int j=n-NowRow;j<n;j++)
			R[NowRow][j] = rand() & 1;
	}

	//特殊高斯消去法
	struct timespec startTime;
    struct timespec endTime;
    timespec_get(&startTime,TIME_UTC);

	int IsNull=0;
	for(int i=0;i<m;i++)
	{
		for(int j=0;j<n;j++)
			IsNull = IsNull | E[i][j];
		while(IsNull)
		{
			int IsNull2 = 0;
			for(int j=0;j<n;j++)
				IsNull2 = IsNull2 | R[lp(E[i])][j];
			if(IsNull2)
			{
				int FirstNum = lp(E[i]);
				for(int j=0;j<n;j++)
					E[i][j] = E[i][j] ^ R[FirstNum][j];
			}
			else
			{
				int FirstNum = lp(E[i]);
				for(int j=0;j<n;j++)
				{
				    R[FirstNum][j] = E[i][j];
					E[i][j] = 0;
				}
				break;
			}

			IsNull = 0;
			for(int j=0;j<n;j++)
			    IsNull = IsNull | E[i][j];
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