#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

using namespace std ;

const unsigned num_items = 40, //Numero de Items a Producir
        tam_vector = 10; //Tamaño del Vector Compartido
int vector_buffer[tam_vector], //Vector de Items
        indice = 0; //Indice del Vector
sem_t producir; //Inicializado a 1
sem_t consumir; //Inicializado a 0
sem_t mutex; //Inicializado a 1

/**
 * @brief Introduce un retraso aleatorio
 * @param smin Valor minimo del retraso
 * @param smax Valor maximo del restraso
 */
void retraso_aleatorio(const float smin, const float smax){
    static bool primera = true;
    if(primera){
        srand(time(NULL));
        primera = false;
    }
    //Segun el Compilador puede ser rand() o random()
    const float tsec = smin+(smax-smin)*((float)rand()/(float)RAND_MAX);
    usleep((useconds_t)(tsec*1000000.0));
}

/**
 * @brief Funcion que produce los items
 * @return Item producido
 */
unsigned producir_dato(){
    static int contador = 0;
    contador = contador + 1;
    retraso_aleatorio(0.1, 0.5);
    cout << "Productor: dato producido: " << contador << endl << flush;
    return contador;
}

/**
 * @brief Consume un item
 * @param dato Item que se consume
 */
void consumir_dato(int dato){
    retraso_aleatorio(0.1, 1.5);
    cout << "Consumidor: dato consumido: " << dato << endl << flush;
}

/**
 * @brief Hebra de produccion de items
 * @param 
 * @return
 */
void * funcion_productor(void *){
    for(unsigned i = 0; i < num_items; i++){
        int dato = producir_dato();
        sem_wait(&producir);
        sem_wait(&mutex);
        vector_buffer[indice] = dato;
        indice++;
	sem_post(&mutex);
        sem_post(&consumir);
        cout << "Productor: dato insertado: " << dato << endl << flush;
  }
  return NULL;
}

/**
 * @brief Hebra de consumicion de items
 * @param
 * @return
 */
void * funcion_consumidor(void *){
    for(unsigned i = 0; i < num_items; i++){
        int dato;
        sem_wait(&consumir);
        sem_wait(&mutex);
        dato = vector_buffer[indice-1];
        indice--;
        sem_post(&mutex);
        sem_post(&producir);
        cout << "Consumidor: dato extraído: " << dato << endl << flush;
        consumir_dato(dato);
    }
    return NULL;
}

int main(){
    sem_init(&producir, 0, tam_vector);
    sem_init(&consumir, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t productor, consumidor;
    pthread_create(&productor,NULL,funcion_productor,NULL);
    pthread_create(&consumidor,NULL,funcion_consumidor,NULL);
    pthread_join(productor, NULL);
    pthread_join(consumidor, NULL);
    cout << "Fin" << endl;
    sem_destroy(&producir);
    sem_destroy(&consumir);
    sem_destroy(&mutex);
    return 0;
}