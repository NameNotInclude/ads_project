#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "AVL.h"
#include "RBT.h"

/* 用 Fisher-Yates 洗牌生成 1..n 的一个随机排列 */
static void Random_order(int n, int* order)
{
	for (int i = 0; i < n; i++)
		order[i] = i + 1;

	for (int i = n - 1; i > 0; i--)
	{
		int j = rand() % (i + 1);
		int t = order[i];
		order[i] = order[j];
		order[j] = t;
	}
}

/* 返回单调时钟的当前毫秒数（不受系统时间调整影响） */
static double now_ms(void)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

int main(int argc, char*argv[])
{

	char* check=argv[1];
	int temp=0;

	while (*check)
	{
		temp=temp*10 + *check-'0';
		check++;
	}

	check=argv[2];
	int iter=0;

	while (*check)
	{
		iter=iter*10 + *check-'0';
		check++;
	}
	
	
	int* input=(int*)malloc(sizeof(int)*(temp));
	for (int i=0;i<temp;i++)
		input[i]=i+1;

	srand((unsigned)time(NULL));
	int* randomOrder=(int*)malloc(sizeof(int)*(temp));

	if (strcmp(argv[3],"AVL")==0)
	{
		PtrToANode initial=NULL;

		//delete in same order
		double start = now_ms();
		for (int k=0;k<iter;k++)
		{
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}

			for (int i=0;i<temp;i++)
			{
				initial=deleteAVL(initial,input[i]);
			}
		}
		double end = now_ms();
		printf("AVL, delete in same order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,end-start);

		//delete in reverse order
		start = now_ms();
		for (int k=0;k<iter;k++)
		{
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}

			for (int i=temp-1;i>=0;i--)
			{
				initial=deleteAVL(initial,input[i]);
			}
		}
		end = now_ms();
		printf("AVL, delete in reverse order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,end-start);

		//delete in random order
		double total=0;
		for (int k=0;k<iter;k++)
		{
			Random_order(temp,randomOrder);
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}

			for (int i=0;i<temp;i++)
			{
				initial=deleteAVL(initial,randomOrder[i]);
			}
			end = now_ms();
			total+=end-start;
		}
		
		printf("AVL, delete in random order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,total);
	}

	else if (strcmp(argv[3],"RBT")==0)
	{
		PtrToRNode initial=NULL;

		//delete in same order
		double start = now_ms();
		for (int k=0;k<iter;k++)
		{
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}

			for (int i=0;i<temp;i++)
			{
				initial=deleteRBT(initial,input[i]);
			}
		}
		double end = now_ms();
		printf("RBT, delete in same order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,end-start);

		//delete in reverse order
		start = now_ms();
		for (int k=0;k<iter;k++)
		{
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}

			for (int i=temp-1;i>=0;i--)
			{
				initial=deleteRBT(initial,input[i]);
			}
		}
		end = now_ms();
		printf("RBT, delete in reverse order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,end-start);

		//delete in random order
		double total=0;
		for (int k=0;k<iter;k++)
		{
			Random_order(temp,randomOrder);
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}

			for (int i=0;i<temp;i++)
			{
				initial=deleteRBT(initial,randomOrder[i]);
			}
			end = now_ms();
			total+=end-start;
		}

		printf("RBT, delete in random order\nnumber of node:%d,iteration:%d time cost:%.3f ms\n",temp,iter,total);
	}

	free(randomOrder);
	free(input);
	return 0;
}
