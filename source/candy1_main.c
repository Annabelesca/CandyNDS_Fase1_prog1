/*------------------------------------------------------------------------------

	$ candy1_main.c $

	Programa principal para la práctica de Computadores: candy-crash para NDS
	(2º curso de Grado de Ingeniería Informática - ETSE - URV)
	
	Analista-programador: santiago.romani@urv.cat
	Programador 1: annabel.pizarro@estudiants.urv.cat
	Programador 2: nuria.cardiel@estudiants.urv.cat
	Programador 3: matiasariel.larrosa@estudiants.urv.cat
	Programador 4: oriol.villaro@estudiants.urv.cat

------------------------------------------------------------------------------*/
#include <nds.h>
#include <stdio.h>
#include <time.h>
#include <candy1_incl.h>


/* variables globales */
char matrix[ROWS][COLUMNS];		// matriz global de juego
int seed32;						// semilla de números aleatorios
int level = 0;					// nivel del juego (nivel inicial = 0)
int points;						// contador global de puntos
int movements;					// número de movimientos restantes
int gelees;						// número de gelatinas restantes
extern char mat_recomb1[ROWS][COLUMNS];	// mapas de configuración
extern char mat_recomb2[ROWS][COLUMNS];	// mapas de configuración



/* actualizar_contadores(code): actualiza los contadores que se indican con el
	parámetro 'code', que es una combinación binaria de booleanos, con el
	siguiente significado para cada bit:
		bit 0:	nivel
		bit 1:	puntos
		bit 2:	movimientos
		bit 3:	gelatinas  */
void actualizar_contadores(int code)
{
	if (code & 1) printf("\x1b[38m\x1b[8;26H %d", level);
	if (code & 2) printf("\x1b[39m\x1b[2;8H %d  ", points);
	if (code & 4) printf("\x1b[38m\x1b[1;28H %d ", movements);
	if (code & 8) printf("\x1b[37m\x1b[2;28H %d ", gelees);
}

/* imprimirInfo(void): imprime por pantalla la configuracion de botones
	disponibles para interactuar con el programa asi como tambien
	las direcciones de memoria donde se encuentra informacion de interes*/
void imprimirInfo()
{
	//teclas
	printf("\x1b[39m\x1b[10;18H  A: New lvl");
	printf("\x1b[39m\x1b[12;18H  B: Reset lvl");
	printf("\x1b[39m\x1b[14;18H  L: conf/game");
	printf("\x1b[39m\x1b[16;18H  R: recombina");
	printf("\x1b[39m\x1b[18;18H  SELECT: Copy");
	printf("\x1b[39m\x1b[19;18H  mapa config");
	
	//direcciones de memoria
	printf("\x1b[43m\x1b[4;0H Matriz juego:              ");
	printf("\x1b[45m\x1b[1;0H @recomb1: %p", &mat_recomb1);
	printf("\x1b[46m\x1b[2;0H @recomb2: %p", &mat_recomb2);
	printf("\x1b[42m\x1b[3;0H @matriz: %p", &matrix);
	
}


/* Programa principal: control general del juego */
int main(void)
{
	seed32 = time(NULL);		// fijar semilla de números aleatorios
	consoleDemoInit();			// inicialización de pantalla de texto
	printf("candyNDS (Prueba prog1)\n");
	printf("\x1b[38m\x1b[8;18H  nivel:");
	actualizar_contadores(1);

	inicializa_matriz(matrix, level);
	escribe_matriz(matrix);
	retardo(5);
	int mapActual = 1;
	imprimirInfo();
	
	do{
		do{	
			swiWaitForVBlank();
			scanKeys();					// esperar pulsación tecla 'A', 'B' o 'L'
		} while (!(keysHeld() & (KEY_A | KEY_B | KEY_L | KEY_R | KEY_SELECT)));
		retardo(5);	
		if (keysHeld() & (KEY_A | KEY_B)) {	//Si pulsa 'A' o 'B'
		//Si pulsa A, se actualizará el mapa para el siguiente nivel, en caso contrario, reinicializara el mismo nivel
			if (keysHeld() & (KEY_A)){	
				level = (level + 1) % MAXLEVEL;
				actualizar_contadores(1);
			}
			inicializa_matriz(matrix, level);	
			escribe_matriz(matrix);
		}
			
		if (keysHeld() & KEY_L){	// si pulsa 'L',
		// alternamos entre mapa juego y mapa configuracion
			if (mapActual==1) {
				printf("\x1b[43m\x1b[4;0H Matriz juego:             ");
				escribe_matriz(matrix);
			}else{
				printf("\x1b[42m\x1b[4;0H Mapa configuracion:");
				escribe_matriz(mapas[level]);
			}
			mapActual=!mapActual;
		}
		
		if (keysHeld() & KEY_R){	// si pulsa 'R',
		// Llamamos a la funcion para recombinar la matriz 
			recombina_elementos(matrix);
			printf("\x1b[43m\x1b[4;0H Se ha recombinado la matriz        ");
			retardo(10);
			
			//Mostramos mat_recomb1 y esperamos que se pulse algo para mostrar mat_recomb2
			printf("\x1b[43m\x1b[4;0H mat_recomb1:                              \n                                     ");
			escribe_matriz(mat_recomb1);
			do{
				scanKeys();
			}while(!keysHeld());
			retardo(5);
			
			//Mostramos mat_recomb2 y esperamos que se pulse algo para mostrar la matriz recombinada
			printf("\x1b[43m\x1b[4;0H mat_recomb2:                               ");
			escribe_matriz(mat_recomb2);
			do{
				scanKeys();
			}while(!keysHeld());
			retardo(5);
			printf("\x1b[43m\x1b[4;0H Matriz juego:                          ");
			escribe_matriz(matrix);
		}
		
		if (keysHeld() & KEY_SELECT){
			printf("\x1b[43m\x1b[4;0H Matriz configuracion:");
			copia_mapa(matrix, level);		
			escribe_matriz(matrix);
		}
		
	} while (1);
		
		
	return(0);
}