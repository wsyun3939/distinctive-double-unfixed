#include <stdio.h>
#include <stdlib.h>
#include "IntDequeue.h"
#include "lower_bound.h"
#include "branch_and_bound.h"
#include "data_info.h"


#ifdef EITHER
int branch_and_bound(IntDequeue *q, int UB, int UB_cur, int LB, direction Dir,clock_t start) {
	static int num_ret;
	static int depth = 0;
	static int SecondPosition = 0;
	static int MinRelocation = 0;
	static int BlockSpace = 0;
	if (UB == UB_cur) {
		depth = 0;
		return MinRelocation = UB;
	}
	int i = 0;
	int j = 0;
	int k = 0;
	int PriorityEdge = 0;
	int LB_temp = 0;
	direction dir = Dir;
	direction DirNext = Dir;
	IntDequeue *q_temp = NULL;
	
	clock_t end;
	end=clock();
	if(((double)(end-start)/CLOCKS_PER_SEC)>7200){
		depth=0;
		return -1;
	}
	switch (dir)
	{
	case both:
		qsort(q+1, STACK-1, sizeof(IntDequeue), (int(*)(const void *, const void *))pricmp);

#if TEST==0
		Array_print(q);
#endif
		
		break;
	}

	while ((q[0].front == q[0].min_idx) || ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx)) {
		if (q[0].front == q[0].min_idx) {
			DequeFront(&q[0], &num_ret);


#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif


		}
		else  if ((q[0].front + q[0].num - 1) % q[0].max == q[0].min_idx) {
			DequeRear(&q[0], &num_ret);


#if TEST==0
			printf("Number Retrieval:%d\n", num_ret);
			Array_print(q);
#endif


		}
		if (insert_sort(q)) {
			depth = 0;
			return MinRelocation = UB_cur;
		}


#if TEST==0
		Array_print(q);
#endif

		if(Array_check(q)==1){
			printf("ブロック数が３個以下\n");

			MinRelocation=LB+depth;
			depth=0;
			return MinRelocation;
		}
	}
	//　積み替え操作　
	depth++;

#if TEST==0
		printf("Block Relocation(depth=%d)\n",depth);
#endif

	int BG_index[STACK]={0};
		dir=q[0].dir;
		if(dir==both) dir= q[0].que[q[0].front] < q[0].que[(q[0].front + q[0].num - 1) % q[0].max] ? lower : upper;
		for(k=0;k<=1;k++){	//下界値を与える方向から順に積み替えを吟味する
			LB_temp=LB;
			if (dir == lower) {
			PriorityEdge = q[0].que[q[0].front];
			SecondPosition = q[0].que[(q[0].front + 1) % q[0].max];
		}
		else if (dir == upper) {
			PriorityEdge = q[0].que[(q[0].front + q[0].num - 1) % q[0].max];
			SecondPosition = q[0].que[(q[0].front + q[0].num - 2) % q[0].max];
		}
		LB_temp-=q[0].LB;
		Deque(q,&num_ret,dir);
		LB_temp +=q[0].LB;
		if (LB_temp + depth > UB_cur);
		else{
		if (SecondPosition == q[0].que[q[0].min_idx]) { //積み替え後，取り出しが可能な場合
			DirNext = both;
				q_temp = malloc(STACK*(sizeof *q_temp));
				Array_initialize(q_temp);
				Array_copy(q_temp, q);
				if(PriorityEdge == q[0].que[q[0].min_idx] + 1){	//ドミナンス

#if TEST==0
				printf("Priority Retrieval:%d\n", num_ret);
#endif

				Deque(&q_temp[0], &num_ret, dir);

#if TEST==0
				printf("Number Retrieval:%d\n", num_ret);
#endif

				if(Array_check(q_temp)==1){
		
					printf("ブロック数が３個以下\n");

					MinRelocation=LB_temp+depth;
					depth=0;
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}			
				insert_media(q_temp, 0);
				if (branch_and_bound(q_temp, UB, UB_cur, LB_temp, DirNext)) {
					Array_terminate(q_temp);
					free(q_temp);
					return MinRelocation;
				}

#if TEST==0
				printf("Lower termination.\n");
				Array_print(q_temp);
#endif

				Array_terminate(q_temp);
				free(q_temp);
				}
				else{
					for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q_temp[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q_temp[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替えが可能
						Enque(&q_temp[j],PriorityEdge,dir);
						if (branch_and_bound(q_temp, UB, UB_cur, LB_temp, DirNext)) {
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q_temp[j],PriorityEdge,dir);
						if (branch_and_bound(q_temp, UB, UB_cur, LB_temp+1, DirNext)) {
						Array_terminate(q_temp);
						free(q_temp);
						return MinRelocation;
					}
					Array_copy(q_temp, q);

#if TEST==0
					Array_print(q_temp);
#endif

					}
					}
					}
					}
		}
		else{
			for(j=STACK-1;j>0;j--){
						BG_index[j]=Enque(&q[j],PriorityEdge,dir);
						if(BG_index[j]!=-1) Deque(&q[j],&num_ret,dir);
					}
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==0){ //BG積み替え可能
						Enque(&q[j],PriorityEdge,dir);
						if (branch_and_bound(q, UB, UB_cur, LB_temp, dir)) {
							return MinRelocation;
						}
						Deque(&q[j],&num_ret,dir);

#if TEST==0
				Array_print(q);
#endif

						
					}
					}
					if(LB_temp+depth+1>UB_cur);
					else{
					for(j=STACK-1;j>0;j--){
					if(BG_index[j]==1){ //BG積み替え不可能
						Enque(&q[j],PriorityEdge,dir);
						if (branch_and_bound(q, UB, UB_cur, LB_temp+1, dir)) {
						return MinRelocation;
					}
					Deque(&q[j],&num_ret,dir);

#if TEST==0
					Array_print(q);
#endif

						
					}
					}
					}
		}	
		}
		Enque(q,PriorityEdge,dir);
		LB_temp=LB;
		if(dir==lower) dir=upper;
		else dir=lower;
		}
		depth--;
	if(depth == 0) {
		UB_cur++;

#if TEST==0
		printf("UB++\n");
#endif

		if (branch_and_bound(q, UB,UB_cur, LB,both)) {
			return MinRelocation;
		}
		return -1;
	}
	else {
		return 0;
	}
}
#endif







