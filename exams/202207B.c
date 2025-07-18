// Si scriva un programma C che prende come argomenti una sequenza di nomi di file.
// Per ciascuna coppia di file, il programma crea un nuovo thread/processo il quale:
// A) se i due file possono essere letti ed hanno la stessa taglia scrive su standard output il
//		nome dei due file analizzati e l’esito del controllo.
// B) prima di terminare scrive su standard output il numero di coppie di file che hanno 
//	   superato il controllo. Nota: il controllo di coppie distinte deve essere concorrente.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

volatile int lock=0;
void acquire(){while(__sync_lock_test_and_set(&lock,1))while(lock);}
void release(){lock=0;}

char **files;

void* routine(void *param[]){
	acquire();
	int count=(int*)param[0];
	int i=(int*)param[1];
	int j=(int*)param[2];
	printf("Sono il thread %d che va a confrontare le coppie %d e %d: ", count+1, i, j);
	printf("(%s,%s)\n", files[i+1], files[j+1]);
	int fd1, fd2, size1,size2;
	fd1=open(files[i+1], O_RDONLY);
	if(fd1==-1) abort("Error while reading file1\n");
	fd2=open(files[j+1], O_RDONLY);
	if(fd2==-1) abort("Error while reading file2\n");
	size1=(int)lseek(fd1, 0, SEEK_END);
	lseek(fd1, 0, SEEK_SET);
	size2=(int)lseek(fd2, 0, SEEK_END);
	lseek(fd2, 0, SEEK_SET);
	if(size1==size2) printf("I file %s e %s hanno la stessa taglia (%d,%d)\n",
									files[i+1], files[j+1], size1, size2);
	else printf("I file %s e %s hanno differente taglia (%d,%d)\n",
					files[i+1], files[j+1], size1, size2);
	printf("--------------------\n");				
	release();
	close(fd1);
	close(fd2);
}

int main(int argc, char *argv[]){

	if(argc<3) abort("Usage: <file1> <file2> ...\n");
	int nfiles=argc-1;
	files=argv;
	
	printf("--------------------\n");
	for(int i=0; i<argc; i++) printf("argv[%d]=%s\n", i, argv[i]);
	printf("--------------------\n");
	for(int i=0; i<argc; i++) printf("files[%d]=%s\n", i, files[i]);
	printf("--------------------\n");
	
	int count=0;
	for(int i=0; i<nfiles-1; i++){
		for(int j=i+1; j<nfiles; j++) {
			printf("Coppia di files (%d,%d) = (%s,%s) per il thread %d\n", 
			i, j, files[i+1], files[j+1], count);
			count++;
		}
	}
	printf("--------------------\n");
	
	pthread_t *threads=(pthread_t*)malloc(count*sizeof(pthread_t));
	printf("Totale thread da creare: %d\n", count);
	printf("--------------------\n");
	count=0;
	
	for(int i=0; i<nfiles-1; i++){
		for(int j=i+1; j<nfiles; j++){
			int *param[3]={count,i,j};
			pthread_create(&threads[count], NULL, &routine, &param);
			sleep(1);
			count++;
		}
	}
	
	for(int i=0; i<count; i++) pthread_join(threads[i], NULL);
	free(threads);
	
	return 0;
}
