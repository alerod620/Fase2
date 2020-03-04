#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <leer_entrada.h>
#include <analizar_comando.h>

int caracter_entrada; //Guardara cada caracter de la consola
int tam_cont;
int posicion; //Llevara control de la posicion en la variable buffer
char buffer[1000]; //Variable para guardar todo lo que se escriba en la consola

char* leer_contenido_terminal()
{
    limpiar_variables_archivo();
    if(!buffer)
    {
        fprintf(stderr, "---------- ERROR EN TERMINAL----------\n");
        exit(EXIT_FAILURE);
    }

    while (1) //Recorrerá la variable buffer caracter por caracter
    {
        caracter_entrada = getchar();

        if (caracter_entrada == EOF || caracter_entrada == '\n') //Compara que el caracter sea el final de un archivo o que tenga un salto de linea
        {
            buffer[posicion] = ' ';
            buffer[posicion + 1] = '\0';
            return buffer;
        } else {
            buffer[posicion] = caracter_entrada;
        }
        posicion++;
    }
}

void limpiar_variables_archivo()
{
    caracter_entrada = 0;
    tam_cont = 1024;
    posicion = 0;
    for(int i = 0; i <1000; i++) //Limpia la variable buffer posicion por posicion
    {
        buffer[i] = '\0';
    }
}

void verificar_comando(char* comando)
{
    if(comando[0] == '#') //Compara el primer caracter del comando con '#' y de ser igual lo omite por ser un comentario
    {

    }
    else if(comando[0] == '\n' || comando[0] == '\0' || comando[0] == '\r') //Compara el primer caracter con salto de linea, NULL y con retorno de carro
    {

    }
    else //Se analiza el comando
    {
        for (int i = 0; i < 1000; i++) //Recorre el la variable del comando para eliminar un posible comentario al final del comando
        {
            if (comando[i] == '#')
            {
                comando[i] = '\0';
            }
        }
        analizar_comando(comando);
    }
}

void leer_archivo(char* path)
{
    FILE* archivo;
    limpiar_variables_archivo();

    archivo = fopen(path, "r");

    if (archivo == NULL)
    {
        printf("%s\n", "*** ERROR AL ABRIR ARCHIVO ***");
    }
    else
    {
        while (caracter_entrada != EOF)
        {
            caracter_entrada = fgetc(archivo);

            if (caracter_entrada == '\n')
            {

                buffer[posicion] = '\n';
                buffer[posicion + 1] = '\0';

                //Analiza la linea de comando
                fprintf(stderr,buffer);
                verificar_comando(buffer);
                limpiar_variables_archivo();
            }
            else if (caracter_entrada == '\r')
            {

            }
            else
            {
                buffer[posicion] = caracter_entrada;
                posicion++;
            }
        }
        fclose(archivo);
    }
}
