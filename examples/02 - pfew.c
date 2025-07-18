#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


void* child_func(void *par){
  *((int*)par) = 1;
  sleep(5);
  printf("I'm the child!\n");
  pthread_exit(par);
}

/* programma che emula FEW ma usando thread al posto dei processi     *
 * dapprima crea un thread tramite pthread_create (1), poi attende    *
 * la terminazione del thread child tramite pthread_join (2) il quale *
 * termina con una pthread_exit (3)                                   */
int main(){

    pthread_t ctid; // current thread id
    int result, *status_ptr, status_val;
    status_ptr = &status_val;
    
    /* la funzione pthread_create prende come terzo parametro la  *
     * funzione che il thread child dovrà eseguire e come quarto  *
     * parametro il parametro necessario all'esecuzione della     *
     * funzione corrispondente al parametro 3. Il primo parametro *
     * corrisponde all'area di memoria del thread id del padre e  *
     * il secondo serve solo se vuoi configurare stack e schedule */
    printf("I'm a thread. I'm going to create another thread\n");
    result=pthread_create(&ctid, NULL, child_func, &status_val);
    
    /* pthread_create restituisce 0 se la creazione va a buon fine */
    if(result!=0) printf("I cannot create a child");

	 /* pthread_join(pthread_t thread, void **retval) aspetta che il  *
	  * thread specificato in "thread" termini e se retval non è NULL *
	  * allora copia l'exit status del thread specificato nella cella *
	  * a cui punta retval. In caso di successo ritorna il valore 0   */
    else{
        printf("I'm now a parent thread.\n"
					"I'll wait for my child %lu thread to die...\n", ctid);
        pthread_join(ctid, (void**)&status_ptr);
        printf("My child has invoked %d\n", *status_ptr);
    }

    printf("My child is dead, so it's my time to die\n");
    return 0;

}


