
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include<iostream>
#include<algorithm>
using namespace std;

#define N 5

int D[N][N] = {
	0,3,9,8,3,
	5,0,1,4,2,
	6,6,0,4,5,
	2,9,2,0,7,
	7,9,3,2,0,
};

int k;

__global__ void MatAdd(int D[N][N])
{
	int i = threadIdx.x;
	int j = threadIdx.y;

	D[i][j] = min(D[i][j], D[i][k] + D[k][j]);
}


int main()
{
	int numBlocks = 1;

	dim3 threadsPerBlock(N, N);

	
   
	for (k = 1; k <= N; ++k)
	{
		MatAdd<<<numBlocks, threadsPerBlock>>>(D);
	}
}
