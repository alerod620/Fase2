#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <comando.h>
#include <ejecutar_comando.h>

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

MKFS parametros_mkfs(char parametros[])
{

}
