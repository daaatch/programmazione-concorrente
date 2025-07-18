// Si scriva il codice di una funzione C con la seguente interfaccia:
// void tunnel(int descriptors[], int count) tale che, se eseguita, porti
// l’applicazione a gestire, per ogni file-descriptor dell’array descriptors
// l’inoltro del flusso dei dati in ingresso verso il canale di standard-output
// dell’applicazione. Il parametro count indica di quanti elementi è costituito
// l’array descriptors. L’inoltro dovrà essere attuato in modo concorrente
// per i diversi canali.
