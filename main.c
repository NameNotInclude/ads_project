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

void testAVL(int temp,int iter,int* input,int* randomOrder,FILE* write)
{
	PtrToANode initial=NULL;
	double insert_total, delete_total;
	double start, end;

		//delete in same order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=deleteAVL(initial,input[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}
		printf("AVL, delete in same order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"AVL,%d,Same_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);

		//delete in reverse order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=temp-1;i>=0;i--)
			{
				initial=deleteAVL(initial,input[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}
		printf("AVL, delete in reverse order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"AVL,%d,Reverse_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);

		//delete in random order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			Random_order(temp,randomOrder);

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertAVL(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=deleteAVL(initial,randomOrder[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}

		printf("AVL, delete in random order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"AVL,%d,Random_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);
}


void testRBT(int temp,int iter,int* input,int* randomOrder,FILE* write)
{
	PtrToRNode initial=NULL;
	double insert_total, delete_total;
	double start, end;

		//delete in same order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=deleteRBT(initial,input[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}
		printf("RBT, delete in same order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"RBT,%d,Same_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);

		//delete in reverse order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=temp-1;i>=0;i--)
			{
				initial=deleteRBT(initial,input[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}
		printf("RBT, delete in reverse order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"RBT,%d,Reverse_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);

		//delete in random order
		insert_total = 0;
		delete_total = 0;
		for (int k=0;k<iter;k++)
		{
			Random_order(temp,randomOrder);

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=insertRBT(initial,input[i]);
			}
			end = now_ms();
			insert_total += end - start;

			start = now_ms();
			for (int i=0;i<temp;i++)
			{
				initial=deleteRBT(initial,randomOrder[i]);
			}
			end = now_ms();
			delete_total += end - start;
		}

		printf("RBT, delete in random order\nnumber of node:%d,iteration:%d insert time cost:%.3f ms, delete time cost:%.3f ms\n",temp,iter,insert_total,delete_total);
		fprintf(write,"RBT,%d,Random_Order,%d,%lf,%lf,%lf,%lf,%lf,%lf\n",temp,iter,insert_total,insert_total/iter,insert_total/iter/temp,delete_total,delete_total/iter,delete_total/iter/temp);
}
int main(int argc, char*argv[])
{
	char* check;
	int temp=0;
	int num=argc-4;
	int* datas=(int*)malloc(sizeof(int)*(num));

	for (int i=0;i<num;i++)
	{
		check=argv[i+1];
		while (*check)
		{
			temp=temp*10 + *check-'0';
			check++;
		}
		datas[i]=temp;
		temp=0;
	}

	check=argv[num+1];
	int iter=0;

	while (*check)
	{
		iter=iter*10 + *check-'0';
		check++;
	}

	FILE* write=fopen(argv[num+3],"w");
	
	fprintf(write,"Tree_Type,Number_of_Node,Delete_Order,Iteration,Insert_Time_Cost_Total,Insert_Time_Cost_Average,Insert_Time_Cost_Amortized,Delete_Time_Cost_Total,Delete_Time_Cost_Average,Delete_Time_Cost_Amortized\n");

	srand((unsigned)time(NULL));
	int* randomOrder;

	int maxN=0;
	for (int i=0;i<num;i++)
		if (datas[i]>maxN)
			maxN=datas[i];

	int* input=(int*)malloc(sizeof(int)*(maxN>0?maxN:1));
	for (int i=0;i<maxN;i++)
		input[i]=i+1;

	if (strcmp(argv[num+2],"AVL")==0)
	{
		for (int i=0;i<num;i++)
		{
			randomOrder=(int*)malloc(sizeof(int)*(datas[i]));

			testAVL(datas[i],iter,input,randomOrder,write);

			free(randomOrder);
		}
	}

	else if (strcmp(argv[num+2],"RBT")==0)
	{
		for (int i=0;i<num;i++)
		{
			randomOrder=(int*)malloc(sizeof(int)*(datas[i]));

			testRBT(datas[i],iter,input,randomOrder,write);

			free(randomOrder);
		}
	}

	free(input);
	free(datas);
	fclose(write);
	return 0;
}
