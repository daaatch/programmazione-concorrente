// Scrivere un programma C invert che dato un file A ne inverte il contenuto
// e lo memorizza in nuovo file B. Il programma deve:
// A) riportare il contenuto di A in memoria;
// B) invertire la posizione di ciascun byte usando N thread/processi concorrenti;
// C) scrivere il risultato in un nuovo file B.
// D) A, B e N sono parametri che il programma deve acquisire da linea di comando.

// The overall approach is: KEEP IT SIMPLE!
// Mi metto A in memoria e non lo modifico, uso un altro buffer (per evitare sync)
// Scrivo su B usando il buffer nuovo già invertito

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

char *ibuff, *obuff;
int nthreads, length;
int slice, rest;

void* function(void* param[]){

	int index=(int*)param[0]; // sono il thread #index
	int nthreads=(int*)param[1]; // in tutto siamo nthreads
	int myslice=slice;
	if(index==nthreads-1) myslice+=rest; // se sono l'ultimo thread mi accollo rest
	int start=slice*index;
	int end=start+myslice;
	for(int i=start; i<end; i++){
		obuff[length-1-i]=ibuff[i];
	}
	return NULL;
}

void invert(int nthreads){
	
	pthread_t *threads=(pthread_t*)malloc(nthreads*sizeof(pthread_t));
	slice=length/nthreads;
	rest= length%nthreads;	
	for(int i=0; i<nthreads; i++){
		int* param[2];
		param[0]=i;
		param[1]=nthreads;
		pthread_create(&threads[i], NULL, function, param);
		pthread_join(threads[i], NULL);
	}		
	free(threads);

}

int main(int argc, char *argv[]){

	int ifd,ofd;
	int size_r, size_w;
	
	if(argc!=4) abort("Usage: invert <input> <output> <threads>\n");	
	int nthreads=atoi(argv[3]);
	if(nthreads<=0) abort("Must use at least 1 thread\n");
	
	ifd=open(argv[1], O_RDONLY);
	if(ifd==-1) abort("Error while reading input\n");
	
	ofd=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0660);
	if(ofd==-1) abort("Error while creating output\n");
	
	// ottengo size muovendo un puntatore a EOF
	length=(int)lseek(ifd, 0, SEEK_END)-1; 
	if(nthreads>length) abort("Too many threads for this file\n");
	
	if(length==-1) abort("Error while reading input\n");
	if(length==0) abort("Input file must not be empty\n"); 	
	lseek(ifd, 0, SEEK_SET); // rimetto il puntatore a inizio file
	
	ibuff=(char*)malloc(length*sizeof(char));
	obuff=(char*)malloc(length*sizeof(char));
	size_r=read(ifd, ibuff, length);
	if(size_r==-1) abort("Error while reading\n");
	
	invert(nthreads);
	
	size_w=write(ofd, obuff, length);
	if(size_w==-1) abort("Error while writing\n");
	
	close(ifd);
	close(ofd);
	free(ibuff);
	free(obuff);
	return 0;
	
}


