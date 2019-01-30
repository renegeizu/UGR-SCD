#include <iostream>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define Productor 0
#define Buffer 5
#define Consumidor 1
#define Mensaje 3
#define ITERS 20
#define TAM 5

using namespace std;

// ---------------------------------------------------------------------

void productor(int rank){
	int value;
	for(unsigned int i = 0; i < ITERS/5; i++){ 
		value = (i + (rank * 10));
		cout << "\033[33m Productor " << rank << " produce valor " << value << " \033[37m" << endl << flush;
		usleep(1000U * (100U + (rand() % 900U)));
		MPI_Ssend(&value, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD);
	}
}

// ---------------------------------------------------------------------

void buffer(){
	int value[TAM], peticion, pos  =  0, rama;
	MPI_Status status;
	for(unsigned int i = 0; i < (ITERS * 2); i++){  
		if(pos == 0)
			rama = 0;
		else if(pos == TAM)
			rama = 1;
		else{
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if(status.MPI_TAG == Productor) 
				rama = 0;
			else
				rama = 1;
		}
		switch(rama){
			case 0:
				MPI_Recv(&value[pos], 1, MPI_INT, MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status);
				cout << "\033[34m Buffer recibe " << value[pos] << " de Productor " << status.MPI_SOURCE << " \033[37m" << endl << flush;  
				pos++;
				break;
			case 1:
				MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, Consumidor, MPI_COMM_WORLD, &status);
				MPI_Ssend(&value[pos-1], 1, MPI_INT, status.MPI_SOURCE, Mensaje, MPI_COMM_WORLD);
				cout << "\033[32m Buffer envía " << value[pos-1] << " a Consumidor " << status.MPI_SOURCE << " \033[37m" << endl << flush;
				pos--;
				break;
		}
	}
}

// ---------------------------------------------------------------------

void consumidor(int rank){
	int value, peticion = 1;
	float raiz; 
	MPI_Status status;
	for(unsigned int i = 0; i < ITERS/4; i++){
		MPI_Ssend(&peticion, 1, MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD);
		MPI_Recv (&value, 1, MPI_INT, Buffer, Mensaje, MPI_COMM_WORLD, &status);
		cout << "\033[35m Consumidor " << rank << " recibe valor " << value << " de Buffer \033[37m" << endl << flush;
		usleep(1000U * (100U + (rand() % 900U)));
		raiz = sqrt(value);
	}
}

// ---------------------------------------------------------------------

int main(int argc, char *argv[]){
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL));
	if (size != 10){
		cout<< "\033[31m El numero de procesos debe ser 10 \033[37m" << endl;
		return 0;
	}
	if (rank > Buffer)
		consumidor(rank);
        else if (rank == Buffer)
		buffer();
        else
		productor(rank);
	MPI_Finalize();
	return 0;
}
