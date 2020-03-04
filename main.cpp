#include <QCoreApplication>
#include <stdio.h>
#include <stdlib.h>

#include <leer_entrada.h>

char* terminal;

int main()
{
    while (1) {
        fprintf(stderr,"****Ingrese comando****\n");

        terminal = leer_contenido_terminal();
        verificar_comando(terminal);
    }
    exit(EXIT_FAILURE);
}
