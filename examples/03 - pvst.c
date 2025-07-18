#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

#define INIT_VALUE 0
#define PARENT_VAL 1
#define TCHILD_VAL 2
#define PCHILD_VAL 3

volatile int global_var = INIT_VALUE;

void* child_func(void *par){
	*((int*)par)=1;
	global_var=TCHILD_VAL;
	printf("I'm the child and I wrote the global var: %d\n", global_var);
	pthread_exit(par);
}

/* programma con lo scopo di evidenziare le differenze basilari tra processi e thread */
int main(){
	pthread_t ctid;
	int result, *status;
	/* prima parte: il main thread crea un thread child */
	printf("I'm a thread, I'm going to create another thread\n");
	result=pthread_create(&ctid, NULL, child_func, status);
	global_var=PARENT_VAL;
	if(result!=0) printf("I cannot create a child\n");
	else{
		printf("Now I'm a parent thread.\n");
		printf("I'll wait for my child thread %lu to die...\n", ctid);
		pthread_join(ctid, (void*)&status);
		printf("My child has invoked %d\n", *status);
	}
	printf("My child is dead, so it's my time to die.\n");
	printf("Global var: %d\n", global_var);
	/* seconda parte: il thread main crea un processo child */
	global_var=PARENT_VAL;
	result=fork();
	if(result==-1) exit(1);
	if(result==0){
		global_var=PCHILD_VAL;
		printf("Child global var: %d\n", global_var);
		exit(0);
	}
	wait(&result);
	printf("Parent global var: %d\n", global_var);
	return 0;
}
