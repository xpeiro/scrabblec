/*
 * File:   main.c
 * Author: Dani Peiro
 *
 * Created on 18 de octubre de 2012, 18:42
 */


/*
 *********************************************************************************************
 *PARA COMPROBAR QUE EL TABLERO EN SI FUNCIONA AL MARGEN DEL SERVIDOR, UTILIZAR main_manual.c*
 *********************************************************************************************
 *
 *PROGRAMAS PROBADOS SIEMPRE MEDIANTE TERMINAL CYGWIN. POR TANTO DEBERIAN FUNCIONAR EN UNIX O CYGWIN.
 *
 *
 *VERSION SERVIDOR: FUNCIONA DE MANERA BASICA PERO ESTA POCO PROBADA. TIENE DOS BUGS
 *CONOCIDOS:
 * 
 *BUG 1: HACE QUE ALEATORIAMENTE (MUY VARIABLE, PUEDE SER EL PRIMERO, SEGUNDO, O EL DECIMO...)
 *INTERCAMBIOS DE INFORMACION CLIENTE/SERVIDOR SE PRODUZCA UNA TRANSMISION INCOMPLETA DE DATOS 
 *DEJANDO BLOQUEADO EL CLIENTE. NO SE HA CONSEGUIDO CORREGIR. SE SUPONE QUE ES POR ERRORES DE TRANSMISION
 *PROPIOS DE LOS SOCKETS, QUE 
 * 
 * BUG 2: MAS QUE UN BUG ES UNA FUNCION INCOMPLETA. LA ASIGNACION DE LA LETRA EN
 * CASO DE USAR COMODIN/ES SE TIENE QUE HACER EN EL SERVIDOR, NO EN EL CLIENTE
 * COMO DEBERIA SER.
 
 USO:
 
 1-COMPILAR ESTE main_tablero.c Y main_jugador.c CON GCC como se indica en el README
 2- EJECUTAR EL SERVIDOR. ACEPTA ARGUMENTO: PUERTO. SINO SE INTRODUCE, USA EL PUERTO 18111 POR DEFECTO
 3- ELEGIR NUMERO DE JUGADORES.
 4- EJECUTAR TANTAS INSTANCIAS DE CLIENTES COMO JUGADORES. ACEPTA ARGUMENTOS: HOST PUERTO (SINO SE
    ESPECIFICA, UTILIZA localhost 18111 POR DEFECTO).
 
 *TODA LA PARTE RELACIONADA CON LOS SOCKETS SE HA HECHO EN BASE A ESTE TUTORIAL:
 *http://www.linuxhowtos.org/C_C++/socket.htm Y LAS TRANSPARENCIAS DE CLASE, AUNQUE
 *A DIFERENCIA DEL TUTORIAL, HEMOS UTILIZADO 5 SOCKETS EN TOTAL, EN VEZ DE CREAR PROCESOS
 *'HIJO' PARA MANEJAR LAS CONEXIONES. UN SOCKET ES DE ESCUCHA (LISTEN) Y LOS OTROS 4 SE CREAN EN FUNCION
 *DE LA CANTIDAD DE JUGADORES EN LA PARTIDA (ACCEPT) Y NO SE CIERRAN HASTA TERMINAR LA MISMA.
 *
 * -------------------TABLERO SCRABBLE - OVERVIEW-----------------------------
 *
 * El funcionamiento del tablero tiene en cuenta todas las reglas del scrabble
 * tradicional en inglés y comprueba que las jugadas que se realizan cumplen con
 * todas ellas mediante diferentes funciones y bucles.
 * Comprueba:
 *
 * En la primera jugada, se incluye la casilla central y la palabra es de mas de una
 * letra.
 *
 * En jugadas siguientes:
 *
 * Comprueba que no se solapan fichas y que la palabra a introducir conecta con las
 * existentes en al menos un puntos
 *
 * Comprueba que tras cada jugada, todas las palabras del tablero estan en el diccionario
 *
 *
 * Ademas de esto, lleva la cuenta de (y asigna) los puntos, los atriles con sus letras,
 * (llenados por el tablero), la bolsa y su contenido y las condiciones de final de partida.
 *
 *
 *
 *
 *
 * EL PROGRAMA SE REALIZO INICIALMENTE CON LA IDEA DE QUE LO USARIA UN COMPUTADOR
 * Y NO UN JUGADOR HUMANO. POR ESTO NO ES  'FOOL-PROOF', (EXCEPTO ALGUNOS
 * SALVAGUARDAS BASICOS). POR EJEMPLO, SI SE INTRODUCEN CHARS CUANDO EL PROGRAMA
 * ESPERA INTS, ES PROBABLE QUE ENTRE EN BUCLE INFINITO O SE BLOQUEE, NO RESPETAR
 * TAMANYO DE BUFFERS BLOQUEARA EL PROGRAMA ETC.
 *
 *TO-DO:
 *
 *
 *ARREGLAR PROBLEMAS CON SOCKETS EN MODO SERVIDOR/CLIENTE.
 *FINALIZAR SACAR() DE LA VERSION SERVIDOR PARA QUE LOS COMODINES SE
 *ASIGNEN DESDE EL PROGRAMA CLIENTE.
 *
 *
 *
 * NOTA: Existen trozos de codigo comentados para evitar su compilacion. Es codigo
 * que en el desarrollo del programa resultaron utiles para buscar fallos, y que se
 * han mantenido para comprobaciones futuras si fueran necesarias.
 */




/*nota sobre comentarios explicativos:

se explica el mismo esquema de funcionamiento PASO A PASO EN MAIN_MANUAL.C 

al no tener todo el codigo de las comunicaciones por sockets de por medio, se ve mas claro el funcionamiento.

 */







/*EXPLICACION DE TRANSMISION DE DATOS POR SOCKETS.


APARTE DE ABRIR Y ESCUCHAR CON SOCKETS, QUE SE HA HECHO SIGUIENDO EL TUTORIAL INDICADO MAS ARRIBA,
PARA EVITAR TENER QUE SERIALIZAR DATOS EN UNA APLICACION SIN DATOS MAS ALLA DE STRINGS (O ENTEROS CONVERTIDOS DESDE
BUFFERS) SE OPTA POR USAR SIMPLEMENTE LA FUNCION SPRINTF, QUE IMPRIME CON FORMATO COMO HARIA PRINTF, PERO A UN VECTOR DE CHARS:
UN STRING. POR TANTO, CAMBIANDO TODAS LAS INSTANCIAS DE PRINTF EN EL PROGRAMA MANUAL POR EL SPRINTF CORRESPONDIENTE Y ENVIANDO
ESTE STRING POR SOCKETS ENTRE SERVIDOR DEBERIA FUNCIONAR. PARA LECTURAS DE VARIOS STRINGS, SE UTILIZAN BUCLES QUE LEEN HASTA
QUE RECIBEN UN STRING DE UN SOLO CARACTER '>' QUE INDICA QUE HA ACABADO LA TRANSMISION. PARA RESETEAR LOS BUFFERS DE LECTURA Y
ESCRITURA, SE USA UN BUCLE FOR QUE RECORRE EL STRING ASIGNANDO '\0'

NO SE HA CONSEGUIDO AVERIGUAR CUAL ES LA CAUSA DE LOS FALLOS EN LA COMUNICACION POR SOCKETS. TEORICAMENTE DEBERIA FUNCIONAR
Y AL SER EL ERROR SEMI ALEATORIO (PASA EN DISTINTOS MOMENTOS HACIENDO LAS MISMAS ACCIONES, INCLUSO USANDO LAS MISMAS LETRAS EXACTAS)
NO SE HA CONSEGUIDO ELIMINAR EL BUG. EL TABLERO EN SI, FUNCIONA BIEN, COMO SE PUEDE COMPROBAR CON MAIN_MANUAL.C

UNA POSIBLE FUENTE DEL ERROR PUEDE SER LA FALTA DE DESCRIPTORES (HEADERS) QUE INDIQUEN LA LONGITUD DE LOS DATOS QUE SE VAN A ENVIAR
Y LA LECTURA SE QUEDE A MEDIAS (NO EXPLICA PORQUE PASA DE FORMA ALEATORIA, YA QUE LOS DATOS SON ESENCIALMENTE DE LA MISMA LONGITUD)

OTRA ES POR PROBLEMAS DE LATENCIA: 
https://access.redhat.com/knowledge/docs/en-US/Red_Hat_Enterprise_MRG/1.1/html/Realtime_Tuning_Guide/sect-Realtime_Tuning_Guide-Application_Tuning_and_Deployment-TCP_NODELAY_and_Small_Buffer_Writes.html
A LO MEJOR UTILIZANDO TCP_NODELAY EN LOS SOCKETS ABIERTOS SE PODRIA ARREGLAR??
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "func/chkfunc.c"
#include "func/playfunc.c"

int main(int argc, char *argv[]) {

    int i = 0, njugador = 0, nmaxjug = 0, primera = 1, sel = 0, pases = 0, k = 0;
    int sockfd, newsockfd1, newsockfd2, newsockfd3, newsockfd4, portno;
    socklen_t clilen;
    char str[256];
    struct sockaddr_in serv_addr, cli_addr;
    ficha tab[15][15];
    ficha bolsa[101], vacio = {.c = '\0', .v = -1};
    ficha atril1[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril2[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril3[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha atril4[7] = {vacio, vacio, vacio, vacio, vacio, vacio, vacio};
    ficha (*p)[7];
    int punt[4] = {0, 0, 0, 0};
    char cambio[7];

    //si no hay argumento de llamada, asigna el puerto al 18111 por defecto.
    if (argc < 2) {
        printf("\n\nNO SE ASIGNO PUERTO. SE USARA EL 18111 (POR DEFECTO) ");
        portno = 18111;
    } else portno = atoi(argv[1]);

    //abre socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("Error al abrir Socket");

    bzero((char *) &serv_addr, sizeof (serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0) error("Error al unir socket a direccion");
    listen(sockfd, 5);
    clilen = sizeof (cli_addr);

    //obtiene por pantalla el numero de jugadores que se van a conectar
    while (nmaxjug <= 0 || nmaxjug > 4) {
        printf("\n\nINTRODUZCA NUMERO DE JUGADORES: ");
        scanf("%i", &nmaxjug);
        fflush(stdin);
    }
    printf("\n\nESPERANDO CONEXION DE %i JUGADOR/ES...\n\n", nmaxjug);

    //acepta (y por tanto crea un nuevo socket) cada conexion, segun el numero de jugadores
    newsockfd1 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd1 < 0) error("ERROR al aceptar conexion");
    asignaj(newsockfd1, 1);
    if (nmaxjug > 1) {
        newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd2 < 0) error("ERROR al aceptar conexion");
        asignaj(newsockfd2, 2);
    }
    if (nmaxjug > 2) {
        newsockfd3 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd3 < 0) error("ERROR al aceptar conexion");
        asignaj(newsockfd3, 3);
    }
    if (nmaxjug > 3) {
        newsockfd4 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd4 < 0) error("ERROR al aceptar conexion");
        asignaj(newsockfd4, 4);
    }

    //crea el tablero, llena la bolsa, y llena los atriles que correspondan.
    crea(tab);
    llenab(bolsa);
    llenaat(bolsa, atril1, vacio);
    if (nmaxjug > 1) llenaat(bolsa, atril2, vacio);
    if (nmaxjug > 2) llenaat(bolsa, atril3, vacio);
    if (nmaxjug > 3) llenaat(bolsa, atril4, vacio);


    //bucle principal del programa

    // Para solucionar el problema de repeticion
    // de codigo que habia en la primera entrega, se ha creado un puntero a ficha[7] para pasar el atril
    // correspondiente a cada jugador. Asi, se reduce 4 veces el codigo resultante.
    do {
        //str es el string que se va a usar para enviar/recibir chars por los sockets. se inicializa a nulo.

        //asignacion de turnos en cada ciclo de bucle.
        if (njugador < nmaxjug) njugador++;
        else njugador = 1;

        //p apunta al atril correspondiente a cada turno.
        if (njugador == 1) p = &atril1;
        if (njugador == 2) p = &atril2;
        if (njugador == 3) p = &atril3;
        if (njugador == 4) p = &atril4;
        // siempre que haya que pasar el atril del jugador como argumento, se pasa *p,contenido de p.


        //el resto del programa es identico a lo explicado en main_manual.c sin el codigo de comunicaciones.
        //se explica el flujo del programa en ese archivo, por ser mas claro. Aqui solo se comentan las funciones
        //relativas a la comunicacion (se comentan solo en la jugada tipo 1, por ser siempre el mismo codigo
        //practicamente). Una explicacion teorica se encuentra al principio de este archivo.
        //Este codigo deberia funcionar, pero como se ha dicho antes, tiene los dos bugs explicados.
        //Ademas, es ineficiente por repetirse en exceso. Para comprobar que solo es problema de las comunicaciones
        //y que el programa (Manejo de jugadas, tablero, jugadores y diccionario) funcionan sin problema, se
        //adjunta main_manual.c, el mismo juego (mismas funciones, misma estructura, todo igual), pero sin servidor/cliente.

        do {
            ver(tab);
            llenaat(bolsa, *p, vacio);
            //anula el vector de chars str
            for (k = 0; k < 256; k++) str[k] = '\0';

            //ver tablero en el cliente (enviado por sockets)
            verclient(tab, newsockfd1);
            for (k = 0; k < 256; k++) str[k] = '\0';
            //escribe a str como si fuera printf
            sprintf(str, "\n\n sus letras: \n\n");
            //escribe-envia str por el socket correspondiente al cliente 1
            write(newsockfd1, str, strlen(str));
            for (k = 0; k < 256; k++) str[k] = '\0';
            //escribe-envia str por el socket correspondiente al cliente 1 el contenido del atril
            for (i = 0; i < 7; i++) {
                sprintf(str, "[%c] ", (*p)[i].c);
                write(newsockfd1, str, strlen(str));
            }

            for (k = 0; k < 256; k++) str[k] = '\0';

            sprintf(str, "\n\nElija lo que quiere hacer JUGADOR %i: \n1- Jugar Palabra\n", njugador);
            write(newsockfd1, str, strlen(str));
            for (k = 0; k < 256; k++) str[k] = '\0';

            sprintf(str, "2- Pasar\n3-Cambiar fichas\n\nSeleccion:");
            write(newsockfd1, str, strlen(str));
            for (k = 0; k < 256; k++) str[k] = '\0';
            //como tiene que recibir por el socket, manda '>' para que el cliente sepa que ha terminado
            //el envio. despues, lee del socket el string str y lo transforma a entero (en este momento
            //str es la seleccion de tipo de jugada)
            str[0] = '>';
            write(newsockfd1, str, strlen(str));
            for (k = 0; k < 256; k++) str[k] = '\0';
            read(newsockfd1, str, 1);
            sel = atoi(str);
        } while (!(sel == 1 || sel == 2 || sel == 3));


        switch (sel) {
            case 1:
            {
                pases = 0;
                jugada(tab, bolsa, punt, *p, vacio, primera, njugador, newsockfd1);
                if (primera) primera = 0;
                llenaat(bolsa, *p, vacio);
            }
                break;
            case 2:
            {
                pases++;


            }
                break;
            case 3:
            {
                pases = 0;
                for (k = 0; k < 256; k++) str[k] = '\0';
                sprintf(str, "\n\n sus letras: \n\n");
                write(newsockfd1, str, strlen(str));
                for (k = 0; k < 256; k++) str[k] = '\0';
                for (i = 0; i < 7; i++) {
                    sprintf(str, "[%c] ", (*p)[i].c);
                    write(newsockfd1, str, strlen(str));
                    for (k = 0; k < 256; k++) str[k] = '\0';
                }
                sprintf(str, "\n\nLetras que quiere devolver a la bolsa:\n", njugador);
                write(newsockfd1, str, strlen(str));
                for (k = 0; k < 256; k++) str[k] = '\0';
                str[0] = '>';
                write(newsockfd1, str, strlen(str));
                for (k = 0; k < 256; k++) str[k] = '\0';
                read(newsockfd1, str, 7);
                sprintf(cambio, "%s", str);
                cambiar(*p, bolsa, cambio, vacio);
                llenaat(bolsa, *p, vacio);
                for (k = 0; k < 256; k++) str[k] = '\0';
                sprintf(str, "\n\n sus letras: \n\n");
                write(newsockfd1, str, strlen(str));
                for (k = 0; k < 256; k++) str[k] = '\0';
                for (i = 0; i < 7; i++) {
                    sprintf(str, "[%c] ", (*p)[i].c);
                    write(newsockfd1, str, strlen(str));
                    for (k = 0; k < 256; k++) str[k] = '\0';
                }
                for (k = 0; k < 256; k++) str[k] = '\0';
                str[0] = '>';
                write(newsockfd1, str, strlen(str));
            }
                break;

        }






        printf("Sacadas de Bolsa=%i    PASES=%i", bolsa[100].v, pases);
    } while (!atrilvacio(atril1) && (!atrilvacio(atril2) || nmaxjug < 2) && (!atrilvacio(atril3) || nmaxjug < 3) && (!atrilvacio(atril4) || nmaxjug < 4) && pases < nmaxjug * 2);
    for (k = 0; k < 256; k++) str[k] = '\0';
    str[0] = '>';
    write(newsockfd1, str, strlen(str));
    if (nmaxjug > 1) write(newsockfd2, str, strlen(str));
    if (nmaxjug > 2) write(newsockfd3, str, strlen(str));
    if (nmaxjug > 3) write(newsockfd4, str, strlen(str));


    if (pases < nmaxjug * 2) {
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
    for (k = 0; k < 256; k++) str[k] = '\0';

    sprintf(str, "\n\n\nPuntos Finales:\n\nJugador 1: %i\n\nJugador 2: %i \n\nJugador 3: %i\n\nJugador 4: %i", punt[0], punt[1], punt[2], punt[3]);
    write(newsockfd1, str, strlen(str));
    if (nmaxjug > 1) write(newsockfd2, str, strlen(str));
    if (nmaxjug > 2) write(newsockfd3, str, strlen(str));
    if (nmaxjug > 3) write(newsockfd4, str, strlen(str));

    for (k = 0; k < 256; k++) str[k] = '\0';
    str[0] = '>';
    write(newsockfd1, str, strlen(str));
    if (nmaxjug > 1) write(newsockfd2, str, strlen(str));
    if (nmaxjug > 2) write(newsockfd3, str, strlen(str));
    if (nmaxjug > 3) write(newsockfd4, str, strlen(str));
    return 0;
}

