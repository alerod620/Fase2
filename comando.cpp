#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <comando.h>
#include <ejecutar_comando.h>
#include <validar_comando.h>

int caracter;
int pos_contenedor;
int pos_parametro;
int pos_valor;
char nombre_parametro[255];
char val_parametro[255];
char parametros[255];
char raid_path[255];

void mayusculas(char *cadena)
{
    int limite = strlen(cadena);
    for (int i = 0; i < limite; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}



void iniciar_buffer(char* parametro)
{
    caracter = 0;
    pos_valor = 0;
    pos_contenedor = 0;
    pos_parametro = 0;
    for(int i = 0; i < 255; i++) //Limpia la basura de las variables
    {
        nombre_parametro[i] = '\0';
        val_parametro[i] = '\0';
        parametros[i] = '\0';
    }
    strcpy(parametros, parametro);
}

void limpiar_buffer(int buffer)
{
    if(buffer == 0)
    {
        pos_parametro = 0;
        for(int i = 0; i < 255; i++)
        {
            nombre_parametro[i] = '\0';
        }
    }
    else if(buffer == 1)
    {
        pos_valor = 0;
        for(int i = 0; i < 255; i++)
        {
            val_parametro[i] = '\0';
        }
    }
}

void v_e_fdisk(char* parametro, int* ban)
{
    if(*ban == 0)
    {
        if(strcmp(parametro, "SIZE") == 0)
        {
            *ban = 1;
        }
        else if(strcmp(parametro, "DELETE") == 0)
        {
            *ban = 2;
        }
        else if(strcmp(parametro, "ADD") == 0)
        {
            *ban = 3;
        }
    }
}

void parametro_error(char* comando)
{
    fprintf(stderr, "****** COMANDO %s NO RECONOCIDO ******\n", comando);
    while(parametros[pos_contenedor] != ' ')
    {
        pos_contenedor++;
    }
}



//*************************************************************************
//******************** Metodos para obtener parametros ********************
//*************************************************************************


void get_type(int* ban) //Reconoce el tipo de particion que se desea hacer
{
    limpiar_buffer(1);
    while(parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    mayusculas(val_parametro);
    if(val_parametro[0] == 'P') //Primaria
    {
        *ban = 1;
    }
    else if(val_parametro[0] == 'E') //Extendida
    {
        *ban = 2;
    }
    else if(val_parametro[0] == 'L') //Logica
    {
        *ban = 3;
    }
    else
    {
        *ban = 4;
    }
}

void get_unit(int* ban) //Reconoce la unidad del tamaño del archivo
{
    limpiar_buffer(1);
    while(parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    mayusculas(val_parametro);
    if(val_parametro[0] == 'K') //Kilobytes
    {
        *ban = 1;
    }
    else if(val_parametro[0] == 'M') //Megabytes
    {
        *ban = 2;
    }
    else if(val_parametro[0] == 'B') //Bytes
    {
        *ban = 3;
    }
    else
    {
        *ban = 4;
    }
}

void get_fit(int* ban) //Reconoce el ajuste de la particion
{
    limpiar_buffer(1);
    while(parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    mayusculas(val_parametro);
    if(strcmp(val_parametro, "BF") == 0) //Mejor ajuste
    {
        *ban = 1;
    }
    else if(strcmp(val_parametro, "FF") == 0) //Primer ajuste
    {
        *ban = 2;
    }
    else if(strcmp(val_parametro, "WF") == 0) //Peor ajuste
    {
        *ban = 3;
    }
    else //ERROR
    {
        *ban = 4;
    }
}

void get_size(int *valor, int *ban)
{
    limpiar_buffer(1);
    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    *valor = atoi(val_parametro);
    if (*valor > 0)
    {
        *ban = 1;
    }
    else
    {
        *ban = 2;
    }
}

void get_add(int* valor, int* ban)
{
    limpiar_buffer(1);
    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    *valor = atoi(val_parametro);
    *ban = 1;
}

void get_delete(int* ban) {
    limpiar_buffer(1);
    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    mayusculas(val_parametro);
    if (strcmp(val_parametro, "FAST") == 0)
    {
        *ban = 1;
    }
    else if(strcmp(val_parametro, "FULL") == 0)
    {
        *ban = 2;
    }
    else
    {
        *ban = 3; // ERROR
    }
}

void get_path(char valor[], int *ban)
{
    limpiar_buffer(1);
    if (parametros[pos_contenedor] != '"')
    {
        while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_contenedor++;
            pos_valor++;
        }
    }
    else
    {
        pos_contenedor++;
        while (parametros[pos_contenedor] != '"') {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_contenedor++;
            pos_valor++;
        }
        pos_contenedor++;
    }
    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void get_name(char valor[], int* ban)
{
    limpiar_buffer(1);
    if (parametros[pos_contenedor] != '"')
    {
        while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_contenedor++;
            pos_valor++;
        }
    }
    else
    {
        pos_contenedor++;
        while (parametros[pos_contenedor] != '"') {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_contenedor++;
            pos_valor++;
        }
        pos_contenedor++;
    }
    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void get_id(char valor[], int* ban)
{
    limpiar_buffer(1);
    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_contenedor++;
        pos_valor++;
    }
    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void mkdisk(char parametro[])
{
    MKDISK tmp = parametros_mkdisk(parametro);
    if(tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1)
        {
            switch(tmp.ban_size)
            {
            case 0:
                fprintf(stderr, "****** DEBE INGRESAR TAMAÑO PARA EL DISCO ******\n");
                return;
                break;

            case 1:
                if(tmp.ban_unit == 3 || tmp.ban_unit == 4)
                {
                    fprintf(stderr, "****** UNIDAD INCORRECTA ******\n");
                }
                else
                {
                    ejecutar_mkdisk(&tmp);

                    //Ejecutar RAID
                    path_raid(tmp.path);
                    strcpy(tmp.path, raid_path);
                    ejecutar_mkdisk(&tmp);
                }
                break;

            case 2:
                fprintf(stderr, "****** SIZE NO VALIDO ******\n");
                return;
                break;
            }
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA PARA EL DISCO ******\n");
        }
    }
}

void rmdisk(char parametro[])
{
    RMDISK tmp = parametros_rmdisk(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_path == 1) {
            ejecutar_rmdisk(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA DEL DISCO ******\n");
        }
    }
}

void fdisk(char parametro[]) {
    FDISK tmp = parametros_fdisk(parametro);
    FILE* archivo; //Variable para saber si debe ejecuar eliminar y redim en el raid

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1) {
            switch (tmp.ban_s_d_a) {
                case 1:
                    crear_fdisk(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        crear_fdisk(&tmp);
                    }

                    break;
                case 2:
                    ejecutar_eliminar(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        ejecutar_eliminar(&tmp);
                    }
                    break;
                case 3:
                    redim_fdisk(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        redim_fdisk(&tmp);
                    }


                    break;
            }
        } else if (tmp.ban_path != 1 && tmp.ban_name == 1) {
            fprintf(stderr, "****** DEBE INGRESAR PATH DE DISCO ******\n");
        } else if (tmp.ban_path == 1 && tmp.ban_name != 1) {
            fprintf(stderr, "****** FALTA NOMBRE DE PARTICION ******\n");
        } else {
            fprintf(stderr, "****** FALTA NOMBRE Y RUTA ******\n");
        }
    }
}

void mount(char parametro[]) {
    MOUNT tmp = parametros_mount(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1) {
            ejecutar_mount(&tmp);
        } else if (tmp.ban_path != 1 && tmp.ban_name == 1) {
            fprintf(stderr, "****** DEBE INGRESAR PATH DE DISCO ******\n");
        } else if (tmp.ban_path == 1 && tmp.ban_name != 1) {
            fprintf(stderr, "****** DEBE INGRESAR NOMBRE DE DISCO ******\n");
        } else {
            fprintf(stderr, "****** DEBE INGRESAR NOMBRE Y RUTA DEL DISCO ******\n");
        }
    }
}

void unmount(char parametro[])
{

    UNMOUNT tmp = parametros_unmount(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_id == 1) {
            ejecutar_unmount(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE DE INGRESAR ID DE PARTICION ******\n");
        }
    }
}

void rep(char parametro[])
{
    REP tmp = parametros_rep(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1 && tmp.ban_id == 1) {
            mayusculas(tmp.name);
            if (strcmp(tmp.name, "DISK") == 0) {
                rep_disk(&tmp);
            } else if (strcmp(tmp.name, "MBR") == 0) {
                rep_mbr(&tmp);
            }
        } else {
            fprintf(stderr, "****** DEBE DE INGRESAR NOMBRE, PATH Y ID ******\n");
        }
    }
}

void exec(char parametro[])
{
    EXEC tmp = parametros_exec(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_path == 1)
        {
            ejecutar_exec(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA ******\n");
        }
    }
}



MKDISK parametros_mkdisk(char parametro[])
{
    MKDISK tmp = nuevo_mkdisk();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "SIZE") == 0)
            {
                get_size(&tmp.size, &tmp.ban_size);
            }
            else if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "UNIT") == 0)
            {
                get_unit(&tmp.ban_unit);
            }
            else if(strcmp(nombre_parametro, "FIT") == 0)
            {
                get_fit(&tmp.ban_fit);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    //En caso de no venir estos atributos
    if (tmp.ban_unit == 0)
    {
        tmp.ban_unit = 2;
    }
    if (tmp.ban_fit == 0)
    {
        tmp.ban_fit = 3;
    }
    return tmp;
}

RMDISK parametros_rmdisk(char parametro[])
{
    RMDISK tmp = nuevo_rmdisk();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        } else {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }
    return tmp;
}

FDISK parametros_fdisk(char parametro[])
{
    FDISK tmp = nuevo_fdisk();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0') {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "SIZE") == 0)
            {
                get_size(&tmp.size, &tmp.ban_size);
                v_e_fdisk(nombre_parametro, &tmp.ban_s_d_a);
            }
            else if (strcmp(nombre_parametro, "UNIT") == 0)
            {
                get_unit(&tmp.ban_unit);
            }
            else if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "TYPE") == 0)
            {
                get_type(&tmp.ban_type);
            }
            else if (strcmp(nombre_parametro, "FIT") == 0)
            {
                get_fit(&tmp.ban_fit);
            }
            else if (strcmp(nombre_parametro, "DELETE") == 0)
            {
                get_delete(&tmp.ban_delete);
                v_e_fdisk(nombre_parametro, &tmp.ban_s_d_a);
            }
            else if (strcmp(nombre_parametro, "NAME") == 0)
            {
                get_name(tmp.name, &tmp.ban_name);
            }
            else if (strcmp(nombre_parametro, "ADD") == 0)
            {
                get_add(&tmp.add, &tmp.ban_add);
                v_e_fdisk(nombre_parametro, &tmp.ban_s_d_a);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }


    //En caso de no venir estos atributos
    if (tmp.ban_unit == 0)
    {
        tmp.ban_unit = 1;
    }
    if (tmp.ban_type == 0)
    {
        tmp.ban_type = 1;
    }
    if (tmp.ban_fit == 0)
    {
        tmp.ban_fit = 3;
    }

    return tmp;
}

MOUNT parametros_mount(char parametro[])
{
    MOUNT tmp = nuevo_mount();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);
            if (strcmp(nombre_parametro, "PATH") == 0) {
                get_path(tmp.path, &tmp.ban_path);
            } else if (strcmp(nombre_parametro, "NAME") == 0) {
                get_name(tmp.name, &tmp.ban_name);
            } else {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }
    return tmp;
}

UNMOUNT parametros_unmount(char parametro[])
{
    UNMOUNT tmp = nuevo_unmount();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "ID") == 0) {
                get_id(tmp.id, &tmp.ban_id);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }
    return tmp;
}

REP parametros_rep(char parametro[])
{
    REP tmp = nuevo_rep();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        } else {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);
            if (strcmp(nombre_parametro, "NAME") == 0)
            {
                get_name(tmp.name, &tmp.ban_name);
                mayusculas(tmp.name);
            }
            else if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "ID") == 0)
            {
                get_id(tmp.id, &tmp.ban_id);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }
    return tmp;
}

EXEC parametros_exec(char parametro[])
{
    EXEC tmp = nuevo_exec();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {
            if (caracter > 0 && caracter != '-'  && caracter != ' ')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);
            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }
    return tmp;
}

void path_raid(char tmp[])
{
    int pos_tmp_path = 0;
    for(int i = 0; i < 255; i++)
    {
        raid_path[i] = '\0';
    }

    for(int i = 0; i < 255; i++)
    {
        if(tmp[i] > 0)
        {
            if(tmp[pos_tmp_path] != '.')
            {
                raid_path[pos_tmp_path] = tmp[i];
                pos_tmp_path++;
            }
            else
            {

            }
        }
    }

    strcat(raid_path, "_ra1.disk");
}


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

//*************************************************************************
//******************** Metodos para obtener parametros ********************
//*************************************************************************

void get_fs(int* ban)
{
    limpiar_buffer(1);

    while(parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_valor++;
        pos_contenedor;
    }
    mayusculas(val_parametro);

    if(strcmp(val_parametro, "2FS") == 0)
    {
        *ban = 1;
    }
    else if(strcmp(val_parametro, "3FS") == 0)
    {
        *ban = 2;
    }
}

void get_usr(char valor[], int *ban)
{
    limpiar_buffer(1);

    if (parametros[pos_contenedor] != '"')
    {
        while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
    }
    else
    {
        pos_contenedor++;
        while (parametros[pos_contenedor] != '"')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
        pos_contenedor++;
    }

    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void get_pwd(char valor[], int* ban)
{
    limpiar_buffer(1);

    if (parametros[pos_contenedor] != '"')
    {
        while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
    }
    else
    {
        pos_contenedor++;
        while (parametros[pos_contenedor] != '"') {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
        pos_contenedor++;
    }

    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void get_grp(char valor[], int* ban)
{
    limpiar_buffer(1);

    if (parametros[pos_contenedor] != '"')
    {
        while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
    }
    else
    {
        pos_contenedor++;
        while (parametros[pos_contenedor] != '"')
        {
            val_parametro[pos_valor] = parametros[pos_contenedor];
            pos_valor++;
            pos_contenedor++;
        }
        pos_contenedor++;
    }

    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}

void get_particion(int *ban)
{
    limpiar_buffer(1);

    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_valor++;
        pos_contenedor++;
    }
    mayusculas(val_parametro);
    if (val_parametro[0] == 'P')
    {
        *ban = 1;
    }
    else if (val_parametro[0] == 'E')
    {
        *ban = 2;
    }
    else if (val_parametro[0] == 'L')
    {
        *ban = 3;
    }
    else {
        *ban = 4;
    }
}

void get_formateo(int* ban)
{
    limpiar_buffer(1);

    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_valor++;
        pos_contenedor++;
    }

    mayusculas(val_parametro);

    if (strcmp(val_parametro, "FAST") == 0)
    {
        *ban = 1;
    } else if (strcmp(val_parametro, "FULL") == 0)
    {
        *ban = 2;
    }
}

void get_ugo(char valor[], int* ban)
{
    limpiar_buffer(1);

    while (parametros[pos_contenedor] != ' ' && parametros[pos_contenedor] != '\n')
    {
        val_parametro[pos_valor] = parametros[pos_contenedor];
        pos_valor++;
        pos_contenedor++;
    }

    val_parametro[pos_valor] = '\0';
    strcpy(valor, val_parametro);
    *ban = 1;
}


void mkfs(char parametro[])
{
    MKFS tmp = parametros_mkfs(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_id == 0)
        {
            mensaje("ERROR DEBE DE INGRESAR UN ID");
        }
        else
        {
            ejecutar_mkfs(&tmp);
            //print_mkfs(&tmp);
        }
    }

}

void login(char parametro[])
{
    LOGIN tmp = parametros_login(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_id == 1 && tmp.ban_pwd == 1 && tmp.ban_usr == 1)
        {
            ejecutar_login(&tmp);
        }
        else
        {
            mensaje("ERROR FALTA PARAMETROS");
        }
    }

}

void logout(char parametro[]) {
    LOGOUT tmp = parametros_logout(parametro);

    if (tmp.ban_error == 0)
    {
        ejecutar_logout(&tmp);
    }

}

void mkgrp(char parametro[]) {
    MKGRP tmp = parametros_mkgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_name == 0)
        {
            mensaje("ERROR FALTA PARAMETRO NAME");
        }
        else
        {
            //print_mkgrp(&tmp);
        }
    }

}

void rmgrp(char parametro[])
{
    RMGRP tmp = parametros_rmgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_name == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO NAME");
        }
        else
        {
            //print_rmgrp(&tmp);
        }
    }

}

void mkusr(char parametro[]) {
    MKUSR tmp = parametros_mkusr(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_usr == 1 && tmp.ban_pwd == 1 && tmp.ban_grp == 1)
        {
            //print_mkusr(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void rmusr(char parametro[])
{
    RMUSR tmp = parametros_rmusr(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_usr == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO USR");
        }
        else
        {
            //print_rmusr(&tmp);
        }
    }

}

void chmod(char parametro[])
{
    CHMOD tmp = parametros_chmod(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_path == 1 && tmp.ban_ugo == 1)
        {
            //print_chmod(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mkfile(char parametro[])
{
    MKFILE tmp = parametros_mkfile(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_path == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO PATH");
        }
        else
        {
            ejecutar_mkfile(&tmp);
            //print_mkfile(&tmp);
        }
    }

}

void cat(char parametro[])
{
    CAT tmp = parametros_cat(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_file == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO FILE-N");
        }
        else
        {
            //print_cat(&tmp);
        }
    }
}

void rem(char parametro[])
{
    REM tmp = parametros_rem(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO PATH");
        }
        else
        {
            //print_rem(&tmp);
        }
    }

}

void edit(char parametro[])
{
    EDIT tmp = parametros_edit(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0 || tmp.ban_cont == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO PATH");
        }
        else
        {

            //print_edit
        }
    }

}

void ren(char parametro[])
{
    REN tmp = parametros_ren(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_name == 1)
        {
            //print_ren(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mkdir(char parametro[])
{
    MKDIR tmp = parametros_mkdir(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO PATH");
        }
        else
        {
            ejecutar_mkdir(&tmp);
        }
    }

}

void cp(char parametro[])
{
    CP tmp = parametros_cp(parametro);

    if (tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1 && tmp.ban_dest == 1)
        {
            //print_cp(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mv(char parametro[])
{
    MV tmp = parametros_mv(parametro);

    if (tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1 && tmp.ban_dest == 1)
        {
            //print_mv(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void find(char parametro[])
{
    FIND tmp = parametros_find(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_name == 1)
        {
            //print_find(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void chown(char parametro[])
{
    CHOWN tmp = parametros_chown(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_usr == 1)
        {
            print_chown(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void chgrp(char parametro[])
{
    CHGRP tmp = parametros_chgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_usr ==  1 && tmp.ban_grp == 1)
        {
            //print_chgrp(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void recovery(char parametro[])
{
    RECOVERY tmp = parametros_recovery(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_id == 0)
        {
            mensaje("ERROR HACE FALTA ID DE PARTICION");
        }
        else
        {
            //print_recovery(&tmp);
        }
    }

}

void loss(char parametro[])
{
    LOSS tmp = parametros_loss(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_id == 0)
        {
            mensaje("ERROR HACE FALTA ID DE PARTICION");
        }
        else
        {
            //print_loss(&tmp);
        }
    }

}


MKFS parametros_mkfs(char parametro[])
{
    MKFS tmp = nuevo_mkfs();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = [pos_contenedor];

        if(caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "ID") == 0)
            {
                get_path(tmp.id, &tmp.ban_id);
            }
            else if (strcmp(nombre_parametro, "TYPE") == 0)
            {
                get_formateo(&tmp.ban_type);
            }
            else if (strcmp(nombre_parametro, "FS") == 0)
            {
                get_fs(&tmp.ban_fs);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }
            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    if (tmp.ban_type == 0)
    {
        tmp.ban_type = 2;
    }
    if (tmp.ban_fs == 0)
    {
        tmp.ban_fs = 1;
    }

    return tmp;
}

LOGIN parametros_login(char parametro[])
{
    LOGIN tmp = nuevo_login();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "ID") == 0)
            {
                get_id(tmp.id, &tmp.ban_id);
            }
            else if (strcmp(nombre_parametro, "USR") == 0)
            {
                get_usr(tmp.usr, &tmp.ban_usr);
            }
            else if (strcmp(nombre_parametro, "PWD") == 0)
            {
                get_pwd(tmp.pwd, &tmp.ban_pwd);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

LOGOUT parametros_logout(char parametro[])
{
    LOGOUT tmp = nuevo_logout();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != ' ' || caracter != '\n' || caracter != '\0')
        {
            tmp.ban_error = 1;
            mensaje("parametros NO RECONOCIDOS EN COMANDO");
        }
        pos_contenedor++;
    }

    if (tmp.ban_error == 0)
    {
        tmp.ban_logout = 1;
    }

    return tmp;
}

MKGRP parametros_mkgrp(char parametro[])
{
    MKGRP tmp = nuevo_mkgrp();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if(caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "NAME") == 0) {
                get_name(tmp.name, &tmp.ban_name);
            } else {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

RMGRP parametros_rmgrp(char parametro[])
{
    RMGRP tmp = nuevo_rmgrp();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "NAME") == 0)
            {
                get_name(tmp.name, &tmp.ban_name);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

MKUSR parametros_mkusr(char parametro[])
{
    MKUSR tmp = nuevo_mkusr();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "USR") == 0)
            {
                get_usr(tmp.usr, &tmp.ban_usr);
            }
            else if (strcmp(nombre_parametro, "PWD") == 0)
            {
                get_pwd(tmp.pwd, &tmp.ban_pwd);
            }
            else if (strcmp(nombre_parametro, "GRP") == 0)
            {
                get_grp(tmp.grp, &tmp.ban_grp);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

RMUSR parametros_rmusr(char parametro[])
{
    RMUSR tmp = nuevo_rmusr();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "USR") == 0)
            {
                get_usr(tmp.usr, &tmp.ban_usr);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

CHMOD parametros_chmod(char parametro[])
{
    CHMOD tmp = nuevo_chmod();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-' && caracter != '\n')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
            else if (caracter == ' ' || caracter == '\n')
            {
                nombre_parametro[pos_parametro] = '\0';
                mayusculas(nombre_parametro);

                if (strcmp(nombre_parametro, "R") == 0)
                {
                    tmp.ban_r = 1;
                    limpiar_buffer(0);
                }
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "UGO") == 0)
            {
                get_ugo(tmp.ugo, &tmp.ban_ugo);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

MKFILE parametros_mkfile(char parametro[])
{
    MKFILE tmp = nuevo_mkfile();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-' && caracter != '\n')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
            else if (caracter == ' ' || caracter == '\n')
            {
                nombre_parametro[pos_parametro] = '\0';
                mayusculas(nombre_parametro);

                if (strcmp(nombre_parametro, "P") == 0)
                {
                    tmp.ban_p = 1;
                    limpiar_buffer(0);
                }
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "SIZE") == 0)
            {
                get_size(&tmp.size, &tmp.ban_size);
            }
            else if (strcmp(nombre_parametro, "CONT") == 0)
            {
                get_path(tmp.cont, &tmp.ban_cont);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

CAT parametros_cat(char parametro[])
{
    CAT tmp = nuevo_cat();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "FILE") == 0)
            {
                get_path(tmp.path, &tmp.ban_file);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

REM parametros_rem(char parametro[])
{
    REM tmp = nuevo_rem();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

EDIT parametros_edit(char parametro[])
{
    EDIT tmp = nuevo_edit();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "CONT") == 0)
            {
                get_path(tmp.cont, &tmp.ban_cont);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

REN parametros_ren(char parametro[])
{
    REN tmp = nuevo_ren();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "NAME") == 0)
            {
                get_name(tmp.name, &tmp.ban_name);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

MKDIR parametros_mkdir(char parametro[])
{
    MKDIR tmp = nuevo_mkdir();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];
        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-' && caracter != '\n')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
            else if (caracter == ' ' || caracter == '\n')
            {
                nombre_parametro[pos_parametro] = '\0';
                mayusculas(nombre_parametro);

                if (strcmp(nombre_parametro, "P") == 0)
                {
                    tmp.ban_p = 1;
                    limpiar_buffer(0);
                }
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

CP parametros_cp(char parametro[])
{
    CP tmp = nuevo_cp();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "DEST") == 0)
            {
                get_path(tmp.dest, &tmp.ban_dest);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

MV parametros_mv(char parametro[])
{
    MV tmp = nuevo_mv();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "DEST") == 0)
            {
                get_path(tmp.dest, &tmp.ban_dest);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

FIND parametros_find(char parametro[])
{
    FIND tmp = nuevo_find();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "NAME") == 0)
            {
                get_name(tmp.name, &tmp.ban_name);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

CHOWN parametros_chown(char parametro[])
{
    CHOWN tmp = nuevo_chown();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-' && caracter != '\n')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
            else if (caracter == ' ' || caracter == '\n')
            {
                nombre_parametro[pos_parametro] = '\0';
                mayusculas(nombre_parametro);

                if (strcmp(nombre_parametro, "R") == 0)
                {
                    tmp.ban_r = 1;
                    limpiar_buffer(0);
                }
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "PATH") == 0)
            {
                get_path(tmp.path, &tmp.ban_path);
            }
            else if (strcmp(nombre_parametro, "USR") == 0)
            {
                get_usr(tmp.usr, &tmp.ban_usr);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

CHGRP parametros_chgrp(char parametro[])
{
    CHGRP tmp = nuevo_chgrp();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-' && caracter != '\n')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "USR") == 0)
            {
                get_usr(tmp.usr, &tmp.ban_usr);
            }
            else if (strcmp(nombre_parametro, "GRP") == 0)
            {
                get_grp(tmp.grp, &tmp.ban_grp);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

RECOVERY parametros_recovery(char parametro[])
{
    RECOVERY tmp = nuevo_recovery();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "ID") == 0)
            {
                get_id(tmp.id, &tmp.ban_id);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}

LOSS parametros_loss(char parametro[])
{
    LOSS tmp = nuevo_loss();
    iniciar_buffer(parametro);

    while (parametros[pos_contenedor] != '\0')
    {
        caracter = parametros[pos_contenedor];

        if (caracter != '=')
        {

            if (caracter > 0 && caracter != ' ' && caracter != '-')
            {
                nombre_parametro[pos_parametro] = caracter;
                pos_parametro++;
            }
        }
        else
        {
            pos_contenedor++;
            nombre_parametro[pos_parametro] = '\0';
            mayusculas(nombre_parametro);

            if (strcmp(nombre_parametro, "ID") == 0)
            {
                get_id(tmp.id, &tmp.ban_id);
            }
            else
            {
                tmp.ban_error = 1;
                parametro_error(nombre_parametro);
                break;
            }

            limpiar_buffer(0);
        }
        pos_contenedor++;
    }

    return tmp;
}
