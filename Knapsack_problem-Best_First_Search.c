#include<stdio.h>
#include<malloc.h>

#define MAX_ELEMENT 10

int n = 5;
int W = 9;
int p[6] = { 0,20,30,35,12,3 };
int w[6] = { 0,2,5,7,3,1 };
int maxprofit = 0;

typedef struct Node {
	int level;
	int profit;
	int weight;
	float bound;
}node;

typedef struct HeapType {
	node* heap[MAX_ELEMENT];
	int count; //힙에 있는 노드의 수
}HeapType;

void insert_heap(HeapType* h, node* v); //bound가 큰 것을 우선순위로 하는 힙에 노드를 삽입하는 함수
node* delete_max_heap(HeapType* h); //힙에서 bound가 가장 큰 노드를 리턴하는 함수
void initialize(HeapType** h); //힙을 초기화하는 함수
int empty(HeapType* Q); //힙이 비어있는지 확인하는 함수

void kanpsack(); //knapsack problem을 풀기위해 상태공간트리를 구축하는 함수
float bound(node u); //bound를 계산하는 함수


int main(void) {
	kanpsack();
	printf("\n최대이익 : %d\n", maxprofit);
}


//bound가 큰 것을 우선순위로 하는 힙에 노드를 삽입하는 함수
void insert_heap(HeapType* h, node* v) {
	node* k = (node*)malloc(sizeof(node)); //노드생성

	//큐에 넣을 노드의 정보 복사
	k->level = v->level;
	k->profit = v->profit;
	k->weight = v->weight;
	k->bound = v->bound;

	int i;
	i = ++(h->count);

	//올라가면서 부모노드와 비교
	while ((i != 1) && (k->bound > h->heap[i / 2]->bound)) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = k; //노드 삽입
}


//힙에서 bound가 가장 큰 노드를 리턴하는 함수
node* delete_max_heap(HeapType* h) {
	int parent, child;
	node* item;
	node* temp;

	item = h->heap[1]; //삭제하는 노드(루트)
	temp = h->heap[(h->count)--];

	parent = 1;
	child = 2; //child는아래로, parent는 child를 따라감

	while (child <= h->count) {
		//자식노드 중 더 작은 자식노드 찾음

		if ((child < h->count) && (h->heap[child]->bound) < (h->heap[child + 1]->bound))
			child++; // child가 오른쪽 자식이 됨

		if (temp->bound >= h->heap[child]->bound)
			break;

		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}

	h->heap[parent] = temp;
	return item;
}


//힙을 초기화하는 함수
void initialize(HeapType** h) {
	(*h) = (HeapType*)malloc(sizeof(HeapType));
	(*h)->count = 0;
}


//힙이 비어있는지 확인하는 함수
int empty(HeapType* Q) {
	if (Q->count == 0)
		return 1;
	else
		return 0;
}


//knapsack problem을 풀기위해 상태공간트리를 구축하는 함수
void kanpsack() {
	HeapType* PQ; node u, v;

	initialize(&PQ);
	v.level = 0; v.profit = 0; v.weight = 0; v.bound = bound(v);
	maxprofit = 0;
	insert_heap(PQ, &v);
	printf("노드 힙에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),힙의 노드개수:%d\n", v.level, v.profit, v.weight, v.bound, PQ->count);

	while (!empty(PQ)) {
		node* v = delete_max_heap(PQ);
		printf("\n노드 힙에서 제거(level=%d,profit=%d,weight=%d,bound=%.2f),힙의 노드개수:%d\n", v->level, v->profit, v->weight, v->bound, PQ->count);

		if (v->bound > maxprofit) { //유망한지 검사
			u.level = (v->level) + 1;
			u.profit = (v->profit) + p[u.level];
			u.weight = (v->weight) + w[u.level];

			if ((u.weight <= W) && (u.profit > maxprofit)) {
				maxprofit = u.profit;
				printf("maxprofit : %d\n", maxprofit);
			}

			u.bound = bound(u);

			if (bound(u) > maxprofit) { //다음 아이템을 포함하는 노드
				insert_heap(PQ, &u);
				printf("노드 힙에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),힙의 노드개수:%d\n", u.level, u.profit, u.weight, bound(u), PQ->count);
			}

			u.weight = v->weight;
			u.profit = v->profit;
			u.bound = bound(u);

			if (bound(u) > maxprofit) { //다음 아이템을 포함하지 않는 노드
				insert_heap(PQ, &u);
				printf("노드 힙에 추가(level=%d,profit=%d,weight=%d,bound=%.2f),힙의 노드개수:%d\n", u.level, u.profit, u.weight, u.bound, PQ->count);
			}
		}
	}
}


//bound를 계산하는 함수
float bound(node u) {
	int j, k;
	int totweight;
	float result;
	if (u.weight >= W)
		return 0;

	else {
		result = (float)u.profit;
		j = u.level + 1;
		totweight = u.weight;

		while (j <= n && totweight + w[j] <= W) {
			totweight = totweight + w[j];
			result = result + p[j];
			j++;
		}

		k = j;

		if (k <= n) {
			result = result + (W - totweight) * (p[k] / w[k]);
		}
		return result;
	}
}