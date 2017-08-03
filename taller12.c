#include <string.h>
#include <stdio.h>
#include <pthead.h>

void * funcion_hilo1(void *arg){

	int argumento = (long)arg;			//Argumento lo recibimos como tipo void*, lo convertimos a int.

	int i = 0;
	for(i = argumento - 1; i >= 0; i--){
		sleep(1);		//duerme 1 segundo a la vez
		printf("Hilo 1: faltan %d segundos para terminar\n", i);	
	}

	return (void *)0;		//tenemos que devolver algo
}

int num_caracteres(){
	
}

int main(int argc, char **argv){

	//tmp
	int longitud = 10, cont = 4;
	//
	
	pthread_t id1, id2;		//aqui almacenamos los IDs de los hilos

	int status;

	
	//Prueba final
	long argumento_entero = 50;
	status = pthread_create(&id1, NULL, funcion_hilo1, (void *)argumento_entero);	//al hilo 1 le mandamos el argumento 50;

	if(status < 0){
		fprintf(stderr, "Error al crear el hilo 1\n");
		exit(-1);	
	}

	
	//Esperamos a que ambos hilos terminen
	
	/*int status1 = pthread_join(id1, NULL);			//Para el hilo 1, no nos importa el valor de retorno, por eso mandamos NULL
	int status2 = pthread_join(id2, &valor_retorno);	//Al inicio, valor_retorno apunta a NULL; al regresar de esta funcion, apuntara a la estructura/valor que retorne el hilo 2
	if(status1 < 0){
		fprintf(stderr, "Error al esperar por el hilo 1\n");
		exit(-1);
	}
	if(status2 < 0){
		fprintf(stderr, "Error al esperar por el hilo 2\n");
		exit(-1);
	}*/
 
		

	printf("Hilos terminaron normalmente\n");
	exit(0);
	

}