#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

#define MAX 1000000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int * num_palabras;
//Declaramos una estructura
typedef struct mi_estructuraTDA{
	int conteo;
	char *str;
} estructura;

typedef struct IntervalosTDA {
	int id;
   	int inicio;
   	int final;
   	int n_palabras;
   	char *sub_texto;
   	char **palabras;
} Intervalos;

int numero_lineas(char *ruta, int *tam_lineas){
	if(ruta != NULL){
		FILE *ar = fopen(ruta,"r");
		int lineas = 0;
		int tam_linea = 0;
		while(!feof(ar)){
			tam_linea++;
			char c = getc(ar);
			if(c == '\n'){
				if (tam_lineas != NULL){
					tam_lineas[lineas] = tam_linea;
				}
				lineas++;
				tam_linea = 0;
			}
		}
		fclose(ar);
		return lineas;
	}
	return -1;
}

//Funcion para el hilo 1
void * funcion_hilo(void *arg){

	Intervalos *intervalo = (Intervalos *)arg;			//Argumento lo recibimos como tipo void*, lo convertimos a int.

	pthread_mutex_lock(&mutex);

	char *palabra;
	palabra = strtok(intervalo->sub_texto," \"!·$&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");
	char **search = intervalo->palabras;
	int n = intervalo->n_palabras;

	while( palabra != NULL ){
		for (int i = 0; i < n; i++){
			if(strcmp(search[i], palabra) == 0){
	    		num_palabras[i]++;
	    		printf("Hilo: %d - Palabra: %s - Veces: %d\n", intervalo->id, palabra, num_palabras[i]);
	    	}
    	}
       	palabra = strtok(NULL, " \"!·$&/()=º|@#~½¬<>-_'ç`+*[]{}ḉç¿,.!?:;");
	
	}
	pthread_mutex_unlock(&mutex);
	return (void *)0;		//tenemos que devolver algo
}


void hilo(int num_lineas, int cont, int num_hilo, char **palabras, int *tam_lineas, pthread_t *thread_id, char *ruta, int n_palabras){
	int ini = 0, fin = 0;
	int ini_char = 0, fin_char = 0;
	int status;

	ini = num_hilo * cont;
	fin = (num_hilo * cont) + (cont - 1);
	if ((num_lineas - fin) < 0){
		fin = num_lineas - 1;
	}

	for (int i = 0; i <= fin; i++){
		if (i == ini){
			ini_char = fin_char + 1;
		}
		fin_char = fin_char + tam_lineas[i];
	}

	//guardar texto
	if(ruta != NULL){
		char *sub_texto = (char *) malloc((fin_char - ini_char + 1) * sizeof(*sub_texto));
		FILE *fp = fopen(ruta,"r");
		fseek(fp, ini_char, SEEK_SET);
		for(int i = ini_char; i <= fin_char; i++){
			char c = getc(fp);
			if(c != '\n'){
				sub_texto[i - ini_char] = c;
			} else{
				sub_texto[i - ini_char] = (char)' ';
			}
		}
		Intervalos *intervalo = malloc(sizeof(Intervalos));
		intervalo->id = num_hilo;
		intervalo->inicio = ini_char;
		intervalo->final = fin_char;
		intervalo->palabras = palabras;
		intervalo->sub_texto = sub_texto;
		intervalo->n_palabras = n_palabras;
		status = pthread_create(&thread_id[num_hilo], NULL, funcion_hilo, (void *)intervalo);	//al hilo 1 le mandamos el argumento 50;
		//sleep(1);
		if(status < 0){
			fprintf(stderr, "Error al crear el hilo 1\n");
			exit(-1);	
		}
		fclose(fp);
	}

	//printf("inicio: %d - fin: %d\n", ini, fin);
	
}

int main(int argc, char **argv){

	int cont;
	int num_hilos = atoi(argv[2]);
	char *ruta = argv[1];
	pthread_t *thread_id = (pthread_t *) malloc(num_hilos * sizeof(*thread_id));
	//printf("%d\n", argc);
	char **palabras = (char **) malloc((argc - 3) * sizeof(*palabras));
	int *tam_lineas = (int *) malloc(MAX * sizeof(*tam_lineas));
	int *char_hilo = (int *) malloc(num_hilos * sizeof(*char_hilo));
	num_palabras = (int *) malloc(num_hilos * sizeof(*num_palabras));

	//Calcular el contador
	int num_lineas = numero_lineas(ruta, tam_lineas);
	float div = 0;
	int div_entero;
	div = (float)num_lineas / num_hilos;
	div_entero = num_lineas / num_hilos;
	printf("%d / %d = %f\n", num_lineas, num_hilos, div);
	printf("%d / %d = %d\n", num_lineas, num_hilos, div_entero);

	if (div > div_entero) cont = div_entero + 1;
	else cont = div_entero;
	printf("Entonces cada hilo tendrá: %d elementos.\n", cont);
	//Fin de calcular cont

	//Llenar palabras
	for (int i = 0; i < (argc - 3); i++){
		palabras[i] = argv[3 + i];
		printf("%d: %s\n", i, palabras[i]);
	}
	//Fin de llenar palabras
	sleep(1);
	
	for (int i = 0; i < num_hilos; ++i){
		hilo(num_lineas, cont, i, palabras, tam_lineas, thread_id, ruta, (argc - 3));
	}
	int status2;
	for (int i = 0; i < num_hilos; ++i){
		status2 = pthread_join(thread_id[i],NULL);
		if (status2 < 0){
			fprintf(stderr, "Error al esperar por el hilo %d\n", i);
			exit(-1);
		}
	}
	
	sleep(1);
	printf("Hilos terminaron normalmente\n");
	exit(0);
	

}
