//gcc ejecutivociclico.c utilRT.c -o ejecutivo -lrt

#include <errno.h>
#include <stdio.h>
#include <time.h>

#include "utilRT.h"

struct timespec origenTiempo;

// 0 -> A, B, C
// 1 -> A, B, D
// 2 -> A, B, C
// 3 -> A, B
// Marco = 4
// nciclos = 5
// clock_nano -> simular C (ms)
// Para pasar segundos -> getTimeSpec
// El tiempo deadline es T (ms)

void TareaA(void);
void TareaB(void);
void TareaC(void);
void TareaD(void);

// *****************************************************************************

int main(int argc, char **argv){
	struct timespec activacionTiempo;
	struct timespec cicloMarco;
	int nciclos = 4;
	int marco = 0;
	cicloMarco = getTimespec(5);
	clock_gettime(CLOCK_REALTIME, &origenTiempo);
	//origenTiempo = getTimespec(CLOCK_REALTIME);
	activacionTiempo = sumaTiempos(&origenTiempo, &cicloMarco);
	while(1){
		switch(marco){
			case 0: TareaA(); TareaB(); TareaC();
				break;
			case 1: TareaA(); TareaB(); TareaD();
				break;
			case 2:	TareaA(); TareaB(); TareaC();
				break;
			case 3: TareaA(); TareaB();
				break;
		}
		marco = (marco+1) % nciclos;
		clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &activacionTiempo, NULL);
		activacionTiempo = sumaTiempos(&activacionTiempo, &cicloMarco);
	}
	return 0;
}

// *****************************************************************************

void TareaA(void){
	struct timespec tInicio, tFinal, tActivacion, tComputo, tiempo;
	static int i = 0;
	tComputo = getTimespec(1);
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tActivacion = sumaTiempos(&tComputo, &tiempo);
	tInicio = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea A: activacion = %d, tiempo activacion = %.6f seg\n", i, getSeconds(&tInicio));
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea A: activacion = %d, tiempo finalizacion = %.6f seg\n", i, getSeconds(&tFinal));
	i++;
}

// *****************************************************************************

void TareaB(void){
	struct timespec tInicio, tFinal, tActivacion, tComputo, tiempo;
	static int i = 0;
	tComputo = getTimespec(1.5);
	clock_gettime(CLOCK_REALTIME, &tiempo);
	tActivacion = sumaTiempos(&tComputo, &tiempo);
	tInicio = restaTiempos(&tiempo, &origenTiempo);
	printf("Tarea B: activacion = %d, tiempo activacion = %.6f seg\n", i, getSeconds(&tInicio));
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tFinal = restaTiempos(&tiempo,&origenTiempo);
	printf("Tarea B: activacion = %d, tiempo finalizacion = %.6f seg\n", i, getSeconds(&tFinal));
	i++;
}

// *****************************************************************************

void TareaC(void){
	struct timespec tInicio, tFinal, tActivacion, tComputo, tiempo;
	static int i = 0;
	tComputo = getTimespec(2);
	clock_gettime(CLOCK_REALTIME,&tiempo);
	tActivacion = sumaTiempos(&tComputo, &tiempo);
	tInicio = restaTiempos(&tiempo, &origenTiempo);
	printf("Tarea C: activacion = %d, tiempo activacion = %.6f seg\n", i, getSeconds(&tInicio));
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);
	clock_gettime(CLOCK_REALTIME, &tiempo);
	tFinal = restaTiempos(&tiempo, &origenTiempo);
	printf("Tarea C: activacion = %d, tiempo finalizacion = %.6f seg\n", i, getSeconds(&tFinal));
	i++;
}

// *****************************************************************************

void TareaD(void){
	struct timespec tInicio, tFinal, tActivacion, tComputo, tiempo;
	static int i = 0;
	tComputo = getTimespec(2.4);
	clock_gettime(CLOCK_REALTIME, &tiempo);
	tActivacion = sumaTiempos(&tComputo, &tiempo);
	tInicio = restaTiempos(&tiempo, &origenTiempo);
	printf("Tarea D: activacion = %d, tiempo activacion = %.6f seg\n", i, getSeconds(&tInicio));
	clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &tActivacion, NULL);
	clock_gettime(CLOCK_REALTIME, &tiempo);
	tFinal = restaTiempos(&tiempo, &origenTiempo);
	printf("Tarea D: activacion = %d, tiempo finalizacion = %.6f seg\n", i, getSeconds(&tFinal));
	i++;
}
