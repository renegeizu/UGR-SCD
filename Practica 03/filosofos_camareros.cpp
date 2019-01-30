#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

void Filosofo(int id, int nprocesos);
void Tenedor(int id, int nprocesos);
void Camarero (int id, int nprocesos);

// ---------------------------------------------------------------------

int main(int argc, char** argv){
	int rank, size;
	srand(time(0));
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if(size!=11){
		if( rank == 0) 
			cout << "\033[31m El numero de procesos debe ser 11 \033[37m" << endl << flush;
		MPI_Finalize();
		return 0;
	}
	if(rank == 10)
		Camarero(rank, size);
	else if((rank % 2) == 0)
		Filosofo(rank, size);
	else
		Tenedor(rank, size);
	MPI_Finalize();
	return 0;
}

// ---------------------------------------------------------------------

void Filosofo(int id, int nprocesos){
	/*
	 * Solucion propuesta en clase:
	 * El primer filosofo coge el tenedor de la derecha para evitar
	 * interbloqueo
	 */
	int izq = (id + 1) % (nprocesos - 1);
	int der = ((id + nprocesos) - 2) % (nprocesos - 1);
	MPI_Status status;
	while(1){
		cout << "\033[33m Filosofo " << id << " solicita sentarse \033[37m" << endl;
		MPI_Send(NULL, 0, MPI_INT, 10, 3, MPI_COMM_WORLD);
		MPI_Recv(NULL, 0, MPI_INT, 10, 3, MPI_COMM_WORLD, &status);
		cout << "\033[33m Filosofo " << id << " se sienta \033[37m" << endl;
		if (id != 0){
			cout << "\033[33m Filosofo " << id << " solicita tenedor izq ..." << izq << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izq, 1, MPI_COMM_WORLD);
			cout << "\033[33m Filosofo " << id << " coge tenedor der ..." << der << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der, 1, MPI_COMM_WORLD);
		}else{
			cout << "\033[33m Filosofo " << id << " coge tenedor der ..." << der << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der, 1, MPI_COMM_WORLD);
			cout << "\033[33m Filosofo " << id << " solicita tenedor izq ..." << izq << " \033[37m" << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izq, 1, MPI_COMM_WORLD);
		}
		cout << "\033[34m Filosofo " << id << " COMIENDO \033[37m" << endl << flush;
		sleep((rand() % 3) + 1);
		cout << "\033[33m Filosofo " << id << " suelta tenedor izq ..." << izq << " \033[37m" << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, izq, 0, MPI_COMM_WORLD);
		cout << "\033[33m Filosofo " << id << " suelta tenedor der ..." << der << " \033[37m" << endl << flush;
		MPI_Ssend(NULL, 0, MPI_INT, der, 0, MPI_COMM_WORLD);
		cout << "\033[33m Filosofo " << id << " se levanta \033[37m" << endl;
        	MPI_Ssend(NULL, 0, MPI_INT, 10, 2, MPI_COMM_WORLD );
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
		MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
		Filo = status.MPI_SOURCE;
		cout << "\033[35m Ten. " << id << " recibe petic. de " << Filo << " \033[37m" << endl << flush;
		MPI_Recv(&Filo, 1, MPI_INT, Filo, 0, MPI_COMM_WORLD, &status);
		cout << "\033[35m Ten. " << id << " recibe liberac. de " << Filo << "\033[37m" << endl << flush;
	}
}

// ---------------------------------------------------------------------

void Camarero(int id, int nprocesos){
	int buf, sentados = 0;
	MPI_Status status;
	while(true){
        	if(sentados < 4)
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		else
			MPI_Probe(MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG == 3){
			buf = status.MPI_SOURCE;
			MPI_Recv(NULL, 0, MPI_INT, buf, 3, MPI_COMM_WORLD,&status);
			sentados++;
			MPI_Send(NULL, 0, MPI_INT, buf, 3, MPI_COMM_WORLD);
			cout << "\033[32m Filosofo " << buf << " se sienta. Hay " << sentados << " filosofos sentados \033[37m" << endl;
		}
		if(status.MPI_TAG == 2){
			buf = status.MPI_SOURCE;
			MPI_Recv( NULL, 0, MPI_INT, buf, 2, MPI_COMM_WORLD,&status);
			sentados--;
			cout << "\033[32m Filosofo " << buf << " se levanta. Hay " << sentados << " filosofos sentados \033[37m" << endl;
		}
	}
}
