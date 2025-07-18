// Si scriva una funzione C con la seguente interfaccia:
// void file_check(char *file_name, int num_threads). Tale funzione dovrà
// lanciare num_thread nuovi threads, in modo che ciascuno di essi legga stringhe
// dallo standard input, e per ogni stringa letta verifichi l’occorrenza di tale
// stringa all’interno di ciascuna riga del file il cui path è identificato
// tramite il parametro file_name, e stampi la stringa su standard output
// in caso affermativo.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

void* filecheck(char* param){
	
	char *stringa, *buffer;
	int thread=atoi(param[0]);
	printf("Sono il thread numero %d\n", thread);
	buffer=(char*)param[1];
	printf("Inserisci la stringa da cercare: ");
	scanf("%s", stringa);
	if(stringa=="exit") exit(1);
	if(strstr(stringa, buffer)) printf("Ho trovato la stringa %s\n", stringa);
	return NULL;
}

int main(int argc, char* argv){

	char *filename;
	int nthreads;
	pthread_t* threads;
	int ifd, size_r, size;
	char *buffer;
	
	if(argc!=3) abort("Usage: check <nomefile> <threads>\n");
	else{filename=argv[1]; nthreads=atoi(argv[2]);}
	
	ifd=open(filename, O_RDONLY);
	if(ifd==-1) abort("Error while reading input\n");
	
	size=(int)lseek(ifd, 0, SEEK_END)-1;
	if(size<=0) abort("Error with the input file\n");
	buffer=(char*)malloc(size*sizeof(char));
	lseek(ifd, 0, SEEK_SET);
	
	size_r=read(ifd, buffer, size);
	if(size_r==-1) abort("Error while reading input\n");
	
	threads=(pthread_t*)malloc(nthreads*sizeof(pthread_t));
	
	for(int i=0; i<nthreads; i++){
		char *param[2];
		param[0]=buffer;
		param[1]=i;
		threads[i]=pthread_create(&threads[i], NULL, filecheck, param);
		pthread_join(threads[i], NULL);
	}
	
	free(threads);
	free(buffer);
	close(ifd);
	return 0;
}
