/* 
 * Si scriva una funzione di codice C con la seguente interfaccia:
 * int check_palindrome(char *filename, int n).
 * Tale funzione deve lanciare un numero di thread/processi pari a n per leggere dal file
 * filename una sequenza di stringhe e contare quante di queste siano palindrome. Infine, la
 * funzione stampa su standard output e ritorna il numero totale di stringhe identificato.
 * Il file è tale per cui, ciascuna stringa è lunga 50 caratteri incluso il terminatore di linea 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>

#define SIZE_R 50
#define abort(msg) do{printf(msg); exit(1);} while(0)

volatile int lock=0;
void acquire(){while(__sync_lock_test_and_set(&lock,1))while(lock);}
void release(){lock=0;}

char* buffer;

void* routine(void* param){
}

int check_palindrome(char *filename, int n){

	int fd=open(filename, O_RDONLY);
	if(fd==0) abort("error while opening the file\n");
	
	int size_f=lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	if(size_f==0) abort("error with the file size\n");
	
	buffer=(char*)malloc(size_f*sizeof(char))
	int size_r=read(fd, buffer, size_f);
	if(size_r==-1) abort("errore lettura file\n");
	
	int n_righe=size_f/50;
	int n_cicli=n_righe/n;
	int n_rest=n_righe%n;
}

int main(int argc, char *argv[]){

	if(argc!=3) abort("usage: <nomefile> <numerothreads>\n");
	if(atoi(argv[2]<0) abort("use at least one thread\n");
	int n=atoi(argv[2]);
	int r=check_palindrome(argv[1],n);
	printf("il file contiene %d righe palindrome\n");
	
	return 0;
}
