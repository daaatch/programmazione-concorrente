#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define abort(msg) do{printf(msg); exit(1);} while(0);
#define length 50

volatile int lock=0;
void acquire(){while(__sync_lock_test_and_set(&lock,1))while(lock);}
void release(){lock=0;}

char carattere;
int nfiles;
int ofd;

void* routine(void* param){
	
	int ifd=*(int*)param; //file descriptor i-esimo
	
	/*calcolo file size prima di iniziare*/
	int size_f=lseek(ifd, 0, SEEK_END);
	lseek(ifd, 0, SEEK_SET);	
	
	/*creo un buffer abbastanza grande*/
	char *buffer=(char*)malloc(size_f*sizeof(char));
	
	/*lo riempio con ifd*/
	int size_r=read(ifd, buffer, size_f);
	if(size_r==-1) abort("Errore durante il caricamento del buffer\n");
	
	/*suddivisione in righe*/
	int righe=size_f/length;
	for(int i=0; i<righe; i++){
		char *riga=(char*)malloc(length*sizeof(char));
		for(int j=0; j<49; j++){
			riga[j]=buffer[(length*i)+j];
			if(i!=righe-1) riga[49]='\0';
		}
		/*se la riga inizia per char la scrivo nel buffer ofd*/
		if(riga[0]==carattere){
			acquire();
			int size_w=write(ofd, riga, length);
			if(size_w==0) abort("Errore scrittura riga\n");
			release();
		}
		free(riga);
	}
	
	/*dealloco i buffer e chiudo i file descriptor*/
	close(ifd);
	free(buffer);
}

int filter_and_tunnel(int descriptors[], int count, char c, int fd_log){

	/*apro un thread per ogni descriptor*/
	pthread_t* threads=(pthread_t*)malloc(count*sizeof(pthread_t));
	for(int i=0; i<count; i++){
		pthread_create(&threads[i], NULL, &routine, &descriptors[i]);
	}
	for(int i=0; i<count; i++) pthread_join(threads[i], NULL);
	
	return 0;
}

int main(int argc, char *argv[]){
	
	if(argc<=1) abort("Usage: <carattere> <fd_log> <file_list>\n");
	
	carattere=argv[1][0]; //il primissimo elemento della stringa carattere ('c\0')
	nfiles=argc-3; //tutti gli argomenti tranne argv[0], char e fd_log
	
	/*creo l'array contenente i file descriptors*/
	int* descriptors=(int*)malloc(nfiles*sizeof(int));
	for(int i=0; i<nfiles; i++){
		descriptors[0]=open(argv[i+3], O_RDONLY);
		if(descriptors[0]==-1) abort("Errore del file descriptor(s)\n");
	}
	
	/*apro l'output file descriptor*/
	ofd=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0660);
	if(ofd==-1) abort("Errore dell'ofd\n");
	
	/*lancio la procedura*/
	filter_and_tunnel(descriptors, nfiles, carattere, ofd);
	
	return 0;
}



