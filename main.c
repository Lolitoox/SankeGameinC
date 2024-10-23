/*FUENTES UTILIZADAS PARA EL TRABAJO: Stackoverflow, repositorios de GitHub y chatGPT*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h> /*Librería necesaria para utilizar funciones de entrada/salida directo a consola
                    y lectura como getche/getch para leer caracteres sin mostrarlos en pantalla o kbhit que
                    útil para implementar bucles que no se detienen a esperar por la entrada del usuario.*/

#include <windows.h>/*Librería necesaria para el uso de la función gotoxy(controla el cursor en consola) y
                    la función srand(time(NULL)) para generar los valores aleatorios */
#include <time.h>

// Definición de variables globales/constantes.
#define ANCHO 20
#define ALTO 20
#define LONGITUD_MAX_SERPIENTE 100
#define MAX_VIDAS 3
#define ARCHIVO "scores.txt"

// Estructura de las paredes del juego.
typedef struct {
    int x;
    int y;
} Punto;

// Tamaño de serpiente.
typedef struct {
    Punto cuerpo[LONGITUD_MAX_SERPIENTE];
    int longitud;
    int direccion;
} Serpiente;

// Posición de la comida, en este caso se representa con la letra "X".
typedef struct {
    Punto posicion;
} Comida;

// Teclas de direccionamiento (W, S, D, A respectivamente). Se utiliza "enum" para definir de manera enumerada cada tecla (0, 1 , 2, 3) respectivamente.
enum Direccion { ARRIBA, ABAJO, IZQUIERDA, DERECHA };

//Funciones para iniciar correctamente el juego.
void menu();
void inicializar_juego(Serpiente *serpiente, Comida *comida);
void actualizar_serpiente(Serpiente *serpiente, Comida *comida, int *puntaje, int *juego_terminado, int *vidas_restantes);
void dibujar_juego(Serpiente *serpiente, Comida *comida, int puntaje, int vidas_restantes);
void gotoxy(int x, int y);
void guardar_puntaje(const char *nombre, const char *apellido, int puntaje);
void mostrar_puntajes();

int main() {
    menu();
    return 0;
}

// Menú de inicio. Se pide que el usuario que ingrese una opción, ingrese su nombre y apellido. Luego de eso empieza el juego.
void menu() {
    int eleccion;
    char nombre[50], apellido[50];
//Bucle do-while, para iniciar el menú y luego ejecutar el juego dependiendo de la opción colocada por el usuario.
    do {
        system("cls");
        printf("1. Empezar a jugar\n");
        printf("2. Score\n");
        printf("3. Salir\n");
        printf("Selecciona una opcion: ");
        scanf("%d", &eleccion);

        switch (eleccion) {
            case 1:
                printf("Ingresa tu nombre: ");
                scanf("%s", nombre);
                printf("Ingresa tu apellido: ");
                scanf("%s", apellido);

                Serpiente serpiente;
                Comida comida;
                int puntaje = 0, juego_terminado = 0, vidas_restantes = MAX_VIDAS;

                //Utilización de time.h que asegura que cada vez que se ejecute el programa, genere una secuencia diferente de números aleatorios.
                srand(time(NULL));
                inicializar_juego(&serpiente, &comida);

                //Se limpia la pantalla para poder iniciar el juego.
                system("cls");

                while (!juego_terminado && vidas_restantes > 0) {
                    if (_kbhit()) {
                        switch (_getch()) {
                            case 'w':
                                if (serpiente.direccion != ABAJO) serpiente.direccion = ARRIBA;
                                break;
                            case 's':
                                if (serpiente.direccion != ARRIBA) serpiente.direccion = ABAJO;
                                break;
                            case 'a':
                                if (serpiente.direccion != DERECHA) serpiente.direccion = IZQUIERDA;
                                break;
                            case 'd':
                                if (serpiente.direccion != IZQUIERDA) serpiente.direccion = DERECHA;
                                break;
                            case 'q':
                                juego_terminado = 1;
                                break;
                        }
                    }

                    actualizar_serpiente(&serpiente, &comida, &puntaje, &juego_terminado, &vidas_restantes);
                    dibujar_juego(&serpiente, &comida, puntaje, vidas_restantes);
                    Sleep(100);
                }

                if (vidas_restantes == 0) {
                    printf("Game Over! Puntaje Final: %d\n", puntaje);
                    guardar_puntaje(nombre, apellido, puntaje);
                } else {
                    printf("Juego Terminado! Puntaje Final: %d\n", puntaje);
                }

                system("pause");
                break;
            case 2:
                mostrar_puntajes();
                break;
            case 3:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion no valida\n");
                system("pause");
        }
    } while (eleccion != 3);
}

// Inicio de la serpiente (Arranca siempre hacia derecha)
void inicializar_juego(Serpiente *serpiente, Comida *comida) {
    serpiente->longitud = 1;
    serpiente->direccion = DERECHA;
    serpiente->cuerpo[0].x = ANCHO / 2;
    serpiente->cuerpo[0].y = ALTO / 2;

    comida->posicion.x = rand() % ANCHO;
    comida->posicion.y = rand() % ALTO;
}

// Función para actualizar la serpiente a medidad que "come" las "X".
void actualizar_serpiente(Serpiente *serpiente, Comida *comida, int *puntaje, int *juego_terminado, int *vidas_restantes) {
    Punto nueva_cabeza = serpiente->cuerpo[0];

    switch (serpiente->direccion) {
        case ARRIBA:
            nueva_cabeza.y--;
            break;
        case ABAJO:
            nueva_cabeza.y++;
            break;
        case IZQUIERDA:
            nueva_cabeza.x--;
            break;
        case DERECHA:
            nueva_cabeza.x++;
            break;
    }

    if (nueva_cabeza.x < 0 || nueva_cabeza.x >= ANCHO || nueva_cabeza.y < 0 || nueva_cabeza.y >= ALTO) {
        (*vidas_restantes)--;
        if (*vidas_restantes > 0) {
            inicializar_juego(serpiente, comida);
        } else {
            *juego_terminado = 1;
        }
        return;
    }

    for (int i = 0; i < serpiente->longitud; i++) {
        if (nueva_cabeza.x == serpiente->cuerpo[i].x && nueva_cabeza.y == serpiente->cuerpo[i].y) {
            (*vidas_restantes)--;
            if (*vidas_restantes > 0) {
                inicializar_juego(serpiente, comida);
            } else {
                *juego_terminado = 1;
            }
            return;
        }
    }

    for (int i = serpiente->longitud - 1; i > 0; i--) {
        serpiente->cuerpo[i] = serpiente->cuerpo[i - 1];
    }
    serpiente->cuerpo[0] = nueva_cabeza;

    if (nueva_cabeza.x == comida->posicion.x && nueva_cabeza.y == comida->posicion.y) {
        if (serpiente->longitud < LONGITUD_MAX_SERPIENTE) {
            serpiente->longitud++;
        }
        (*puntaje) += 100;
        comida->posicion.x = rand() % ANCHO;
        comida->posicion.y = rand() % ALTO;
    }
}

//Función para dibujar el juego. Representación grafica de las paredes y serpiente.
void dibujar_juego(Serpiente *serpiente, Comida *comida, int puntaje, int vidas_restantes) {
    system("cls");

    for (int y = 0; y < ALTO; y++) {
        for (int x = 0; x < ANCHO; x++) {
            int es_serpiente = 0;
            for (int k = 0; k < serpiente->longitud; k++) {
                if (serpiente->cuerpo[k].x == x && serpiente->cuerpo[k].y == y) {
                    es_serpiente = 1;
                    break;
                }
            }

            if (es_serpiente) {
                printf("O");
            } else if (comida->posicion.x == x && comida->posicion.y == y) {
                printf("X");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("Puntaje: %d\n", puntaje);
    printf("Vidas restantes: %d\n", vidas_restantes);
}

// Función para guardar el puntaje en el archivo "Score.txt"
void guardar_puntaje(const char *nombre, const char *apellido, int puntaje) {
    FILE *archivo = fopen(ARCHIVO, "a");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    fprintf(archivo, "Nombre: %s %s, Puntos: %d\n", nombre, apellido, puntaje);
    fclose(archivo);
}

// Función para abrir el archivo .txt y leer los scores
void mostrar_puntajes() {
    char ch;
    FILE *archivo = fopen(ARCHIVO, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }
    system("cls");
    printf("Scores:\n");
    while ((ch = fgetc(archivo)) != EOF) {
        putchar(ch);
    }
    fclose(archivo);
    system("pause"); //Función que detiene brevemente la ejecución del programa.
}

