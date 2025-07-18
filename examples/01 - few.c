#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* programma che crea un processo tramite fork  (1) *
 * attende tramite wait che il processo termini (2) *
 * e termina il processo corrente tramite exit  (3) */
int main(){

	int result, status;
	
	/* il main thread fa una stampa e invoca la fork */
	printf("I'm a process and I'm going to create a child\n");
	result=fork();
	
	/* se la fork fallisce il processo principale termina *
	 * altrimenti entrambi i processi vengono eseguiti a  *
	 * partire dalla terminazione della fork              */
	if(result<0) printf("I cannot create a child\n");

	/* la fork restituisce result=0 al processo child *
	 * e result=id del child al processo parent		  */
   else if(result==0){
		     sleep(5);
		     printf("I'm the child!\n");
		     exit(1);
		 	}

	/* il parent attente la terminazione di un generico thread child *
	 * tramite la syscall wait */
   else{
		printf("I'll wait for my child to die\n");
		wait(&status);
		printf("My child has invoked exit(%d)\n", WEXITSTATUS(status));
		printf("My child is dead, so it's my time to die\n");
   }

    exit(0);

}
