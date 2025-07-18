#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* interfaccia testuale per il sistema operativo attraverso la quale è possibile eseguire programmi e comandi. Il processo principale crea un processo child tramite fork(), il child invoca una funzione di tipo exec() e il processo principale attende la sua terminazione tramite wait() */
int main(){
	char comando[256];
	pid_t pid;
	int status, result;
	while(1){
		printf("Digitare un comando: ");
		result=scanf("%s", comando);
		if(result==EOF) continue;
		pid=fork();
		if(pid==-1){
			printf("Errore nella fork\n");
			exit(1);
		}
		if(pid==0) execlp(comando, comando, NULL);
		else wait(&status);
	}
	return 0;
}
