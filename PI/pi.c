/*
 * Nazwa: PI
 * Autor: Maciej Rak
 * Opis: Program pozwala przybliżyć liczbę PI korzystając z metody Monte Carlo
 * 	 korzystając z wielu wątków.
 * Uwaga: Ze względu na niedoskonałość generatora liczb losowych z biblioteki standardowej,
 * 	  dokładność przybliżenia liczby jest niska.
 * /


#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define num 30

int counter;

typedef struct{
	long long int* all;
	long long int* inside;
}PIcalcArgs;

int threadCounter = 0;

void* PIcalc(void* argv){
	long long x,y;
	threadCounter++;
	PIcalcArgs* arg = (PIcalcArgs*)argv;
	while(1){
		x = rand()%100001;
		y = rand()%100001;		
		if (x*x+y*y <= 10000000000) (*(arg->inside))++;
		(*(arg->all))++;
	}
};


int main(){
	long long int all, inside;
	srand(time(NULL));
	all = 1;
	inside = 1;
	PIcalcArgs* args = (PIcalcArgs*)malloc(sizeof(PIcalcArgs));
	args->all = &all;
	args->inside = &inside;
	pthread_t tid[num];
	for(int i = 0; i<num; i++) pthread_create(&tid[i],NULL,&PIcalc,(void*)args);
	while(1){
		printf("PI=%e Number of threads: %d\n",4*(double)((double)inside/(double)all),threadCounter);
	} 
}  
