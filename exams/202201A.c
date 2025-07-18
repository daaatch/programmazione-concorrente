// Si scriva una funzione di codice C con la seguente interfaccia:
// int above_threshold(char *filename, int n, int threshold).
// Tale funzione deve lanciare un numero di thread/processi pari a n per leggere
// dal file binario filename una sequenza di int e contare quanti di questi hanno
// un valore maggiore di threshold. Infine, la funzione stampa su standard output e
// ritorna il numero totale di int con valore maggiore di threshold.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

char *buffer;
int result;

void* function(void* param[]){
	int start=(int*)param[0];
	int end=(int*)param[1];
	int threshold=(int*)param[2];
	for(int i=start; i<end; i++) if(atoi(buffer[i])>threshold) result++;
}

int above_threshold(char *filename, int n, int threshold){
	
	int fd, size_r, size_f;
	int slice, rest;
	pthread_t *threads;
	result=0;
	
	/* LETTURA INPUT */
	
	// apro il file descriptor
	fd=open(filename, O_RDONLY);
	if(fd==-1) abort("Error while opening the file\n");
	
	// controllo le dimensioni del file
	size_f=(int)lseek(fd, 0, SEEK_END)-1;
	lseek(fd, 0, SEEK_SET);
	if(size_f<=0) abort("The input file seems to be empty\n");
	
	// leggo l'input e riempio il buffer
	buffer=(char*)malloc(size_f*sizeof(char));
	size_r=read(fd, buffer, size_f);
	if(size_r==-1) abort("Error while reading\n");
	printf("file size: %d\n", size_f);
	
	printf("--------------------\n");
	printf("Current buffer:\n");
	for(int i=0; i<size_f; i++){
		printf("Buffer[%d]:%s\n", i, buffer[i]);
		printf("basta"); exit(0);
	}
	printf("--------------------\n");
	
	
	/* GESTIONE THREADS */
	
	threads=(pthread_t*)malloc(n*sizeof(pthread_t));
	slice=size_f/n;
	rest=size_f%n;
	
	for(int i=0; i<n; i++){
		int *param[3];
		int myslice=slice;
		if(i==n-1) myslice+=rest;
		param[0]=slice*i;
		param[1]=param[0]+myslice;
		param[2]=threshold;
		pthread_create(&threads[i], NULL, function, param);
	}
	
	for(int i=0; i<n; i++) pthread_join(threads[i], NULL);
	
	close(fd);
	free(buffer);
	free(threads);
	return result;
}

int main(int argc, char *argv[]){
	if(argc!=4) abort("Usage: <filename> <threads> <threshold>\n");
	result=above_threshold(argv[1], atoi(argv[2]), atoi(argv[3]));
	printf("Result: %d\n", result);
	return 0;
}
