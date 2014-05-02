/*
 * File:   main.c
 * Author: Dani Peiró
 *
 * Created on 18 de octubre de 2012, 18:42
 */


/*USO VERSION MANUAL: COMPILAR Y EJECUTAR main_manual.c CON GCC EN EL MISMO DIRECTORIO QUE dic_en Y LA CARPETA FUNC CON los modulos chkfunc.c
 *                    playfuncman.c
 *
 * TABLERO SCRABBLE - OVERVIEW---VERSION MANUAL
 * 
 * El funcionamiento del tablero tiene en cuenta todas las reglas del scrabble
 * tradicional en inglés y comprueba que las jugadas que se realizan cumplen con
 * todas ellas mediante diferentes funciones y bucles.
 * 
 * Comprueba:
 * 
 * En la primera jugada, se incluye la casilla central y la palabra es de mas de una
 * letra.
 * 
 * En jugadas siguientes:
 * 
 * Comprueba que no se solapan fichas y que la palabra a introducir conecta con las
 * existentes en al menos un punto
 * 
 * Comprueba que tras cada jugada, todas las palabras del tablero estan en el diccionario
 * 
 * 
 * Ademas de esto, lleva la cuenta de (y asigna) los puntos, los atriles con sus letras,
 * (llenados por el tablero), la bolsa y su contenido y las condiciones de final de partida.
 * 
 * 
 * 
 * Esta es la versión 'manual' cuya unica finalidad es
 * la de comprobar el funcionamiento del tablero en si, y no del servidor, de
 * forma comoda (es como se desarrollo el programa antes de crear el servidor).
 * Es un juego de 4 jugadores, todos en el mismo programa, que
 * juegan por turnos. Cada turno da la opcion de Jugar una palabra, pasar, o
 * cambiar letras (pasando el turno). Este ciclo continua hasta que uno de los
 * atriles está vacío (se rellenan desde la bolsa de fichas al finalizar cada turno,
 * luego un atril vacio al final de turno significa que no hay fichas en la bolsa
 * y el jugador no puede rellenarlo, indicando el fin de la partida) o todos los
 * jugadores pasan 2 veces seguidas. Al salir del bucle, el ultimo jugador
 * que formo palabra recibe los puntos de las letras restantes en los atriles
 * de los demás, si la partida no acabó por 2 pases consecutivos. 
 * 
 * 
 * EL PROGRAMA SE REALIZÓ INICIALMENTE CON LA IDEA DE QUE LO USARIA UN COMPUTADOR
 * Y NO UN JUGADOR HUMANO. POR ESTO NO ES  'FOOL-PROOF', (EXCEPTO ALGUNOS
 * SALVAGUARDAS BASICOS). POR EJEMPLO, SI SE INTRODUCEN CHARS CUANDO EL PROGRAMA
 * ESPERA INTS, ES PROBABLE QUE ENTRE EN BUCLE INFINITO O SE BLOQUEE.
 * 
 * 
 * BUGS VERSION MANUAL: NINGUNO CONOCIDO.
 *
 * 
 * 
 * NOTA: Existen trozos de código comentados para evitar su compilacion. Es codigo
 * que en el desarrollo del programa resultaron utiles para buscar fallos, y que se
 * han mantenido para comprobaciones futuras si fueran necesarias.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "func/chkfunc.c"
#include "func/playfuncman.c"

/*Las notas introducidas en el codigo de este archivo son aplicables al archivo main_tablero.c.
se han escrito aqui porque el codigo de comunicaciones hace que el otro archivo sea menos legible.*/

int main() {

    int i = 0, j = 0, njugador = 0, primera = 1, sel = 0, pases = 0;
    
    ficha tab[15][15];
    ficha bolsa[101], vacio = {.c = '\0', .v = -1};
    ficha atril1[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril2[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril3[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril4[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha (*p)[7];  //puntero a array de fichas de 7 elementos. puntero a 'atril'.
    int punt[4] = {0, 0, 0, 0};


    char cambio[7];


    crea(tab);
    /*for (i = 2; i < 6; i++) {
        tab[1][i].c = prueba[i - 2];
        tab[1][i].v = 1;
    }*/

    llenab(bolsa);
    llenaat(bolsa, atril1, vacio);
    llenaat(bolsa, atril2, vacio);
    llenaat(bolsa, atril3, vacio);
    llenaat(bolsa, atril4, vacio);
    // Para solucionar el problema de repeticion de codigo que habia en la primera entrega, 
    // se ha creado un puntero a ficha[7] p para pasar el atril correspondiente de cada jugador. 
    // Asi, se reduce 4 veces el codigo resultante.
    do {
        if (njugador < 4) njugador++;
        else njugador = 1;
        // p apunta al atril correspondiente a cada turno.
        if (njugador == 1) p = &atril1;
        if (njugador == 2) p = &atril2;
        if (njugador == 3) p = &atril3;
        if (njugador == 4) p = &atril4;
        // siempre que haya que pasar el atril del jugador como argumento, se pasa *p.

        do {
            ver(tab);
            llenaat(bolsa, *p, vacio);
            printf("\n\n sus letras: \n\n");
            for (i = 0; i < 7; i++) printf("[%c] ", (*p)[i].c);
            printf("\n\nElija lo que quiere hacer JUGADOR %i: \n1- Jugar Palabra\n2- Pasar\n3-Cambiar fichas\n\nSeleccion:", njugador);
            scanf("%i", &sel);
            fflush(stdin);
        } while (!(sel == 1 || sel == 2 || sel == 3));


        switch (sel) {
                //si elige 1. jugar palabra
            case 1:
            { //se resetea el numero de pases consecutivos (2 seguidos por jugador es condicion de fin de partida)
                pases = 0;
                //se ejecuta la funcion jugada, explicada en el modulo playfuncman.c
                jugada(tab, bolsa, punt, *p, vacio, primera, njugador);
                //si era la primera jugada (primera inicializada a 1), para la siguiente ya no es la primera jugada.
                if (primera) primera = 0;
                //rellena el atril, funcion explicada en modulo playfuncman.c, pasa al turno siguiente.
                llenaat(bolsa, *p, vacio);
            }   for (i = 0; i < 7; i++) printf("[%c] ", (*p)[i].c);
                break;
            case 2:
            { //si se selecciona 2. pasar, simplemente se aumenta el numero de pases consecutivos en uno
                //y se pasa al siguiente jugador
                pases++;


            }
                break;
            case 3:
            { //si se elige 3. cambiar letras, se resetea pases como en case 1
                pases = 0;
                //se solicitan las letras a cambiar del atril y se leen al buffer cambio
                printf("\n\n sus letras: \n\n");
                for (i = 0; i < 7; i++) printf("[%c] ", (*p)[i].c);
                printf("\n\nLetras que quiere devolver a la bolsa:\n");
                scanf("%s", cambio);
                fflush(stdin);
                //se cambian las letras, funcion explicada en modulo playfuncman.c
                cambiar(*p, bolsa, cambio, vacio);
                //rellena atril despues de cambiar,muestra las letras obtenidas por pantalla y pasa turno
                llenaat(bolsa, *p, vacio);
                printf("\n\n sus letras: \n\n");
                for (i = 0; i < 7; i++) printf("[%c] ", (*p)[i].c);

            }
                break;

        }





        //muestra informacion sobre el estado de la partida.
        printf("Sacadas de Bolsa=%i    PASES=%i", bolsa[100].v, pases);
        //comprueba que ningun atril esta vacio y que hay menos de 2 pases por jugador (en este caso, siempre son
        //cuatro jugadores, por tanto pases<8). Si es asi, repite el bucle.
    } while (!atrilvacio(atril1) && !atrilvacio(atril2) && !atrilvacio(atril3) && !atrilvacio(atril4) && pases < 8);
    //si la partida ha acabado y no ha sido por pases consecutivos, el ultimo jugador (njugador-1 es por diferencia entre
    //indice del vector punt y el njugador que va de 1 a 4) se lleva los puntos restantes de los atriles de los contrincantes.
    if (pases < 8) {
        for (i = 0; i < 7; i++) {
            if (atril1[i].v > 0) {
                punt[njugador - 1] = punt[njugador - 1] + atril1[i].v;
            }
            if (atril2[i].v > 0) {
                punt[njugador - 1] = punt[njugador - 1] + atril2[i].v;
            }
            if (atril3[i].v > 0) {
                punt[njugador - 1] = punt[njugador - 1] + atril3[i].v;
            }
            if (atril4[i].v > 0) {
                punt[njugador - 1] = punt[njugador - 1] + atril4[i].v;
            }

        }

    }
    //imprime resultado de partida.
    printf("\n\n\n\n\nPuntos Finales:\n\nJugador 1: %i\n\nJugador 2: %i \n\nJugador 3: %i\n\nJugador 4: %i", punt[0], punt[1], punt[2], punt[3]);

    return 0;
}

