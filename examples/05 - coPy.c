#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 250
#define abort(msg) do{printf(msg); exit(1);} while(0)

// programma per la gestione di file che prende da riga di comando un
// input file e lo copia in un file destinazione specificato
int main(int argc, char *argv[]){

	int ifd, ofd, size_r, size_w, end=0;
	char buffer[BUFSIZE];
	
	// argcount perchè devono esserci 3 parametri
	if(argc!=3) abort("usage: copy <source> <target>\n");
	
	// int open(const char *path, int flags);
	// int flags = O_RDONLY|O_WRONLY|O_RDWR|...
	ifd=open(argv[1], O_RDONLY);
	if(ifd==-1) abort("input opening error\n");
	
	// int open(const char *path, int flags, mode_t mode);
	// se tra le flags ho create o truncate devo specificare mode
	// mode = owner-group-other con 0660 = rw-rw---- e 
	// 0000 = ---------, 0666 = rw-rw-rw-, 0777 = rwxrwxrwx, ...
	ofd=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0660);
	if(ofd==-1) abort("output creation error\n");
	
	while(!end){
		
		// ssize_t read(int fd, void *buffer, size_t count);
		// legge count byte dal file descriptor fd e li mette nel buffer
		// la lettura comincia da un file offset incrementato ogni volta
		size_r=read(ifd, buffer, BUFSIZE);
		if(size_r==-1) abort("error while reading\n");
		
		// size_r è il numero di bytes letti (se 0 -> EOF)
		if(size_r==0) end=1;
		
		// ssize_t write(int fd, void *buffer, size_t count);
		size_w=write(ofd, buffer, size_r);
		if(size_w==-1) abort("error while writing\n");
		printf("%d byte(s) written\n", size_w);
	}
	
	// close(int fd); de-referenzia il puntatore al file descriptor
	close(ifd);
	close(ofd);
	return 0;
	
} 
