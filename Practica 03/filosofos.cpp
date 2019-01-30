#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define coger 1
#define soltar 0

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor(int id, int nprocesos);

// ---------------------------------------------------------------------

void Filosofo(int id, int nprocesos){
	/*
	 * Solucion propuesta en clase:
	 * El primer filosofo coge el tenedor de la derecha para evitar
	 * interbloqueo
	 */
	int izq = (id + 1) % nprocesos;
	int der = ((id + nprocesos) - 1) % nprocesos;
	while(true){
		if (id != 0){
			cout << "\033[33m Filosofo " << id << " solicita tenedor izq ..." << izq << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izq, coger, MPI_COMM_WORLD);
			cout << "\033[33m Filosofo " << id << " coge tenedor der ..." << der << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der, coger, MPI_COMM_WORLD);
		}else{
			cout << "\033[33m Filosofo " << id << " coge tenedor der ..." << der << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der, coger, MPI_COMM_WORLD);
			cout << "\033[33m Filosofo " << id << " solicita tenedor izq ..." << izq << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izq, coger, MPI_COMM_WORLD);
		}
		cout << "\033[34m Filosofo " << id << " COMIENDO \033[37m" << endl << flush;
		sleep((rand() % 3) + 1);
		cout << "\033[33m Filosofo " << id << " suelta tenedor izq ..." << izq << " \033[37m" << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, izq, soltar, MPI_COMM_WORLD);
		cout << "\033[33m Filosofo " << id << " suelta tenedor der ..." << der << " \033[37m" << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, der, soltar, MPI_COMM_WORLD);
		cout << "\033[34m Filosofo " << id << " PENSANDO \033[37m" << endl << flush;
		usleep(1000U * (100U + (rand() % 900U)));
	}
}

// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos){
	int buf;
	MPI_Status status;
	int Filo;  
	while(true){
		MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, coger, MPI_COMM_WORLD, &status);
		Filo = status.MPI_SOURCE;
		cout << "\033[35m Ten. " << id << " recibe petic. de " << Filo << " \033[37m" << endl << flush;
		MPI_Recv(&Filo, 1, MPI_INT, Filo, soltar, MPI_COMM_WORLD, &status);
		cout << "\033[35m Ten. " << id << " recibe liberac. de " << Filo << " \033[37m" << endl << flush;
	}
}

// ---------------------------------------------------------------------

int main(int argc, char** argv){
	int rank, size;
	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size!=10){
		if(rank == 0) 
			cout << "\033[31m El numero de procesos debe ser 10 \033[37m" << endl << flush;
		MPI_Finalize();
		return 0;
	}
	if((rank % 2) == 0)
		Filosofo(rank, size);
	else
		Tenedor(rank, size);
	MPI_Finalize();
	return 0;
}
