// Nei sistemi operativi UNIX, /dev/urandom è un dispositivo a caratteri
// (char device) virtuale in grado di generare numeri casuali. Nello specifico, 
// l’operazione di lettura dal relativo file produce byte casuali. 
// Scrivere un programma C che genera un file con contenuto randomico:
// A) prende come parametri da linea di comando: un numero N e una stringa S da
//    usare come nome del file da creare;
// B) crea un file S contenente N byte randomici;
// C) utilizza il dispositivo /dev/random come sorgente di numeri pseudo-casuali.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/random.h>

#define abort(msg) do{printf(msg); exit(1);} while(0)

int main(int argc, char *argv[]){

	int ifd, ofd, size_r, size_w, size_b;
	int *buffer;

	if(argc!=3) abort("usage: random <bytes> <filename>\n");
	
	size_b=atoi(argv[1]);
	if(size_b<=0) abort("input bytes error\n");
	else buffer=(int*)malloc(size_b*sizeof(int));
	
	ifd=open("/dev/urandom", O_RDONLY);
	if(ifd==-1) abort("input reading error\n");
	
	ofd=open(argv[2], O_WRONLY|O_CREAT|O_TRUNC, 0660);
	if(ofd==-1) abort("output creation error\n");
	
	size_r=read(ifd, buffer, size_b);
	if(size_r==-1) abort("error while reading\n");
	printf("read %d random bytes from /dev/urandom\n", size_r);
	
	size_w=write(ofd, buffer, size_r);
	if(size_w==-1) abort("error while writing\n");
	printf("wrote %d random bytes on %s\n", size_w, argv[2]);
	
	close(ifd);
	close(ofd);
	free(buffer);
	
	return 0;

}


