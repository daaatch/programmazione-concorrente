// Dato un file binario contenente un sequenza di 2^15 interi di tipo short
// scrivere un programma che crea N processi o threads, i quali leggono il
// contenuto del file ed individuano il valore minimo e massimo contenuto nel
// file. Nel fornire una soluzione rispettare i seguenti vincoli:
// A) ciascun intero non può essere letto da più di un thread/processo;
// B) ciascun thread/processo può leggere il medesimo intero al più una volta;
// C) ciascun thread/processo può allocare memoria nell’heap per al più 512 byte;
// D) N è un parametro definito a tempo di compilazione o tramite command line;
// E) N è minore o uguale a 8;
// F) è ammesso allocare di variabili globali (data) e locali (stack) per
//    memorizzare tipi primitivi (puntatori, int, short, char, long, etc.)
//    per al più 128 byte.
// G) Per generare il file è possibile utilizzare la soluzione dell’esercizio 1.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define BUFSIZE 64
#define abort(msg) do{printf(msg); exit(1);} while(0)

short buffer[BUFSIZE];
short min, max;

void* calc(void* args){
	int start=(int*)args[0];
	int end=(int*)args[1];
	printf("slice from %d to %d\n", start, end);
	for(int i=start; i<end; i++){
		printf("slice[%d]: %d\n", i, buffer[i]);
		if(buffer[i]<min) min=buffer[i];
		if(buffer[i]>max) max=buffer[i];
	}
	printf("----------------------------------------------\n");
}

int main(int argc, char *argv[]){
	
	int nthreads, slice;
	int ifd, size_r;
	int result;
	pthread_t *threads;
	
	if(argc!=3) abort("insert <threads> <filename>\n");
	else{ min=0; max=0;}
	
	// CHECK E LETTURA INTERI
	ifd=open(argv[2], O_RDONLY);
	if(ifd==-1) abort("input opening error\n");	
	size_r=read(ifd, buffer, BUFSIZE);
	if(size_r==-1) abort("error while reading\n");
	if(size_r!=BUFSIZE) abort("error while reading\n");	
	for(int i=0; i<BUFSIZE; i++) printf("buffer[%d]: %d\n", i, buffer[i]);
	printf("----------------------------------------------\n");
	
	// CHECK E CREAZIONE THREAD(S)
	nthreads=atoi(argv[1]);
	if(nthreads<=0||nthreads>8) abort("wrong number of threads (max. 8)\n");
	else{
		slice=BUFSIZE/nthreads;
		printf("numero threads: %d, dimensione slice: %d\n", nthreads, slice);
		printf("----------------------------------------------\n");
		threads=(pthread_t*)malloc(nthreads*sizeof(pthread_t));
		for(int i=0; i<nthreads; i++){
			int args[2];
			args[0]=i*slice;
			args[1]=(1+i)*slice;
			if(args[1]>BUFSIZE) args[1]=BUFSIZE;
			result=pthread_create(&threads[i], NULL, calc, args);
			if(result!=0) abort("I cannot create a thread\n");
			else{
				printf("creato thread %d che legge da %d a %d\n",i+1,args[0],args[1]);
				printf("----------------------------------------------\n");
				pthread_join(threads[i], NULL);
			}
		}
	}
	
	//TODO: trovare  un modo per non perdersi record qunado nthreads è dispari
	
	printf("minimo: %d, massimo: %d\n", min, max);
	close(ifd);
	return 0;
	
}

