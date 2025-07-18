#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define FILE_NAME "(6)DupLog.txt"
#define STDOUT 1
#define abort(msg) do{printf(msg); exit(1);} while(0)

// programma che utilizza le primitive POSIX per ridirezionare lo standard
// output di un eseguibile (in questo caso un comando) su un file regolare
// 1) crea e apre il file destinazione ottenendone il file descriptor ofd
// 2) chiude lo stream di output rendendo disponibile il file descriptor
// 3) invoca dup passando ofd come parametro e sostituisce il programma
//    del processo corrente tramite exec();
// 4) il comando ls stampa su standard output (file descriptor 1) il quale
//    è stato direzionato su "log.txt" piuttosto che su terminale
int main(){

	int ofd;
	
	// int open(char *path, int flags, mode_t mode);
	// con i flag a create o truncate è necessaria la mode
	// permessi owner|group|other -> 0660 = rw-|rw-|---
	ofd=open(FILE_NAME, O_WRONLY|O_CREAT|O_TRUNC, 0660);
	if(ofd==-1) abort("output creation error\n");
	
	close(STDOUT); // chiudo lo standard output
	
	// int dup(int oldd); duplicates an existing object descriptor (oldd)
	// pointer!=descriptor: an integer used to identify an opened file at
	// kernel level in Linux and Unix-like systems
	ofd=dup(ofd);
	if(ofd==-1) abort("duplication failed\n");
	
	// execlp(char *file, char *arg, ...); replaces the current process
	// image with a new one specified by file (used to construct a pathname
	// that identifies the new process image file). args are the arguments 
	// available to the new process image (list must end with NULL)
	execlp("ls", "ls", NULL);
	
}
