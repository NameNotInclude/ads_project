#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "AVL.h"
#include "RBT.h"
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

	if (strcmp(argv[3],"AVL")==0)
	{
		PtrToANode initial=NULL;

		//delete in same order
		size_t start = time(NULL);
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
		size_t end = time(NULL);
		printf("AVL, delete in same order\nnumber of node:%d,iteration:%d time cost:%ld\n",temp,iter,(long)(end-start));

		//delete in reverse order
		start = time(NULL);
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
		end = time(NULL);
		printf("AVL, delete in reverse order\nnumber of node:%d,iteration:%d time cost:%ld\n",temp,iter,(long)(end-start));
	}

	else if (strcmp(argv[2],"RBT")==0)
	{
		PtrToRNode initial=NULL;

		//delete in same order
		size_t start = time(NULL);
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
		size_t end = time(NULL);
		printf("RBT, delete in same order\nnumber of node:%d,iteration:%d time cost:%ld\n",temp,iter,(long)(end-start));

		//delete in reverse order
		start = time(NULL);
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
		end = time(NULL);
	printf("RBT, delete in reverse order\nnumber of node:%d,iteration:%d time cost:%ld\n",temp,iter,(long)(end-start));	}

	free(input);
	return 0;
}
