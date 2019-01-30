#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

const int n = 3;
sem_t estanquero;
sem_t fumador[n];

void retraso_aleatorio(const float smin, const float smax){
  	static bool primera = true;
  	if (primera){
		srand(time(NULL));
     	primera = false;
  	}
  	const float tsec = smin+(smax-smin)*((float)random()/(float)RAND_MAX);
  	usleep((useconds_t)(tsec*1000000.0));
}

void fumar(int num_fumador){
   	cout << "Fumador número " << num_fumador << ": comienza a fumar." << endl << flush;
   	retraso_aleatorio(0.2, 0.8);
   	cout << "Fumador número " << num_fumador << ": termina de fumar." << endl << flush;
}

void * funcion_estanquero(void *){
	while(true){
		sem_wait(&estanquero);
		int n_hebra = random()%3;
		sem_post(&(fumador[n_hebra]));
	}
}

void * funcion_fumador(void * num_fum_void){
	unsigned long n_hebra = (unsigned long) num_fum_void;
	while(true){
		sem_wait(&(fumador[n_hebra]));
		sem_post(&estanquero);
		fumar(n_hebra);
	}
}

int main(){
  	srand(time(NULL));

	sem_init(&estanquero, 0, 1);
	for(int i = 0; i < n; i++){
		sem_init(&fumador[i], 0, 0);
	}

	pthread_t fumadores[n], vendedor;
	pthread_create(&vendedor, NULL, funcion_estanquero, NULL);
   	for(unsigned int i = 0; i < n; i++){
        	pthread_create(&(fumadores[i]), NULL, funcion_fumador, (void *) i);
	}

	pthread_join(vendedor, NULL);
	for(unsigned int i = 0; i < n; i++){
		pthread_join(fumadores[i], NULL);
	}

	/*sem_destroy(&estanquero);
	for(int i = 0; i < n; i++){
		sem_destroy(&fumador[i]);
	}*/

  	return 0;
}
