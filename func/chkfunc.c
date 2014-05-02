
/*MODULO DE FUNCIONES DE COMPROBACION*/


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct {
    char c;
    int v;
    /* Define la unidad basica del programa: El struct ficha.
     * contiene un char c, que es la letra que hay sobre la ficha y
     * un entero v, el valor asignado a la ficha.
     * Adem·s de las fichas normales con letras + comodines (c=? v=0) existen
     * fichas 'vacio' con c=\0 y v<0 que dependiendo de su funcion tendr·n distinto v:
     * El tablero esta compuesto de 15x15 fichas, sean vacio o letras. Las letras vacio
     * normalmente tiene v=-1 c='.' indicando una posicion 'normal' y libre en el tablero.
     * Las posiciones doble letra tienen v=-2, las triple letra v=-3,
     * las doble palabra v=-20 y las triple v=-30. Estos valores facilitan luego el
     * calculo de la puntuaciÛn simplemente con multiplicaciones (y divisiones en caso
     * de doble palabra/triple palabra) manteniendo el v<0 que asigna la casilla como
     * 'libre'.
     */
} ficha;

int chkpalab(char palab[], ficha a[7]) {
    /*este bucle  recorre palab, que es lo escrito por el usuario
    y comprueba una a una si todas las letras estan en el atril. Si alguna no esta en el atril
    se sale del bucle y devuelve 0 (falso) sino, devuelve 1 (verdadero, o 'pasa el test')
    por supuesto, las letras de palab que se indica que estan ya en el tablero
    mediante '_' estan exentas de estar en el atril. de ahi el if !='_' */
    int i, j;
    int n;


    for (j = 0; j < strlen(palab); j++) {
        //resetea el iterador i y el comprobador n
        i = 0;
        n = 0;
        //si no es una letra que debe estar en el tablero, comprueba que esta en el atril. sino siguiente letra.
        if (palab[j] != '_') {

            //bucle que recorre el atril y mira si la letra esta. si esta n=1
            while (i < 7 && n == 0) {
                if (palab[j] == a[i].c) {
                    n = 1;


                }
                i++;
            }
            //si tras recorrer el atril, n=0, es que no esta la letra, se sale del for y devuelve n=0, falso.
            if (n == 0) break;

        } else n = 1;





    }
    return n;
}

int chkdicc(ficha pos[15]) {
    /*recorre el archivo dic_en (un diccionario ingles normal en txt) linea a linea
     * y compara cada linea con la palabra formada por los .c del vector de fichas
     * pos. Si encuentra una linea igual a la palabra, devuelve 1 (es decir, la
     * palabra esta en el diccionario y por tanto es valida) y sino devuelve 0.
     * Se intento implementar la libreria hunspell pero su instalacion resulto muy
     * compleja y cuando se consiguio, la utilizacion de las funciones no era intuitiva.
     * Este metodo seguramente sea poco eficiente, pero no ralentiza practicamente
     * la ejecucion del programa y por tanto se entiende que es suficiente.
     */
    int i;
    FILE * f;
    char dic[50];
    char palmin[15];
    //pasa a minusculas los caracteres de la palabra a comprobar, porque el dicc esta en minusculas
    for (i = 0; i < 15; i++) {
        palmin[i] = tolower(pos[i].c);
    }
    //abre archivo dic_en
    f = fopen("dic_en", "r");
    
    //mientras no se lea el final del archivo
    while (!feof(f)) {
        //lee 15 caracteres de la siguiente linea de f
        fgets(dic, sizeof(dic), f);
        //quita el caracter nulo de lo leido, ya que palmin no lo tiene
        for (i = 0; i < strlen(dic); i++)
        {
        	if (dic[i]=='\n' || dic[i]=='\r')
        	{
        		dic[i]='\0';
        	}
        }
        //linea de codigo de comprobacion durante desarrollo
        //printf("\n\ndic: %s len=%d  palmin:  %s len=%d\n\n", dic,strlen(dic), palmin, strlen(palmin));

        //compara lo leido del dicc y la palabra a comprobar. si son iguales, devuelve uno y cierra f.
        if (strcmp(dic, palmin) == 0) {
            fclose(f);
            return 1;
        }
    }
    //si no son iguales, devuelve 0
    fclose(f);
    return 0;

}

int chkalin(char palab[15], ficha t[15][15], int i1, int j1, char hv, int primera) {

    /*Comprueba que no se solapen letras al colocarlas en el tablero y que la
     * palabra nueva 'toque' alguna palabra ya existente. Devuelve 1 si es correcta.
     *
     * El entero primera indica si es la primera jugada o no. Si lo es, no se
     * comprueba nada (la funcion chkprim se encarga de las comprobaciones de la
     * primera jugada)
     *
     */
    int i = 0, j = 0;

    //sino es la primera jugada, realiza las comprobaciones
    if (!primera) {
        //caso horizontal (el vertical es identico,cambiando iteradores)
        if (hv == 'h') {
            //recorre la palabra principal (idem, el otro for)
            for (j = j1; j < j1 + strlen(palab); j++) {
                //si la ficha es una letra y hay otra en su lugar, devuelve 0.
                if (palab[j - j1] != '_' && t[i1][j].v >= 0) return 0;
                //si la ficha es _ indicando ficha en tablero, pero no hay ficha en tablero, devuelve 0.
                if (palab[j - j1] == '_' && t[i1][j].v < 0) return 0;
            }

            for (j = j1; j < j1 + strlen(palab); j++) {
                //si la ficha es una letra comprueba las casillas superior e inferior para ver si hay ficha. si la hay, jugada correcta, devuelve 1.
                if (palab[j - j1] != '_' && ((j == j1 && j - 1 >= 0 && t[i1][j - 1].v >= 0) || (j == (j1 + strlen(palab) - 1) && j + 1 < 15 && t[i1][j + 1].v >= 0))) return 1;
                //misma comprobacion anterior, pero en vez de arriba y abajo, en los extremos de la palabra principal
                if ((i1 + 1 < 15 && t[i1 + 1][j].v >= 0) || (i1 - 1 >= 0 && t[i1 - 1][j].v >= 0)) return 1;
                //comprueba una conexion en medio de la palabra.
                if (palab[j - j1] == '_' && t[i1][j].v >= 0) return 1;
            }


        } else if (hv == 'v') {
            for (i = i1; i < i1 + strlen(palab); i++) {
                if (palab[i - i1] != '_' && t[i][j1].v >= 0) return 0;
                if (palab[i - i1] == '_' && t[i][j1].v < 0) return 0;
            }


            for (i = i1; i < i1 + strlen(palab); i++) {
                if (palab[i - i1] != '_' && ((i == i1 && i - 1 >= 0 && t[i - 1][j1].v >= 0) || (i == (i1 + strlen(palab) - 1) && i + 1 < 15 && t[i + 1][j1].v >= 0))) return 1;
                if ((j1 + 1 < 15 && t[i][j1 + 1].v >= 0) || (j1 - 1 >= 0 && t[i][j1 - 1].v >= 0)) return 1;
                if (palab[i - i1] == '_' && t[i][j1].v >= 0) return 1;
            }


        }

        //si no se ha satisfecho ningun if de return 1, devuelve 0.
        return 0;

    }
    //si es la primera, devuelve 1 sin mas.
    return 1;

}

int chktab(ficha pos[15], char palab[15], ficha t[15][15], int i1, int j1, char hv, ficha vacio) {
    /*Comprueba que todas las palabras(entendidas como conjuntos de mas de una letra
     * en vertical u horizontal) sobre el tablero est√°n en el diccionario. De esta forma
     * se evita tener que reconocer las palabras formadas en una jugada, que ser√≠a un
     * proceso similar al que se hace en la funcion calculapuntos, pero mas complejo.
     *
     * No utiliza el tablero real, sino uno auxiliar, por si resulta que la jugada
     * no es valida, el real queda inalterado.
     *
     * Las partes comentadas sirven para mostrar por pantalla las palabras que est√°
     * comprobando. Esto es util para ver que funciona correctamente esta funcion.
     *
     */

    int i = 0, j = 0, k = 0;
    //int k2 = 0;
    ficha tab[15][15];
    ficha p[15];
    //llena el vector de fichas auxiliar p con fichas vacio.
    for (k = 0; k < 15; k++) {
        p[k] = vacio;
    }
    //crea copia del tablero actual a tablero auxiliar tab
    for (j = 0; j < 15; j++) {
        for (i = 0; i < 15; i++) {
            tab[i][j] = t[i][j];
        }
    }
    //mete la palabra en el tablero auxiliar tab, para horizontal y vertical
    if (hv == 'h') {

        for (j = j1; j < j1 + strlen(palab); j++) {
            if (pos[j - j1].c != '_' && pos[j - j1].c != '\0') tab[i1][j] = pos[j - j1];
            else if (pos[j - j1].c == '\0') break;

        }
    } else if (hv == 'v') {


        for (i = i1; i < i1 + strlen(palab); i++) {
            if (pos[i - i1].c != '_' && pos[i - i1].c != '\0') tab[i][j1] = pos[i - i1];
            else if (pos[i - i1].c == '\0') break;

        }
    }

    k = 0;
    //codigo de comprobacion durante desarrollo.
    //printf("\n\npalabras en horizontal: \n\n");

    //doble for, recorre tablero fila a fila (el vertical es identico cambiando iteradores)
    for (i = 0; i < 15; i++) {

        for (j = 0; j < 15; j++) {
            //en el if se busca un conjunto de dos o mas letras. Si la posicion actual tiene letra, y la anterior
            //o la siguiente tienen letra, esta en un conjunto de letras. sigue aÒadiendo caracteres al vector de fichas p.
            //si se ha acabado el conjunto de letras, no aÒade caracter a p, comprueba validez de conjunto actual (en el else).
            if ((tab[i][j].v >= 0 && (j + 1 < 15) && tab[i][j + 1].v >= 0) || (tab[i][j].v >= 0 && (j - 1 >= 0) && tab[i][j - 1].v >= 0)) {
                p[k] = tab[i][j];

                k++;
            } else {
                //comprobacion de que p no este vacio
                if (p[0].v >= 0) {
                    //codigo para comprobacion durante desarrollo
                    /*for (k2 = 0; k2 < 15; k2++) {
                        if (p[k2].v >= 0) {
                            printf("[%c] ", p[k2].c);
                        }
                    }
                    printf("\n");*/

                    //p es el conjunto de letras a comprobar. si el p actual no esta en diccionario, puede retornar, valor 0.
                    if (!chkdicc(p)) {
                        return 0;
                    }
                }
                //si no ha retornado, p era valido. Se vacia p, para siguiente conjunto de letras.
                for (k = 0; k < 15; k++) {
                    if (p[k].c == '\0') break;
                    p[k] = vacio;
                }
                k = 0;

            }

        }
    }
    //caso vertical, identico al horizontal cambiando iteradores.
    for (k = 0; k < 15; k++) {
        p[k] = vacio;
    }
    k = 0;
    //printf("\n\npalabras en vertical: \n\n");
    for (j = 0; j < 15; j++) {
        for (i = 0; i < 15; i++) {

            if ((tab[i][j].v >= 0 && (i + 1 < 15) && tab[i + 1][j].v >= 0) || (tab[i][j].v >= 0 && (i - 1 >= 0) && tab[i - 1][j].v >= 0)) {
                p[k] = tab[i][j];
                k++;
            } else {

                if (p[0].v >= 0) {

                    /*for (k2 = 0; k2 < 15; k2++) {
                        if (p[k2].v >= 0) {
                            printf("[%c] ", p[k2].c);
                        }
                    }
                    printf("\n");*/
                    if (!chkdicc(p)) {
                        return 0;
                    }
                }

                for (k = 0; k < 15; k++) {
                    if (p[k].c == '\0') break;
                    p[k] = vacio;
                }
                k = 0;

            }

        }
    }
    //si no ha retornado antes, es que el tablero es correcto, tiene todos sus conjuntos de letras en el diccionario
    //devuelve verdadero.
    return 1;

}

int chkprim(int primera, char palab[15], int i1, int j1, char hv) {

    /*comprueba que en la primera jugada se incluya la casilla central y
     * que es de mas de una letra
     *
     */
    int k = 0;
    if (primera) {
        //comprobacion de que esta en la fila o columna 7, las centrales. y la palabra
        //es mas de 1 caracter. else: devuelve falso.
        if ((j1 == 7 || i1 == 7) && strlen(palab) > 1) {
            //si la orientacion es vertical y esta en columna 7, mira si incluye el centro. devuelve verdadero en tal caso.
            if (j1 == 7 && hv == 'v') {
                for (k = i1; k < i1 + strlen(palab); k++) {
                    if (k == 7) {
                        return 1;
                    }
                }

            }                //idem al anterior pero en horizontal, fila 7. sino se cumple esto o lo anterior else if: devuelve falso.
            else if (i1 == 7 && hv == 'h') {
                for (k = j1; k < j1 + strlen(palab); k++) {
                    if (k == 7) {
                        return 1;
                    }
                }
            } else return 0;

        } else return 0;
        //por si se alcanza este punto, devuelve 0. Podria quitarse (se deja por evitar bugs, sabido que asi funciona)
        return 0;
    }
    return 1;
}

//funcion que comprueba si el atril a esta vacio. devuelve 1 si es asi.

int atrilvacio(ficha a[7]) {

    int i = 0;
    for (i = 0; i < 7; i++) {
        if (a[i].v >= 0) return 0;
    }
    return 1;
}
//para mostrar errores de ejecucion en modo servidor.

void error(const char *msg) {
    perror(msg);
    exit(1);
}





