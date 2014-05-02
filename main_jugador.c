
/*LEER DOCUMENTACION EN main_tablero.c*/



/* TRANSMISION DE DATOS POR SOCKETS.


APARTE DE ABRIR Y ESCUCHAR CON SOCKETS, QUE SE HA HECHO SIGUIENDO EL TUTORIAL INDICADO EN EL MAIN,
PARA EVITAR TENER QUE SERIALIZAR DATOS EN UNA APLICACION SIN DATOS MAS ALLA DE STRINGS (O ENTEROS CONVERTIDOS DESDE
BUFFERS) SE OPTA POR USAR SIMPLEMENTE LA FUNCION SPRINTF, QUE IMPRIME CON FORMATO COMO HARIA PRINTF, PERO A UN VECTOR DE CHARS:
UN STRING. POR TANTO, CAMBIANDO TODAS LAS INSTANCIAS DE PRINTF EN EL PROGRAMA MANUAL POR EL SPRINTF CORRESPONDIENTE Y ENVIANDO
ESTE STRING POR SOCKETS ENTRE SERVIDOR DEBERIA FUNCIONAR. PARA LECTURAS DE VARIOS STRINGS, SE UTILIZAN BUCLES QUE LEEN HASTA
QUE RECIBEN UN STRING DE UN SOLO CARACTER '>' QUE INDICA QUE HA ACABADO LA TRANSMISION. PARA RESETEAR LOS BUFFERS DE LECTURA Y
ESCRITURA, SE USA UN BUCLE FOR QUE RECORRE EL STRING ASIGNANDO '\0' */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]) {
    int sockfd, portno, n, njugador, sel = 0, i1 = 0, j1 = 0, i = 0;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256], cambio[7], palab[15], hv, comod;

    //comprueba argumentos y si no estan ambos, asigna por defecto.
    if (argc < 3) {
        printf("\n\nNO SE ASIGNO HOST Y/O PUERTO. SE USARA localhost y PUERTO 18111 (POR DEFECTO)\n\n");
        portno = 18111;
        server = gethostbyname("localhost");
    } else {
        portno = atoi(argv[2]);
        server = gethostbyname(argv[1]);
    }
    //abre socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof (serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server->h_addr,
            (char *) &serv_addr.sin_addr.s_addr,
            server->h_length);
    serv_addr.sin_port = htons(portno);
    //conecta al socket abierto por el servidor
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof (serv_addr)) < 0)
        error("ERROR connecting");

    for (i = 0; i < 256; i++) buffer[i] = '\0';
    //envia 'a' al servidor. Esto lo interpreta la funcion asignaj del servidor como senyal de 'cliente listo'
    //y le asigna un numero de jugador (por orden de conexion).
    buffer[0] = 'a';
    n = write(sockfd, buffer, 1);
    if (n < 0)
        error("ERROR writing to socket");
    for (i = 0; i < 256; i++) buffer[i] = '\0';

    n = read(sockfd, buffer, 256);
    if (n < 0)
        error("ERROR reading from socket");
    printf("%s\n", buffer);


    do {
        //como se explica en main_tablero.c lee de forma continua en bucles desde su socket
        //hasta que recibe un string de solo '>' en [0]. El cliente lo interpreta como senyal de
        //'avanzar', y en caso de tener que escribir algo al servidor, hace lo mismo (escribir y
        //enviar senyal '>' para siguente paso).

        do {
            do {

                for (i = 0; i < 256; i++) buffer[i] = '\0';

                read(sockfd, buffer, 100);
                if (buffer[0] != '>') printf("%s", buffer);
            } while (buffer[0] != '>');

            scanf("%i", &sel);
            fflush(stdin);
            for (i = 0; i < 256; i++) buffer[i] = '\0';
            sprintf(buffer, "%i", sel);
            write(sockfd, buffer, 1);
        } while (!(sel == 1 || sel == 2 || sel == 3));

        switch (sel) {
            case 1:
            {
                do {//diccionario
                    do {//alineadas y primera
                        do {//escribirpal
                            do {

                                for (i = 0; i < 256; i++) buffer[i] = '\0';

                                read(sockfd, buffer, 255);
                                if (buffer[0] != '>') printf("%s", buffer);
                            } while (buffer[0] != '>');
                            scanf("%s", palab);
                            fflush(stdin);
                            for (i = 0; i < 256; i++) buffer[i] = '\0';
                            sprintf(buffer, "%s", palab);
                            write(sockfd, buffer, strlen(buffer));
                            for (i = 0; i < 256; i++) buffer[i] = '\0';

                            read(sockfd, buffer, 255);
                        } while (buffer[0] != '>');
                        do {//coordenadas
                            do {

                                for (i = 0; i < 256; i++) buffer[i] = '\0';

                                read(sockfd, buffer, 255);
                                if (buffer[0] != '>') printf("%s", buffer);
                            } while (buffer[0] != '>');
                            scanf("%i %i %c", &i1, &j1, &hv);
                            fflush(stdin);
                            for (i = 0; i < 256; i++) buffer[i] = '\0';
                            sprintf(buffer, "%i", i1);
                            write(sockfd, buffer, strlen(buffer));
                            for (i = 0; i < 256; i++) buffer[i] = '\0';
                            sprintf(buffer, "%i", j1);
                            write(sockfd, buffer, strlen(buffer));
                            for (i = 0; i < 256; i++) buffer[i] = '\0';
                            sprintf(buffer, "%c", hv);
                            write(sockfd, buffer, strlen(buffer));

                            for (i = 0; i < 256; i++) buffer[i] = '\0';

                            read(sockfd, buffer, 255);
                        } while (buffer[0] != '>');
                        //CODIGO INCOMPLETO PARA ASIGNAR LETRA A COMODINES
                        /*do {
                            do {

                                 for (i = 0; i < 256; i++) buffer[i] = '\0';
                               
                                read(sockfd, buffer, 255);
                                if (buffer[0] != '>') printf("%s", buffer);
                            } while (buffer[0] != '>');
                            if (buffer[1] == '?') {
                                while (getchar() != '\n');
                                scanf("%c", &comod);
                                fflush(stdin);
                                 for (i = 0; i < 256; i++) buffer[i] = '\0';
                                sprintf(buffer, "%c", comod);
                                write(sockfd, buffer, strlen(buffer));
                                 for (i = 0; i < 256; i++) buffer[i] = '\0';
                            }
                        } while (!(buffer[0] == '>' && buffer[1] == '-'));*/

                        do {

                            for (i = 0; i < 256; i++) buffer[i] = '\0';

                            read(sockfd, buffer, 255);
                            if (buffer[0] != '>') printf("%s", buffer);
                        } while (buffer[0] != '>');
                        for (i = 0; i < 256; i++) buffer[i] = '\0';
                        read(sockfd, buffer, 255);
                    } while (buffer[0] != '>');
                    do {

                        for (i = 0; i < 256; i++) buffer[i] = '\0';

                        read(sockfd, buffer, 255);
                        if (buffer[0] != '>') printf("%s", buffer);
                    } while (buffer[0] != '>');
                    for (i = 0; i < 256; i++) buffer[i] = '\0';
                    read(sockfd, buffer, 255);
                } while (buffer[0] != '>');
                do {

                    for (i = 0; i < 256; i++) buffer[i] = '\0';

                    read(sockfd, buffer, 255);
                    if (buffer[0] != '>') printf("%s", buffer);
                } while (buffer[0] != '>');

            }
                break;
            case 2:
            {


            }
                break;
            case 3:
            {

                do {

                    for (i = 0; i < 256; i++) buffer[i] = '\0';

                    read(sockfd, buffer, 255);
                    if (buffer[0] != '>') printf("%s", buffer);
                } while (buffer[0] != '>');
                scanf("%s", cambio);
                fflush(stdin);
                for (i = 0; i < 256; i++) buffer[i] = '\0';
                sprintf(buffer, "%s", cambio);
                write(sockfd, buffer, strlen(buffer));
                do {

                    for (i = 0; i < 256; i++) buffer[i] = '\0';

                    read(sockfd, buffer, 255);
                    if (buffer[0] != '>') printf("%s", buffer);
                } while (buffer[0] != '>');
            }
                break;

        }
        for (i = 0; i < 256; i++) buffer[i] = '\0';
        read(sockfd, buffer, 255);
    } while (buffer[0] != '>');
    do {

        for (i = 0; i < 256; i++) buffer[i] = '\0';

        read(sockfd, buffer, 255);
        if (buffer[0] != '>') printf("%s", buffer);
    } while (buffer[0] != '>');

    return 0;
}
