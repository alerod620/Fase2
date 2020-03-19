#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <analizar_comando.h>
#include <comando.h>

int c;
int tam_contenedor;
int posicion_contenedor;
int pos_linea;
char comando_analizado[10];
char parametros_comando[500];

/*
void mayusculas(char *comando) {
    int limite = strlen(comando);
    for (int i = 0; i < limite; i++) {
        comando[i] = toupper(comando[i]);
    }
}
*/

void limpiar_linea_comando() //Limpia todas las variables para leer el comando
{
    c = 0;
    tam_contenedor = 1024;
    posicion_contenedor = 0;
    pos_linea = 0;

    for(int i = 0; i < 10; i++)
    {
        comando_analizado[i] = '\0';
    }

    for(int i = 0; i < 500; i++)
    {
        parametros_comando[i] = '\0';
    }
}

void analizar_comando(char* comando)
{
    limpiar_linea_comando();

    while(1)
    {
        c = comando[pos_linea];
        if(c > 0 && c != ' ' && c != '\n') //Compara con caracter vacio y con salto de linea para saber si debe concatenar
        {
            comando_analizado[posicion_contenedor] = c; //Concatena los caracteres en la variable comando_analizado
            pos_linea++;
        }
        else
        {
            comando_analizado[posicion_contenedor] = '\0';
            posicion_contenedor = 0;
            pos_linea++;
            while(comando[pos_linea] != '\0')
            {
                c = comando[pos_linea];
                parametros_comando[posicion_contenedor] = c;
                pos_linea++;
                posicion_contenedor++;
            }
            ejecutar_comando(comando_analizado, parametros_comando);
            //fprintf(stderr, "------ RECONOCIO COMANDO %s ------ \n",comando_analizado);
            return;
        }
        posicion_contenedor++;
    }
}

void ejecutar_comando(char comando[],char parametros[]){
    mayusculas(comando); //Convierte todo el texto en la variable comando a mayusculas

    if(strcmp(comando,"MKDISK") ==0){
        mkdisk(parametros);
    }
    else if(strcmp(comando,"RMDISK") ==0)
    {
        rmdisk(parametros);
    }
    else if(strcmp(comando,"FDISK") ==0)
    {
        fdisk(parametros);
    }
    else if(strcmp(comando,"MOUNT") ==0)
    {
        mount(parametros);
    }
    else if(strcmp(comando,"UNMOUNT") ==0)
    {
        unmount(parametros);
    }
    else if(strcmp(comando,"REP") ==0)
    {
        rep(parametros);
    }
    else if(strcmp(comando,"EXEC") ==0)
    {
        exec(parametros);
    }
    else if(strcmp(comando,"EXIT") == 0)
    {
        exit(EXIT_FAILURE);
    }
    else if(strcmp(comando, "MKFS") == 0) //Comandos de la fase 2
    {

    }
    else if(strcmp(comando, "LOGIN") == 0)
    {

    }
    else if(strcmp(comando, "LOGOUT") == 0)
    {

    }
    else if(strcmp(comando, "MKGRP") == 0)
    {

    }
    else if(strcmp(comando, "RMGRP") == 0)
    {

    }
    else if(strcmp(comando, "MKUSR") == 0)
    {

    }
    else if(strcmp(comando, "RMUSR") == 0)
    {

    }
    else if(strcmp(comando, "CHMOD") == 0)
    {

    }
    else if(strcmp(comando, "MKFILE") == 0)
    {

    }
    else if(strcmp(comando, "CAT") == 0)
    {

    }
    else if(strcmp(comando, "REM") == 0)
    {

    }
    else if(strcmp(comando, "EDIT") == 0)
    {

    }
    else if(strcmp(comando, "REN") == 0)
    {

    }
    else if(strcmp(comando, "MKDIR") == 0)
    {

    }
    else if(strcmp(comando, "CP") == 0)
    {

    }
    else if(strcmp(comando, "MV") == 0)
    {

    }
    else if(strcmp(comando, "FIND") == 0)
    {

    }
    else if(strcmp(comando, "CHOWN") == 0)
    {

    }
    else if(strcmp(comando, "CHGRP") == 0)
    {

    }
    else if(strcmp(comando, "RECOVERY") == 0)
    {

    }
    else if(strcmp(comando, "LOSS") == 0)
    {

    }
    else
    {
        fprintf(stderr, "********** COMANDO %s NO RECONOCIDO **********\n",comando);
    }
}

