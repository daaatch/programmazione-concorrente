// Si scriva un programma C che prende come argomenti una sequenza di nomi di file.
// Per ciascuna coppia di file, il programma crea un nuovo thread/processo il quale:
// A) verifica se i due possono essere letti e se i rispettivi byte in posizione 0 
// 	sono uguali tra loro;
// B) scrive su standard output il nome dei due file analizzati e l’esito del 
// 	controllo.
// C) Il programma prima di terminare scrive su standard output il numero di coppie 
// 	di file che hanno superato il controllo. 
// Nota: il controllo di coppie distinte deve essere concorrente. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

char** filenames;

volatile int lock=0;

void acquire(){while(__sync_lock_test_and_set(&lock,1))while(lock);}
void release(){lock=0;}

void* routine(void* param[]){
	acquire();
	int indice=(int*)param[0];
	int i=(int*)param[1];
	int j=(int*)param[2];
	printf("thread in posizione %d, coppia di file %d, %d, ", indice, i, j);
	int fd1,fd2;
	fd1=open(filenames[i+1], O_RDONLY);
	fd2=open(filenames[j+1], O_RDONLY);
	if(fd1==-1) abort("Error while opening the first file\n");
	if(fd2==-1) abort("Error While opening the second file\n");
	
	char* c1=(char*)malloc(sizeof(char*));
	char* c2=(char*)malloc(sizeof(char*));
	
	read(fd1,c1,1);
	read(fd2,c2,1);
	
	printf("primi caratteri %s, %s\n", c1, c2);
	
	close(ifd);
	close(ofd);
	free(c1);
	free(c2);
	
	release();
}

int main(int argc, char *argv[]){

	if(argc<2) abort("Usage: <filelist>\n");
	
	int nfiles = argc-1;
	filenames=argv;
	
	printf("--------------------\n");
	for(int i=0; i<argc; i++) printf("Argv[%d]: %s\n",i, argv[i]);	
	printf("--------------------\n");	
	for(int i=0; i<argc; i++) printf("Filenames[%d]: %s\n",i, (char*)filenames[i]);
	printf("--------------------\n");
	
	int totthreads=nfiles*nfiles;
	
	pthread_t* threads=(pthread_t*)malloc(totthreads*sizeof(pthread_t));
	
	int count=0;
	
	for(int i=0; i<nfiles-1; i++){
		for(int j=i+1; j<nfiles; j++){
			//printf("thread in posizione %d, coppia di file %d,%d\n", count, i, j);
			int *coppia[3]={count,i,j};
			pthread_create(&threads[count], NULL, &routine, &coppia);
			sleep(2);
			count++;
		}
	}
	
	printf("--------------------\n");
	
	
	
	for(int i=0; i<totthreads; i++) {
		pthread_join(threads[i],NULL);
	}
	
	free(threads);

	return 0;
}
