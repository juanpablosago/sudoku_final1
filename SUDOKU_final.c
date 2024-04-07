#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DIMENSION 9

typedef struct SDK {
    int matrizInicial[DIMENSION][DIMENSION];
    int matrizJuego[DIMENSION][DIMENSION];
    int numModificaciones;
    char nombreJugador[100];
} Sudoku;

const int matrizInicialPredeterminada[DIMENSION][DIMENSION] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
};



void iniciarJuego(Sudoku* juego) {

    memcpy(juego->matrizInicial, matrizInicialPredeterminada, sizeof(juego->matrizInicial));
    memcpy(juego->matrizJuego, matrizInicialPredeterminada, sizeof(juego->matrizJuego));
    juego->numModificaciones = 0;
   
    printf("Introduzca el nombre del jugador:\n");
    fgets(juego->nombreJugador, sizeof(juego->nombreJugador), stdin);
    juego->nombreJugador[strcspn(juego->nombreJugador, "\n")] = 0;
}



void imprimirEstadoActual(const Sudoku* juego) {

    for (int i = 0; i < DIMENSION; i++) {       
        for (int j = 0; j < DIMENSION; j++) {                                        
            printf("%4d ", juego->matrizJuego[i][j]);
        }
        printf("\n");
    }
}



bool esValido(Sudoku* juego, int fila, int columna, int valor) {

    // Verificar fila y columna
    for (int i = 0; i < DIMENSION; i++) {
        if (juego->matrizJuego[fila][i] == valor || juego->matrizJuego[i][columna] == valor) {
            return false;
        }
    }

    // Verificar el subcuadrante de 3x3
    int inicioFila = fila - fila % 3, inicioColumna = columna - columna % 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (juego->matrizJuego[i + inicioFila][j + inicioColumna] == valor) {
                return false;
            }
        }
    }

    return true;
}



void imprimirEstadoInicial(const Sudoku* juego) {
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            printf("%4d ", juego->matrizInicial[i][j]);
        }
        printf("\n");
    }
}


bool cambiarValor(Sudoku* juego, int fila, int columna, int valor) {
    // Verifica que la fila y columna estén dentro del rango permitido
    if (fila < 0 || fila >= DIMENSION || columna < 0 || columna >= DIMENSION) {
        printf("Posición fuera de rango.\n");
        return false;
    }

    // Verifica que la posición no corresponda a un valor fijo de la matriz inicial
    if (juego->matrizInicial[fila][columna] != 0) {
        printf("Posición no modificable. Corresponde a un valor del juego inicial.\n");
        return false;
    }

    // Verifica que el valor esté dentro del rango permitido
    if (valor < 1 || valor > 9) {
        printf("Valor fuera de rango.\n");
        return false;
    }

    // Verifica si el valor es válido según las reglas del Sudoku
    if (!esValido(juego, fila, columna, valor)) {
        printf("Movimiento no válido. El número ya existe en la fila, columna o subcuadrante.\n");
        return false;
    }

    // Cambia el valor en la celda especificada
    juego->matrizJuego[fila][columna] = valor;
    juego->numModificaciones++;
    return true;
}



bool borrarValor(Sudoku* juego, int fila, int columna) {
    // Verifica que la fila y columna estén dentro del rango permitido
    if (fila < 0 || fila >= DIMENSION || columna < 0 || columna >= DIMENSION) {
        printf("Posición fuera de rango.\n");
        return false;
    }

    // Verifica que la posición no corresponda a un valor fijo de la matriz inicial
    if (juego->matrizInicial[fila][columna] != 0) {
        printf("No se puede borrar un valor del juego incial.\n");
        return false;
    }

    // Verifica si la celda ya está vacía
    if (juego->matrizJuego[fila][columna] == 0) {
        printf("La celda está vacía.\n");
        return false;
    }

    // Borra el valor en la celda especificada
    juego->matrizJuego[fila][columna] = 0;
    juego->numModificaciones++;
    return true;
}



bool ingresarValor(Sudoku* juego, int fila, int columna, int valor) {
    if (fila < 0 || fila >= DIMENSION || columna < 0 || columna >= DIMENSION || juego->matrizInicial[fila][columna] != 0) {
        printf("Posición no modificable.\n");
        return false;
    }

    if (valor < 1 || valor > 9) {
        printf("Valor fuera de rango.\n");
        return false;
    }

    if (!esValido(juego, fila, columna, valor)) {
        printf("Movimiento no válido. El número ya existe en la fila, columna o subcuadrante.\n");
        return false;
    }

    juego->matrizJuego[fila][columna] = valor;
    juego->numModificaciones++;
    return true;
}



bool esSudokuResuelto(Sudoku* juego) {
    for (int fila = 0; fila < DIMENSION; fila++) {
        for (int columna = 0; columna < DIMENSION; columna++) {
            // Verifica si hay casillas vacías
            if (juego->matrizJuego[fila][columna] == 0) {
                return false;
            }
            // Verifica si el número cumple con las reglas del Sudoku
            int valor = juego->matrizJuego[fila][columna];
            juego->matrizJuego[fila][columna] = 0; // Temporalmente elimina el valor para la validación
            bool valido = esValido(juego, fila, columna, valor);
            juego->matrizJuego[fila][columna] = valor; // Restaura el valor
            if (!valido) {
                return false;
            }
        }
    }
    return true;
}



void rotarMatriz90Grados(Sudoku* juego) {
    int temp[DIMENSION][DIMENSION];

    //Función para rotar la matriz
    for (int fila = 0; fila < DIMENSION; fila++) {
        for (int columna = 0; columna < DIMENSION; columna++) {
            temp[fila][columna] = juego->matrizJuego[columna][fila];
        }
    }

    //Función para  invertir cada fila para obtener la rotación de 90 grados
    for (int fila = 0; fila < DIMENSION; fila++) {
        for (int columna = 0; columna < DIMENSION; columna++) {
            juego->matrizJuego[fila][columna] = temp[fila][DIMENSION - 1 - columna];
        }
    }
}



void intercambiarNumeros(Sudoku* juego) {
    int i, j, temp, valores[9], mapeo[9];
    srand((unsigned int)time(NULL)); // Inicialización de la semilla aleatoria

    // Inicializar los valores de 1 a 9
    for (i = 0; i < 9; i++) {
        valores[i] = i + 1;
    }

    // Generar un mapeo aleatorio de números
    for (i = 8; i > 0; i--) {
        j = rand() % (i + 1);
        temp = valores[i];
        valores[i] = valores[j];
        valores[j] = temp;
    }

    // Crear el mapeo para reemplazar los números según el orden aleatorio generado
    for (i = 0; i < 9; i++) {
        mapeo[valores[i] - 1] = i + 1;
    }

    // Aplicar el mapeo al juego actual
    for (i = 0; i < DIMENSION; i++) {
        for (j = 0; j < DIMENSION; j++) {
            if (juego->matrizJuego[i][j] != 0) { // Asegurarse de no mapear celdas vacías
                juego->matrizJuego[i][j] = mapeo[juego->matrizJuego[i][j] - 1];
            }
        }
    }

    // Opcional: puedes querer imprimir el estado del juego después de intercambiar los números
    imprimirEstadoActual(juego);
}



void intercambiarEnBloque(Sudoku* juego, bool esFila, int bloque, int posicion1, int posicion2) {
    int inicioBloque = bloque * 3; // Calcula el índice inicial del bloque
    int temp;

    // Verifica que las posiciones estén dentro de los límites del bloque
    if (posicion1 < 0 || posicion1 > 2 || posicion2 < 0 || posicion2 > 2) {
        printf("Posiciones fuera de rango. Deben estar entre 0 y 2.\n");
        return;
    }

    if (esFila) {
        // Intercambiar filas dentro del mismo bloque
        for (int i = 0; i < DIMENSION; i++) {
            temp = juego->matrizJuego[inicioBloque + posicion1][i];
            juego->matrizJuego[inicioBloque + posicion1][i] = juego->matrizJuego[inicioBloque + posicion2][i];
            juego->matrizJuego[inicioBloque + posicion2][i] = temp;
        }
    } else {
        // Intercambiar columnas dentro del mismo bloque
        for (int i = 0; i < DIMENSION; i++) {
            temp = juego->matrizJuego[i][inicioBloque + posicion1];
            juego->matrizJuego[i][inicioBloque + posicion1] = juego->matrizJuego[i][inicioBloque + posicion2];
            juego->matrizJuego[i][inicioBloque + posicion2] = temp;
        }
    }
}



void guardarJugadas(const Sudoku* juego) {
    FILE *archivo;
    char nombreArchivo[100];
    sprintf(nombreArchivo, "%s.txt", juego->nombreJugador); // Construye el nombre del archivo usando el nombre del jugador
    archivo = fopen(nombreArchivo, "a"); // Abre el archivo en modo "append" para agregar al final
    
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar jugadas.\n");
        return;
    }

    fprintf(archivo, "Estado actual del juego:\n");
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            fprintf(archivo, "%2d ", juego->matrizJuego[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fprintf(archivo, "Número de modificaciones: %d\n", juego->numModificaciones);
    fprintf(archivo, "\n");

    fclose(archivo);
}

void guardarJuego(const Sudoku* juego) {
    FILE *archivo;
    char nombreArchivo[100];
    sprintf(nombreArchivo, "%s.txt", juego->nombreJugador);
    archivo = fopen(nombreArchivo, "w"); // Abre el archivo en modo "write" para escribir desde el principio
    
    if (archivo == NULL) {
        printf("Error al abrir el archivo para guardar el juego.\n");
        return;
    }

    fprintf(archivo, "Matriz inicial del juego:\n");
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            fprintf(archivo, "%2d ", juego->matrizInicial[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fprintf(archivo, "\n");

    fprintf(archivo, "Matriz actual del juego:\n");
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            fprintf(archivo, "%2d ", juego->matrizJuego[i][j]);
        }
        fprintf(archivo, "\n");
    }
    fprintf(archivo, "Número de modificaciones: %d\n", juego->numModificaciones);
    fprintf(archivo, "\n");

    fclose(archivo);
}

void cargarJuego(Sudoku* juego) {
    FILE *archivo;
    char nombreArchivo[100];
    sprintf(nombreArchivo, "%s.txt", juego->nombreJugador);
    archivo = fopen(nombreArchivo, "r"); // Abre el archivo en modo "read" para leer
    
    if (archivo == NULL) {
        printf("Error al abrir el archivo para cargar el juego.\n");
        return;
    }

    printf("Cargando juego...\n");

    // Leer la matriz inicial del juego
    printf("Matriz inicial del juego:\n");
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            fscanf(archivo, "%d", &juego->matrizInicial[i][j]);
            printf("%2d ", juego->matrizInicial[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Leer la matriz actual del juego
    printf("Matriz actual del juego:\n");
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            fscanf(archivo, "%d", &juego->matrizJuego[i][j]);
            printf("%2d ", juego->matrizJuego[i][j]);
        }
        printf("\n");
    }

    // Leer el número de modificaciones
    fscanf(archivo, "%d", &juego->numModificaciones);
    printf("Número de modificaciones: %d\n", juego->numModificaciones);

    fclose(archivo);
}








int main() {
    Sudoku juego;
    iniciarJuego(&juego);
    
    int opcion;
    int fila, columna, valor;

    do {
        printf("\nJugador: %s\n", juego.nombreJugador);
        printf("\t Ingresa una opción para iniciar tu juego: \n");
        printf("1. Imprimir estado actual del juego y ver el número de modificaciones\n");
        printf("2. Ingresar un valor\n");
        printf("3. Imprimir el tablero inicial\n");
        printf("4. Cambiar un valor\n");
        printf("5. Borrar un valor\n");
        printf("\t Ingresa una opción para crear juegos:\n");
        printf("6. Rotar tablero 90°\n");
        printf("7. Intercambiar números\n");
        printf("8. Mezclar renglones y columnas\n");
        printf("9. Guardar juego\n");
        printf("10. Cargar juego\n");
        printf("11. Salir\n");
        printf("\nSeleccione una opción: ");
        scanf("%d", &opcion);

        switch(opcion) {
            case 1:
                imprimirEstadoActual(&juego);
                printf("Movimientos realizados: %d\n", juego.numModificaciones);
                break;
            


            case 2:
                printf("Ingrese fila, columna y valor separados por comas (ej. 3,2,5): ");
                scanf("%d,%d,%d", &fila, &columna, &valor);
            if (ingresarValor(&juego, fila - 1, columna - 1, valor)) {
                printf("Valor ingresado correctamente.\n");
        
            if (esSudokuResuelto(&juego)) {
                printf("¡Felicidades, %s! Has completado el Sudoku correctamente.\n", juego.nombreJugador);
                opcion = 11; 
        }
        } else {
        printf("No se pudo ingresar el valor.\n");
     }
                break;



            case 3:
                    imprimirEstadoInicial(&juego);
                break;



            case 4:
                printf("Ingrese fila, columna y el nuevo valor separados por comas (ej. 3,2,5): ");
                scanf("%d,%d,%d", &fila, &columna, &valor);
                if (cambiarValor(&juego, fila - 1, columna - 1, valor)) {
                    printf("Valor cambiado correctamente.\n");
                } else {
                    printf("No se pudo cambiar el valor.\n");
                }
                break;



            case 5:
                printf("Ingrese fila y columna separados por comas para borrar el valor (ej. 3,2): ");
                scanf("%d,%d", &fila, &columna);
                if (borrarValor(&juego, fila - 1, columna - 1)) {
                    printf("Valor borrado correctamente.\n");
                } else {
                    printf("No se pudo borrar el valor.\n");
                }
                break;



            case 6:
                    rotarMatriz90Grados(&juego); // Llamada a la función para rotar la matriz
                    printf("Matriz rotada 90 grados:\n");
                    imprimirEstadoActual(&juego); // Imprimir el estado actual para ver los cambios
                break;



            case 7:
                    intercambiarNumeros(&juego);
                    printf("Los números han sido intercambiados aleatoriamente.\n");
                break;



            case 8: {
                bool esFila;
                int bloque, posicion1, posicion2;
                char tipoIntercambio;

                printf("Intercambiar filas o columnas? (F/C): ");
                scanf(" %c", &tipoIntercambio); // Asegúrate de incluir un espacio antes de %c para consumir cualquier newline anterior
                
                // Determina si se intercambiarán filas o columnas
                if (tipoIntercambio == 'F' || tipoIntercambio == 'f') {
                    esFila = true;
                } else if (tipoIntercambio == 'C' || tipoIntercambio == 'c') {
                    esFila = false;
                } else {
                    printf("Opción no válida.\n");
                    break;
                }

                printf("Ingresa el número de bloque (0-2), posicion 1 (0-2), y posicion 2 (0-2) con la que se intercambiará, separados por espacio: ");
                printf("\nLos bloques de 3 comienzan con el bloque 0, así que si quieres modificar el primer bloque selecciona 0\n");                
                scanf("%d %d %d", &bloque, &posicion1, &posicion2);

                // Verifica que el bloque y las posiciones estén en el rango correcto
                if (bloque < 0 || bloque > 2 || posicion1 < 0 || posicion1 > 2 || posicion2 < 0 || posicion2 > 2) {
                    printf("Valores fuera de rango. Asegúrate de que el bloque y las posiciones estén entre 0 y 2.\n");
                    break;
                }

                intercambiarEnBloque(&juego, esFila, bloque, posicion1, posicion2);
                printf("Intercambio realizado.\n");
                break;
            }

            case 9:
                guardarJuego(&juego);
                printf("Juego guardado con éxito.\n");
                break;

            case 10:
                cargarJuego(&juego);
                break;


            case 11:
                printf("Gracias por jugar %s.\n", juego.nombreJugador);
                break;
            default:
                printf("Opción no válida.\n");
                break;


        }
    } while (opcion != 11);

    return 0;
}
