#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"

#define BUFFER 256
/*変数の定義
Stack:スタックの最大個数
Tier:積み上げられるブロックの最大値
Nblocking_lower:priorityの下側に位置するブロック数
Nblocking_upper:priorityの上側に位置するブロック数
stack[i]:i番目のスタック
Nblocking_upper_again:上側から移動させた後もブロッキングブロックとなる個数
Nblocking_lower_again:下側から移動させた後もブロッキングブロックとなる個数

*/

int main(void) {
	clock_t start = clock();
	IntDequeue* stack = malloc(STACK*(sizeof *stack));
	Array_initialize(stack);
	int nblock = NBLOCK;
	int i, j, x, l;
	int k = 0;
	int miss=0;
	int sum = 0;
	int difference=0;
	char filename[BUFFER];
	FILE *fp_read = NULL;
	FILE *fp_write = NULL;
		for (int a = NUMBER; a < NUMBER+100*TIER; a++) {
			FILE * fp = NULL;
			sprintf(filename, "/Users/watanabeshun/Documents/Benchmark/%d-%d-%d/%05d.txt", TIER, STACK, nblock, a);
			printf("%s\n", filename);

			//	読み込みモードでファイルを開く
			fp=fopen(filename, "r");

			// スタック数と高さを読み込む　//
			fscanf(fp, "%d %d", &i, &j);
			for (i = 0; i < STACK; i++) {
				fscanf(fp, "%d ", &l);
				for (j = 0; j < l; j++) {
					fscanf(fp, "%d ", &x);
					EnqueRear(&stack[i], x);
				}
			}

			
			//*---LB1---*//
			int LB1 = lower_bound1(stack);
			printf("LB1:%d\n", LB1);


			qsort(stack, STACK, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);
			printf("sort:\n");
			Array_print(stack);
			//int UB=UpperBound(stack,both);
			int UB_cur = LB1;
			int min_relocation = branch_and_bound(stack, 100, UB_cur, LB1, both);
			//int min_relocation = enumerate_relocation(stack, depth,both);
			sum += min_relocation;
			if (min_relocation == LB1) {
				k++;
			}
			difference+=min_relocation-LB1;
			printf("min_relocation:%d,difference:%d\n", min_relocation, min_relocation - LB1); 
			fclose(fp);

			if (a % 100 == 1) {
				sprintf(filename, "/Users/watanabeshun/Documents/Benchmark/%d-%d-%d.csv", TIER, STACK, nblock);
				fp_read=fopen(filename, "r");
				sprintf(filename, "/Users/watanabeshun/Documents/Benchmark/%d-%d-%d_unfixed.csv", TIER, STACK, nblock);
				fp_write = fopen(filename, "w");
			}
			fscanf(fp_read, "%d ", &x);
			if (x != min_relocation) {
				printf("missmatch\n");
				printf("%d\n", a);
				miss++;
			}
			//「fprintfExample.txt」に「aは100です」と書き込む
    		fprintf(fp_write, "%d\n", min_relocation);
		Array_clear(stack);
		if (a % 100 == 0) {
				nblock++;
				fclose(fp_read);
				fclose(fp_write);
			}
		}
	Array_terminate(stack);
	clock_t end=clock();
	printf("time:%f,match:%d,missmatch:%d,ave_relocation:%f,ave_difference%f\n", (double)(end - start) / CLOCKS_PER_SEC, k,miss,(double)sum/(100*TIER),(double)difference/(100*TIER-k));
	return 0;
}
