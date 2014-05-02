


/*MODULO DE FUNCIONES DE JUEGO MANUAL*/

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void crea(ficha tab[15][15]) {

    /*Crea el tablero vacio inicial con las casillas de bonificacion
     como se corresponden al scrabble estandar inglés*/


    int i, j;
    //Define las fichas especiales, triple,doble palabra, triple,doble letra para usarlas en las asignaciones.
    ficha tp = {.c = '!', .v = -30}, dp = {.c = '*', .v = -20}, tl = {.c = '3', .v = -3}, dl = {.c = '2', .v = -2}, vacio = {.c = '.', .v = -1};


    //Son solo bucles para facilitar la construccion las partes repetidas del tablero.
    //Podria haberse hecho por asignacion una a una, pero asi es mas eficiente.
    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++) {
            tab[i][j] = vacio;
            if (i == j || (14 - j) == i) {
                tab[i][j] = dp;
                if (i == 0 || i == 14) tab[i][j] = tp;
                if (i == 5 || i == 9) tab[i][j] = tl;
                if ((i == 6 || i == 8) || i == 7) tab[i][j] = dl;
            }

        }
    }
    for (i = 5; i < 10; i++) {
        for (j = 1; j < 4; j++) {
            if (tab[i][j + 4].c != dp.c) {
                tab[i][j] = tab[i][j + 4];
                tab[i][14 - j] = tab[i][10 - j];

            }

        }
    }
    for (i = 1; i < 4; i++) {
        for (j = 5; j < 10; j++) {
            if (tab[i + 4][j].c != dp.c) {
                tab[i][j] = tab[i + 4][j];
                tab[14 - i][j] = tab[10 - i][j];

            }

        }
    }
    tab[3][0] = dl;
    tab[7][0] = tp;
    tab[11][0] = dl;
    tab[7][7].c = 'X';
    tab[7][7].v = -20;
    for (i = 3; i < 12; i++) {
        tab[0][i] = tab[i][0];
        tab[14][i] = tab[i][0];
        tab[i][14] = tab[i][0];
    }

}

void ver(ficha tab[15][15]) {
    /*imprime por pantalla una representación visual del tablero asi como una
     leyenda para entender los simbolos*/
    int i, j;
    printf("\n\nLeyenda: !=Triple Palabra *=Doble Palabra 3=Triple Letra 2=Doble Letra\n\n");
    printf("\n\n     1   2   3   4   5   6   7   8   9   10  11  12  13  14  15");
    printf("\n     __________________________________________________________");
    printf("\n 1| ");

    for (i = 0; i < 15; i++) {
        for (j = 0; j < 15; j++) {
            printf("[%c] ", tab[i][j].c);
            if (j == 14) {
                if (i != 14) printf("\n%2i| ", i + 2);

            }
        }

    }
    printf("\n\n");
}

void llenab(ficha b[101]) {
    /*llena un vector de fichas b con las letras correspondientes al scrabble
     estandar inglés. Además de esto, tiene una ficha extra, la de posicion 100,
     que se utiliza para llevar la cuenta de fichas extraidas de la bolsa a lo
     largo de la partida. Podría hacerse con un entero sin mas, pero asi nos
     resultaba mas comodo*/
    int i;

    //la ultima ficha nunca se alcanza y como se ha dicho se usa para contabilizar las fichas extraidas
    b[100].c = ' ';
    b[100].v = 0;
    //recorre la bolsa del 0 al 99, creando tantas fichas de letras con sus valores correspondientes como corresponde.
    for (i = 0; i < 100; i++) {
        if (i < 9) {
            b[i].c = 'A';
            b[i].v = 1;

        } else if (i < 11) {
            b[i].c = 'B';
            b[i].v = 3;
        } else if (i < 13) {
            b[i].c = 'C';
            b[i].v = 3;
        } else if (i < 17) {
            b[i].c = 'D';
            b[i].v = 2;
        } else if (i < 29) {
            b[i].c = 'E';
            b[i].v = 1;
        } else if (i < 31) {
            b[i].c = 'F';
            b[i].v = 4;
        } else if (i < 34) {
            b[i].c = 'G';
            b[i].v = 2;
        } else if (i < 36) {
            b[i].c = 'H';
            b[i].v = 4;
        } else if (i < 45) {
            b[i].c = 'I';
            b[i].v = 1;
        } else if (i < 46) {
            b[i].c = 'J';
            b[i].v = 8;
        } else if (i < 47) {
            b[i].c = 'K';
            b[i].v = 5;
        } else if (i < 51) {
            b[i].c = 'L';
            b[i].v = 1;
        } else if (i < 53) {
            b[i].c = 'M';
            b[i].v = 3;
        } else if (i < 59) {
            b[i].c = 'N';
            b[i].v = 1;
        } else if (i < 67) {
            b[i].c = 'O';
            b[i].v = 1;
        } else if (i < 69) {
            b[i].c = 'P';
            b[i].v = 3;
        } else if (i < 70) {
            b[i].c = 'Q';
            b[i].v = 10;
        } else if (i < 76) {
            b[i].c = 'R';
            b[i].v = 1;
        } else if (i < 80) {
            b[i].c = 'S';
            b[i].v = 1;
        } else if (i < 86) {
            b[i].c = 'T';
            b[i].v = 1;
        } else if (i < 90) {
            b[i].c = 'U';
            b[i].v = 1;
        } else if (i < 92) {
            b[i].c = 'V';
            b[i].v = 4;
        } else if (i < 94) {
            b[i].c = 'W';
            b[i].v = 4;
        } else if (i < 95) {
            b[i].c = 'X';
            b[i].v = 8;
        } else if (i < 97) {
            b[i].c = 'Y';
            b[i].v = 4;
        } else if (i < 98) {
            b[i].c = 'Z';
            b[i].v = 10;
        } else if (i < 100) {
            b[i].c = '?';
            b[i].v = 0;
        }
    }

}

void mayus(char palab[]) {
    /*una funcion sencilla para pasar un string a mayusculas con la funcion toupper*/
    int i;
    for (i = 0; i < strlen(palab); i++) {
        palab[i] = toupper(palab[i]);

    }
}

void llenaat(ficha bolsa[101], ficha atril[7], ficha vacio) {
    int i, j;
    /* si la bolsa no est� vacia, esto es: se han extraido menos de 100 fichas de ella,
     para cada uno de los puestos del atril (for i=0--->7) genera un numero aleatorio
     en base al cual seleccione una ficha de las 100 de la bolsa. repite la seleccion
     del numero aleatorio hasta que no sea un puesto vacio de la bolsa (while(bolsa[j]==vacio
     una vez encontrada aleatoriamente una ficha no vacia, si ese puesto del atril
     est� vacio, lo llena con la ficha de la bolsa, lo elimina de la bolsa y aumenta en uno
     el contador de fichas extraidas de la bolsa. Es probable que fuera mejor comprobar
     si la ficha del atril est� vacia ANTES del bucle de numero aleatorio para evitar
     ciclos de bucle sin utilidad, pero tampoco es algo critico.*/

    //si la bolsa no esta vacia
    if (bolsa[100].v < 100) {
        //para cada letra del atril
        for (i = 0; i < 7; i++) {

            //genera un numero aleatorio entre 0 y 99 con semilla el tiempo, y repite hasta que se seleccione
            //un puesto no vacio
            do {
                srand(time(NULL) + j * j - time(NULL) * j - i * j * time(NULL) - i * i);
                j = rand() % 99;
            } while (bolsa[j].v == vacio.v);
            //si el puesto del atril esta vacio, asigna la ficha a ese puesto, elimina la ficha de la bolsa
            //y aumenta en uno las letras sacadas de la bolsa
            if (atril[i].v == vacio.v) {
                atril[i] = bolsa[j];
                bolsa[j] = vacio;
                bolsa[100].v++;
            }



        }
    }


}

void sacar(char palab[], ficha a[], ficha pos[15], ficha vacio, const int boo) {

    /*crea un vector de fichas pos a partir de la palabra introducida por el
     *jugador y las letras presentes en el atril. Tiene dos modos, indicados por el
     *valor del entero boo:
     * 
     * si boo es 0 está en modo 'solo comprobacion' y deja el atril original sin tocar
     * porque aun no se ha comprobado la validez de la jugada (en el cuerpo principal
     * del programa).
     * 
     *si boo es !=0 está en modo 'ya comprobado' y quita las letras del atril que se
     * han utilizado porque ya se ha comprobado (en el cuerpo principal del programa)
     * que la jugada es valida.
     * 
     *Tambien asigna el valor de .c para los comodines, si son utilizados.      
     */
    int i, j;
    char comod;
    ficha atril[7];

    // si esta en modo comprobacion, utiliza atril auxiliar atril. sino, else: lo mismo con el atril real a.
    if (!boo) {

        for (i = 0; i < 15; i++) {
            pos[i] = vacio;
        }
        for (j = 0; j < 7; j++) {
            atril[j] = a[j];
        }
        //recorriendo la palabra escrita por el usuario palab
        for (i = 0; i < strlen(palab); i++) {
            //asigna al mismo indice de pos, el caracter de palab.
            pos[i].c = palab[i];
            //recorre atril buscando la letra usada, asigna el v del atril y vacia el puesto del atril.
            //sale del bucle con break para evitar vaciar dos letras iguales en el atril
            for (j = 0; j < 7; j++) {
                if (pos[i].c == atril[j].c && atril[j].v >= 0) {
                    pos[i].v = atril[j].v;
                    atril[j] = vacio;
                    break;
                }
            }

        }
        //en caso de que haya comodin, pide al usuario introducir la letra para el comodin.
        //no es necesario hacer esto en el caso ya comprobado, porque el caso no comprobado siempre se hace antes
        //el comodin queda definido.
        for (i = 0; i < strlen(palab); i++) if (palab[i] == '?') {
                printf("\n\nLetra para el Comodin(valor 0, no multiplica.Si estan ambos comodines, por orden de aparicion en palabra):\n");
                while (getchar() != '\n');
                scanf("%c", &comod);
                fflush(stdin);
                pos[i].c = toupper(comod);
            }
    } else {


        for (i = 0; i < strlen(palab); i++) {

            for (j = 0; j < 7; j++) {
                if (palab[i] == a[j].c && a[j].v >= 0) {
                    pos[i].v = a[j].v;
                    a[j] = vacio;
                    break;
                }
            }

        }


    }
}

void metertab(ficha pos[15], char palab[15], ficha tab[15][15], int i1, int j1, char hv) {
    /*Mete el vector de fichas pos en el tablero tab a partir de las coordenadas
     * i1 j1 y la orientacion hv. Solo se hace cuando las comprobaciones son satisfactorias.
     */

    int i = 0, j = 0;

    //caso horizontal. recorre desde la coordenada j1 el vector pos y lo inserta en el tablero dado.
    //no hay comprobacion ya que solo se usa la funcion tras hacer todas las comprobaciones.

    if (hv == 'h') {

        for (j = j1; j < j1 + strlen(palab); j++) {
            if (pos[j - j1].c != '_' && pos[j - j1].c != '\0') tab[i1][j] = pos[j - j1];
                //comprueba que se llega al final de pos, mirando su c==\0
            else if (pos[j - j1].c == '\0') break;

        }

        //idem para caso vertical.
    } else if (hv == 'v') {


        for (i = i1; i < i1 + strlen(palab); i++) {
            if (pos[i - i1].c != '_' && pos[i - i1].c != '\0') tab[i][j1] = pos[i - i1];
            else if (pos[i - i1].c == '\0') break;

        }
    }

}

int calculapuntos(ficha tab[15][15], ficha pos[15], char palab[15], int i1, int j1, char hv, const int recur) {
    /*calcula los puntos de una jugada:
     * 
     * primero calcula los puntos de la palabra principal (la indicada por i1,j1 y hv)
     * despues, mediante el entero recur (de recursivo) añade los puntos de todas
     * las palabras 'secundarias' (creadas como combinacion de las de la principal
     * con las direcciones contrarias a la principal (i.e, si la palabra principal
     * esta en horizontal, las secundarias tienen que estar en vertical))
     * 
     * 
     * Aqui entran en juego los valores negativos de los multiplicadores. Cambiandolos
     * de signo y multiplicando por la letra que se va a colocar en esa casilla
     * se obtiene la puntuacion de esa letra. De forma similar los multiplicadores
     * de palabra se acumulan en el entero m, dividiendo por 10 y cambiando de signo
     * devolviendo el valor acumulado de las letras multiplicado por m.
     * 
     * Si se han utilizado 7 letras propias (no del tablero), suma 50 puntos al final     
     */
    int i = 0, j = 0, ptos = 0, m = 1, k = 0, scra = 0;
    i = i1;
    j = j1;
    ficha p[15];

    //rellena la ficha auxiliar p, usada para las palabras secundarias, con fichas vacio.
    for (k = 0; k < 15; k++) {
        p[k].c = '\0';
        p[k].v = -1;
    }
    //caso palabra principal horizontal.
    if (hv == 'h') {
        //bucle while, que recorre horizontalmente (j++) hasta que ya no hay letras.
        while (tab[i][j].v >= 0 || pos[j - j1].v >= 0) {
            //si es una letra puesta por el usuario, hay que considerar el multiplicador.
            if (pos[j - j1].v >= 0) {
                //si el multiplicador no es de palabra (v>-10) solo hay que multiplicar la ficha
                //del usuario por el valor de la casilla en la que esta, cambiada de signo
                if (tab[i][j].v>-10) ptos = ptos + (pos[j - j1].v)*(-tab[i][j].v);
                    //si es de palabra, y no es un comodin (los comodines no multiplican nunca),
                    //hace que el factor de multiplicacion m de toda la palabra, sea ahora el que era,
                    //multiplicado por el v de esta casilla cambiado de signo y dividido entre 10 
                    //(-20 para doble palabra, -30 para triple palabra)
                else if (pos[j - j1].v > 0) {
                    ptos = ptos + (pos[j - j1].v);
                    m = m * (-tab[i][j].v) / 10;
                }
                //sino es una letra puesta por el usuario, solo hay que sumar el valor de la letra sin multiplicar.
            } else ptos = ptos + tab[i][j].v;
            j++;
        }
        //acumula los puntos de letras anteriores a la posicion inicial de letra dada, ya que siempre se calculan
        //los puntos tras comprobar dicc y alineacion, estas letras forman, seguro, parte de la palabra principal.
        j = j1 - 1;
        while (tab[i][j].v >= 0 && j >= 0) {
            ptos = ptos + tab[i][j].v;
            j--;
        }
        j = j1;
        //si recur==1, es decir, ejecucion de calculapuntos para palabra principal, deben contabilizarse las secundarias
        if (recur) {
            for (j = j1; j < 15; j++) {
                //si es una letra puesta por el usuario y hay letras arriba o abajo de esta, p es ahora la letra de la
                //palabra principal de indice j. Se ejecuta con recur=0 y p como vector, calculapuntos en esa vertical y se suma a los ptos
                //generales.
                if (pos[j - j1].v >= 0 && ((i1 + 1 < 15 && tab[i1 + 1][j].v >= 0) || (i1 - 1 >= 0 && tab[i1 - 1][j].v >= 0))) {
                    p[0] = pos[j - j1];
                    ptos = ptos + calculapuntos(tab, p, palab, i1, j, 'v', 0);
                }
            }
        }
        //idem para vertical cambiando iteradores.
    } else if (hv == 'v') {
        while (tab[i][j].v >= 0 || pos[i - i1].v >= 0) {
            if (pos[i - i1].v >= 0) {
                if (tab[i][j].v>-10) ptos = ptos + (pos[i - i1].v)*(-tab[i][j].v);
                else if (pos[i - i1].v > 0) {
                    ptos = ptos + (pos[i - i1].v);
                    m = m * (-tab[i][j].v) / 10;
                }
            } else ptos = ptos + tab[i][j].v;
            i++;
        }
        i = i1 - 1;
        while (tab[i][j].v >= 0 && i >= 0) {
            ptos = ptos + tab[i][j].v;
            i--;
        }
        i = i1;
        if (recur) {
            for (i = i1; i < 15; i++) {
                if (pos[i - i1].v >= 0 && ((j1 + 1 < 15 && tab[i][j1 + 1].v >= 0) || (j1 - 1 >= 0 && tab[i][j1 - 1].v >= 0))) {
                    p[0] = pos[i - i1];
                    ptos = ptos + calculapuntos(tab, p, palab, j1, i, 'h', 0);
                }
            }
        }
    }
    //si se usan las 7 fichas del atril, es 'scrabble'. se suman 50 puntos. bucle para ver si las letras de pos que no son
    // '_' suman 7.
    for (k = 0; k < 15; k++) {
        if (pos[k].v >= 0) scra++;
    }
    //se devuelve los puntos calculados, multiplicados por el multiplicador de palabra acumulado m.
    if (scra == 7) return (m * ptos + 50);
    return m*ptos;
}

//obtiene por teclado la palabra que quiere probar el jugador

int escribirpal(ficha atril[7], char palab[15]) {


    int i = 0, chk_ = 0;

    do {

        printf("\n\n tus letras: \n\n");
        for (i = 0; i < 7; i++) printf("[%c] ", atril[i].c);
        printf("\n\nFormar palabra (Poner _ donde haya fichas del tablero: \n");
        fflush(stdout);
        scanf("%s", palab);
        fflush(stdin);
        chk_ = 0;
        for (i = 0; i < strlen(palab); i++) if (palab[i] != '_' && palab[i] != '\0') {
                chk_ = 1;
                break;
            }

        mayus(palab);
        //el bucle while comprueba que se usan letras solo del atril, y que palab no es todo '_'
    } while (!(chkpalab(palab, atril) && chk_));


}


//funcion que permite hacer jugar una palabra (no pasar, y cambiar letras)

void jugada(ficha tab[15][15], ficha bolsa[101], int punt[4], ficha atril[7], ficha vacio, int primera, int njugador) {

    ficha pos[15];
    char palab[15];
    int i1, j1, repe = 0;
    char hv;
    //Para asignar letras de forma manual al atril. Solo para comprobaciones.

    /*atril[0].c = '?';
    atril[0].v = 0;
    atril[1].c = '?';
    atril[1].v = 0; 
    atril[2].c = 'A';
    atril[2].v = 1;
    atril[3].c = 'N';
    atril[3].v = 1;
    atril[4].c = 'D';
    atril[4].v = 1;
    atril[5].c = 'O';
    atril[5].v = 1;
    atril[6].c = 'N';
    atril[6].v = 1;*/


    do {
        do {
            repe = 0;
            //ensenya tablero
            ver(tab);
            //introducir palabra
            escribirpal(atril, palab);
            //meter coordenadas y orientacion. Bucle para evitar errores.
            do {
                printf("\nCoordenadas de la primera letra y orientacion hor/ver :\nEjemplo: 8 8 h\n\n");
                fflush(stdout);

                scanf("%i %i %c", &i1, &j1, &hv);
                fflush(stdin);
                hv = tolower(hv);
                i1 = i1 - 1;
                j1 = j1 - 1;
            } while (!(i1 >= 0 && i1 < 15 && j1 >= 0 && j1 < 15 && ((hv == 'h' && j1 + strlen(palab) <= 15) || (hv == 'v' && i1 + strlen(palab) <= 15))));
            //crea el vector de fichas pos, sin modificar el atril (aun)
            sacar(palab, atril, pos, vacio, 0);
            //comprobaciones de alineacion y caso de primera jugada.
            //repite bucle si no se cumplen
            if (!chkalin(palab, tab, i1, j1, hv, primera)) {
                printf("\n\n NO ALINEADAS \n\n");
                repe = 1;
            }
            if (!chkprim(primera, palab, i1, j1, hv)) {
                printf("\n\n DEBE INCLUIR EL CENTRO Y SER MAS DE UNA LETRA \n\n");
                repe = 1;
            }
        } while (repe);
        repe = 0;
        //comprueba que todas las palabras del tablero estan en el diccionario
        if (!chktab(pos, palab, tab, i1, j1, hv, vacio)) {
            printf("\n\nNO ESTA EN EL DICCIONARIO\n\n");
            repe = 1;
        }
    } while (repe);
    repe = 0;
    //como ya se ha comprobado que la jugada es valida, saca las letras del atril
    sacar(palab, atril, pos, vacio, 1);
    //calcula los puntos de la jugada
    printf("\n\n puntos:\n\n");
    punt[njugador - 1] = punt[njugador - 1] + calculapuntos(tab, pos, palab, i1, j1, hv, 1);
    printf("\n%i\n\n", punt[njugador - 1]);
    //y finalmente introduce el vector pos en el tablero
    metertab(pos, palab, tab, i1, j1, hv);

}

void cambiar(ficha atril[7], ficha bolsa[101], char cambio[7], ficha vacio) {

    /*funcion para cambiar letras del atril por nuevas de la bolsa, devolviendo
     * las descartadas a la bolsa. Si se ponen letras que no estan en el atril, o la bolsa esta llena, no 
     * hace nada*/

    int i = 0, k = 0, j = 0;
    //convierte a mayusculas el buffer.
    mayus(cambio);
    for (k = 0; k < 7; k++) {
        for (j = 0; j < 7; j++) {
            //si se encuentra la letra en el atril
            if (cambio[k] == atril[j].c && cambio[k] != '\0') {
                //busca en la bolsa un espacio libre
                //cuando lo encuentra, mete en la bolsa la ficha, vacia el puesto del atril,
                //resta 1 a las letras sacadas de la bolsa, se sale del bucle para evitar devolver letras
                //repes en el atril
                for (i = 0; i < 100; i++) {
                    if (bolsa[i].v < 0) {
                        bolsa[i] = atril[j];
                        atril[j] = vacio;
                        bolsa[100].v--;

                        break;
                    }
                }
                break;
            }
        }

    }
}


