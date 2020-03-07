#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>

#include <ejecutar_comando.h>
#include <leer_entrada.h>
#include <comando.h>
#include <validar_comando.h>
#include <reporte.h>

PARTICIONES_MONTADAS part_memoria[26];
int ban_part_memoria = 0;

void ini_particiones_memoria()
{
    if(ban_part_memoria == 0)
    {
        part_memoria[0].letra = 'a';
        part_memoria[1].letra = 'b';
        part_memoria[2].letra = 'c';
        part_memoria[3].letra = 'd';
        part_memoria[4].letra = 'e';
        part_memoria[5].letra = 'f';
        part_memoria[6].letra = 'g';
        part_memoria[7].letra = 'h';
        part_memoria[8].letra = 'i';
        part_memoria[9].letra = 'j';
        part_memoria[10].letra = 'k';
        part_memoria[11].letra = 'l';
        part_memoria[12].letra = 'm';
        part_memoria[13].letra = 'n';
        part_memoria[14].letra = 'o';
        part_memoria[15].letra = 'p';
        part_memoria[16].letra = 'q';
        part_memoria[17].letra = 'r';
        part_memoria[18].letra = 's';
        part_memoria[19].letra = 't';
        part_memoria[20].letra = 'u';
        part_memoria[21].letra = 'v';
        part_memoria[22].letra = 'w';
        part_memoria[23].letra = 'x';
        part_memoria[24].letra = 'y';
        part_memoria[25].letra = 'z';
        ban_part_memoria = 1;
    }
}

void ejecutar_mkdisk(MKDISK* tmp)
{
    ini_particiones_memoria();
    FILE* archivo;
    validar_dir(tmp->path);
    archivo = fopen(tmp->path, "wb");

    if(archivo != NULL)
    {
        MBR nuevo;
        BUFFER_CONT_DISK llenar;
        int it = 0;

        llenar = new_buffer();

        switch (tmp->ban_unit)
        {
        case 1: //kilobytes
            nuevo = new_mbr((tmp->size * 1024), tmp->ban_fit);
            fseek(archivo, 0, SEEK_SET);

            for(int i = 0; i < tmp->size; i++) //Llena el archivo
            {
                fwrite(&llenar, sizeof(BUFFER_CONT_DISK), 1, archivo);
            }
            break;

        case 2: //megabytes
            it = tmp->size * 1024;

            nuevo = new_mbr((it * 1024), tmp->ban_fit);
            fseek(archivo, 0, SEEK_SET);

            for(int i = 0; i < it; i++) // Llena el archivo
            {
                fwrite(&llenar, sizeof(BUFFER_CONT_DISK), 1, archivo);
            }
            break;
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&nuevo, sizeof(MBR), 1, archivo);
        fclose(archivo);
        fprintf(stderr, "****** EL DISCO HA SIDO CREADO CON EXITO ******\n");
    }
    else
    {
        mensaje("ERROR EN CREACION DE DISCO");
    }
}

void ejecutar_rmdisk(RMDISK* tmp)
{
    ini_particiones_memoria();
    FILE* archivo;
    archivo = fopen(tmp->path, "rb");

    if(archivo == NULL)
    {
        mensaje("NO SE ENCONTRO EL DISCO");
    }
    else
    {
        fprintf(stderr, "**** DESEA ELIMINAR DISCO [S/N] ****\n");
        char* opcion = leer_contenido_terminal();
        mayusculas(opcion);
        if(opcion[0] == 'S')
        {
            fclose(archivo);
            if(remove(tmp->path) == 0)
            {
                fprintf(stderr, "****** EL DISCO FUE ELIMINADO CON EXITO ******\n");
            }
            else
            {
                mensaje("EL DISCO NO SE PUDO ELIMINAR");
            }
        }
    }
}

void crear_fdisk(FDISK* tmp)
{
    ini_particiones_memoria();
    //MBR tmp_mbr;

    if(tmp->ban_type == 1)
    {
        crear_primaria(tmp);
    }
    else if(tmp->ban_type == 2)
    {
        crear_extendida(tmp);
    }
    else if(tmp->ban_type == 3)
    {
        crear_logica(tmp);
    }

    /*
    for(int i = 0; i < 4; i++)
    {
        if(strcpy(tmp_mbr.mbr_particion[i].name, tmp->name) != 0)
        {

        }
        else
        {
            mensaje("YA EXISTE UNA PARTICION CON ESE NOMBRE");
            break;
        }
    }
    */

}

void crear_primaria(FDISK* tmp)
{
    FILE* archivo;
    MBR tmp_mbr;
    archivo = fopen(tmp->path, "rb+");
    int nombre_valido = 0;

    if(archivo == NULL)
    {
        mensaje("DISCO NO ENCONTRADO");
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        if(validar_pri(&tmp_mbr) == 0)
        {
            /*
            //Recorre las particiones para ver si no hay una con el mismo nombre
            for(int i = 0; i < 4; i++)
            {
                if(strcpy(tmp_mbr.mbr_particion[i].name,tmp->name) == 0)
                {
                    nombre_valido = 1;
                    break;
                }
            }*/

            for(int i = 0; i < 4; i++) // Recorre las partciones del disco para ver si hay vacias
            {
                if (tmp_mbr.mbr_particion[i].status == '0')
                {
                    int tam_part = tam_particion(tmp->ban_unit, tmp->size) - 1;
                    int pos_inicio = sizeof (MBR) + 1;


                    switch(i)
                    {
                    case 0:
                        if(validar_p1(&tmp_mbr, tam_part, &pos_inicio) == 1)
                        {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                        }
                        break;

                    case 1:
                        if (validar_p2(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                        }
                        break;
                    case 2:
                        if (validar_p3(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                        }
                        break;
                    case 3:
                        if (validar_p4(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                        }
                        break;
                    }
                    break;
                }
                if(i == 3)
                {
                    mensaje("NO HAY ESPACIO SUFICIENTE");
                }
            }
        }
        else
        {
            mensaje("YA SE ALCANZO EL NUMERO MAXIMO DE PARTICIONES PRIMARIAS");
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}

void crear_extendida(FDISK* tmp)
{
    FILE* archivo;
    MBR tmp_mbr;
    archivo = fopen(tmp->path, "rb+");
    int nombre_valido = 0;

    if (archivo == NULL)
    {
        mensaje("DISCO NO ENCONTRADO");
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        if (validar_ext(&tmp_mbr) == 0)
        {
            /*
            //Recorre las particiones para ver si no hay una con el mismo nombre
            for(int i = 0; i < 4; i++)
            {
                if(strcpy(tmp_mbr.mbr_particion[i].name,tmp->name) == 0)
                {
                    nombre_valido = 1;
                    break;
                }
            }*/

            for (int i = 0; i < 4; i++)
            {
                if (tmp_mbr.mbr_particion[i].status == '0')
                {
                    int tam_part = tam_particion(tmp->ban_unit, tmp->size) - 1;
                    int pos_inicio = sizeof (MBR) + 1;

                    switch (i) {
                    case 0:
                        if (validar_p1(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                            EBR nuevo = new_ebr('0', 0, pos_inicio, 0);
                            fseek(archivo, pos_inicio, SEEK_SET);
                            fwrite(&nuevo, sizeof (EBR), 1, archivo);
                        }
                        break;
                    case 1:
                        if (validar_p2(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                            EBR nuevo = new_ebr('0', 0, pos_inicio, 0);
                            fseek(archivo, pos_inicio, SEEK_SET);
                            fwrite(&nuevo, sizeof (EBR), 1, archivo);
                        }
                        break;
                    case 2:
                        if (validar_p3(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                            EBR nuevo = new_ebr('0', 0, pos_inicio, 0);
                            fseek(archivo, pos_inicio, SEEK_SET);
                            fwrite(&nuevo, sizeof (EBR), 1, archivo);
                        }
                        break;
                    case 3:
                        if (validar_p4(&tmp_mbr, tam_part, &pos_inicio) == 1) {
                            escribir_particion_mbr(&tmp_mbr, tmp, i, pos_inicio, tam_part);
                            EBR nuevo = new_ebr('0', 0, pos_inicio, 0);
                            fseek(archivo, pos_inicio, SEEK_SET);
                            fwrite(&nuevo, sizeof (EBR), 1, archivo);
                        }
                        break;
                    }
                    break;
                }
                if (i == 3) {
                    mensaje("NO HAY ESPACIO SUFICIENTE");
                    break;
                }
            }
        }
        else
        {
            mensaje("YA EXISTE UNA PARTICION LOGICA");
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}

void crear_logica(FDISK* tmp)
{
    FILE* archivo;
    MBR tmp_mbr;
    EBR tmp_ebr;
    archivo = fopen(tmp->path, "rb+");

    if (archivo == NULL)
    {
        mensaje("DISCO NO ENCONTRADO");
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        for (int i = 0; i < 4; i++)
        {
            if (tmp_mbr.mbr_particion[i].type == 'E')
            {
                int tam_part = tam_particion(tmp->ban_unit, tmp->size) - 1;
                int pos_inicio = tmp_mbr.mbr_particion[i].start;

                while (1)
                {
                    fseek(archivo, pos_inicio, SEEK_SET);
                    fread(&tmp_ebr, sizeof (EBR), 1, archivo);

                    //De existir una extendida siempre estara un EBR
                    if (tmp_mbr.mbr_particion[i].start == tmp_ebr.start)
                    {
                        if (tmp_ebr.status == '0') //El primer EBR esta vacio
                        {
                            if (tmp_ebr.next == -1) //Esta vacia la particion extendida
                            {
                                fseek(archivo, pos_inicio, SEEK_SET);
                                escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, -1);
                                fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                break;
                            }
                            else //El primer EBR esta vacio
                            {

                                if ((pos_inicio + tam_part) < tmp_ebr.next) //Valida si es posible colocarlo entre el EBR actual y el siguiente
                                {
                                    //Se actualiza el puntero siguiente del EBR y se inserta el nuevo
                                    int puntero_next = tmp_ebr.next;
                                    tmp_ebr.next = pos_inicio;
                                    fseek(archivo, tmp_ebr.start, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                    escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, puntero_next);
                                    fseek(archivo, pos_inicio, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                    break;
                                }
                            }
                        }
                        else
                        {
                            pos_inicio = tmp_ebr.start + tmp_ebr.size + 1;

                            if (tmp_ebr.next == -1) //Verifica si es el ultimo EBR
                            {
                                if ((pos_inicio + tam_part) <= (tmp_mbr.mbr_particion[i].size + tmp_mbr.mbr_particion[i].start)) //Comprueba si cabe entre esta particion y el final de la particion extendida
                                {

                                    tmp_ebr.next = pos_inicio;
                                    fseek(archivo, tmp_ebr.start, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                    escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, -1);
                                    fseek(archivo, pos_inicio, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                    break;
                                }
                                else
                                {
                                    mensaje("ESPACIO INSUFICIENTE");
                                    break;
                                }
                            }
                            else
                            {
                                if ((pos_inicio + tam_part) < tmp_ebr.next)
                                {

                                    int puntero_next = tmp_ebr.next;
                                    tmp_ebr.next = pos_inicio;
                                    fseek(archivo, tmp_ebr.start, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                    escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, puntero_next);
                                    fseek(archivo, pos_inicio, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                    break;
                                }
                                else
                                {
                                    pos_inicio = tmp_ebr.next;
                                }
                            }
                        }
                    }
                    else
                    {
                        pos_inicio = tmp_ebr.start + tmp_ebr.size + 1;

                        if (tmp_ebr.next == -1) //Verifica si es el ultimo EBR
                        {
                            if ((pos_inicio + tam_part) <= (tmp_mbr.mbr_particion[i].size + tmp_mbr.mbr_particion[i].start))
                            {

                                tmp_ebr.next = pos_inicio;
                                fseek(archivo, tmp_ebr.start, SEEK_SET);
                                fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, -1);
                                fseek(archivo, pos_inicio, SEEK_SET);
                                fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                break;
                            }
                            else
                            {
                                mensaje("ESPACIO INSUFICIENTE");
                                break;
                            }
                        }
                        else
                        {
                            if ((pos_inicio + tam_part) < tmp_ebr.next)
                            {

                                int puntero_next = tmp_ebr.next;
                                tmp_ebr.next = pos_inicio;
                                fseek(archivo, tmp_ebr.start, SEEK_SET);
                                fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                escribir_particion_ebr(&tmp_ebr, tmp, pos_inicio, tam_part, puntero_next);
                                fseek(archivo, pos_inicio, SEEK_SET);
                                fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);
                                break;
                            }
                            else
                            {
                                pos_inicio = tmp_ebr.next;
                            }
                        }
                    }
                }
                break;
            }
            if (i == 3)
            {
                mensaje("NO EXISTE PARTICION EXTENDIDA");
                break;
            }
        }
        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}

void ejecutar_eliminar(FDISK* tmp)
{
    ini_particiones_memoria();
    FILE* archivo;
    EBR tmp_ebr;
    MBR tmp_mbr;
    archivo = fopen(tmp->path, "rb+");
    int montada = 0;

    if (archivo == NULL)
    {
        mensaje("NO SE ENCONTRO DISCO");
        return;
    }
    else
    {
        int ciclo_escribir;
        BUFFER_CONT_DISK llenar;

        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        //Recorre las particones montadas para saber si hay una con el nombre de la particion que se quiere borrar
        for(int i=0; i < 26; i++)
        {
            for(int j = 0; j < 25; j++)
            {
                if (strcmp(part_memoria[i].PART_MONTADA[j].nombre, tmp->name) == 0)
                {
                    montada = 1;
                    break;
                }
            }
        }

        //Recorrera particiones para eliminar primarias y extendidas
        if(montada == 0)
        {
            for (int i = 0; i < 4; i++)
            {
                if (strcmp(tmp_mbr.mbr_particion[i].name, tmp->name) == 0)
                {

                    if (tmp_mbr.mbr_particion[i].type == 'E')
                    {
                        fseek(archivo, tmp_mbr.mbr_particion[i].start, SEEK_SET);
                        fread(&tmp_ebr, sizeof (EBR), 1, archivo);

                        if (tmp_ebr.status == '1' || tmp_ebr.next != -1)
                        {
                            mensaje("PARTICION EXTENDIDA POSEE PARTICIONES LOGICAS");
                            return;
                        }
                        else
                        {

                            //Solo hace esto si es full
                            if (tmp->ban_delete == 2)
                            {
                                ciclo_escribir = tmp_mbr.mbr_particion[i].size / 1024;
                                fseek(archivo, tmp_mbr.mbr_particion[i].start, SEEK_SET);
                                llenar = new_buffer();

                                for (int i = 0; i < ciclo_escribir; i++)
                                {
                                    fwrite(&llenar, sizeof (BUFFER_CONT_DISK), 1, archivo);
                                }
                            }

                            //Siempre hace esto para fast y full
                            tmp_mbr.mbr_particion[i].status = '0';
                            tmp_mbr.mbr_particion[i].type = 0;
                            tmp_mbr.mbr_particion[i].fit = 0;
                            tmp_mbr.mbr_particion[i].start = 0;
                            tmp_mbr.mbr_particion[i].size = 0;
                            strcpy(tmp_mbr.mbr_particion[i].name, "vacia");

                            fprintf(stderr, "****** PARTICION ELIMINADA CON EXITO ******\n");
                            break;
                        }
                        break;
                    }
                    else //Particione primarias
                    {

                        //Solo hace esto si es full
                        if (tmp->ban_delete == 2)
                        {
                            ciclo_escribir = tmp_mbr.mbr_particion[i].size / 1024;
                            fseek(archivo, tmp_mbr.mbr_particion[i].start, SEEK_SET);
                            llenar = new_buffer();
                            for (int i = 0; i < ciclo_escribir; i++)
                            {
                                fwrite(&llenar, sizeof (BUFFER_CONT_DISK), 1, archivo);
                            }
                        }

                        //Siempre hace esto para fast y full
                        tmp_mbr.mbr_particion[i].status = '0';
                        tmp_mbr.mbr_particion[i].type = 0;
                        tmp_mbr.mbr_particion[i].fit = 0;
                        tmp_mbr.mbr_particion[i].start = 0;
                        tmp_mbr.mbr_particion[i].size = 0;
                        strcpy(tmp_mbr.mbr_particion[i].name, "vacia");

                        fprintf(stderr, "****** PARTICION ELIMINADA CON EXITO ******\n");
                        break;
                    }
                }

                if (i == 3)
                {
                    mensaje("LA PARTICION NO EXISTE");
                    break;
                }
            }
        }
        else
        {
            mensaje("NO SE PUEDE ELIMINAR PORQUE ESTA MONTADA");
            return;
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}


void ejecutar_mount(MOUNT* tmp)
{
    ini_particiones_memoria();
    FILE *archivo;
    MBR tmp_mbr;

    if (existe_part(tmp) == 1)
    {
        archivo = fopen(tmp->path, "rb");

        if (archivo == NULL)
        {
            mensaje("DISCO NO ENCONTRADO");
        }
        else
        {
            fseek(archivo, 0, SEEK_SET);
            fread(&tmp_mbr, sizeof (MBR), 1, archivo);

            for (int i = 0; i < 26; i++)
            {
                if (part_memoria[i].ban_vacia == 0) //Verifica si existe el disco
                {

                    strcpy(part_memoria[i].path, tmp->path);
                    part_memoria[i].ban_vacia = 1;
                    part_memoria[i].PART_MONTADA[0].ban_ocupado = 1;
                    part_memoria[i].PART_MONTADA[0].id[0] = 'v';
                    part_memoria[i].PART_MONTADA[0].id[1] = 'd';
                    part_memoria[i].PART_MONTADA[0].id[2] = part_memoria[i].letra;
                    part_memoria[i].PART_MONTADA[0].id[3] = '1';
                    strcpy(part_memoria[i].PART_MONTADA[0].nombre, tmp->name);
                    fprintf(stderr, "PARTICION CON ID = %s MONTADA CON EXITO\n", part_memoria[i].PART_MONTADA[0].id);
                    return;
                }
                else if (strcmp(tmp->path, part_memoria[i].path) == 0)
                {
                    //El disco existe y se le asigna numero
                    for (int j = 0; j < 25; j++)
                    {
                        if (strcmp(part_memoria[i].PART_MONTADA[j].nombre, tmp->name) == 0)
                        {
                            mensaje("PARTICION YA MONTADA");
                            return;
                        }
                        else
                        {
                            if (part_memoria[i].PART_MONTADA[j].ban_ocupado == 0)
                            {
                                part_memoria[i].PART_MONTADA[j].ban_ocupado = 1;
                                part_memoria[i].PART_MONTADA[j].id[0] = 'v';
                                part_memoria[i].PART_MONTADA[j].id[1] = 'd';
                                part_memoria[i].PART_MONTADA[j].id[2] = part_memoria[i].letra;

                                int indice;

                                if ((j + 1) > 9 && (j + 1) < 19)
                                {
                                    indice = (j + 1) % 10;
                                    part_memoria[i].PART_MONTADA[j].id[3] = '1';
                                    part_memoria[i].PART_MONTADA[j].id[4] = indice + '0';
                                }
                                else if ((j + 1) > 19 && (j + 1) < 29)
                                {
                                    indice = (j + 1) % 10;
                                    part_memoria[i].PART_MONTADA[j].id[3] = '2';
                                    part_memoria[i].PART_MONTADA[j].id[4] = indice + '0';
                                }
                                else
                                {
                                    part_memoria[i].PART_MONTADA[j].id[3] = (j + 1) + '0';
                                }


                                strcpy(part_memoria[i].PART_MONTADA[j].nombre, tmp->name);
                                fprintf(stderr, "PARTICION CON ID = %s MONTADA CON EXITO\n", part_memoria[i].PART_MONTADA[j].id);
                                return;
                            }
                        }
                    }
                }
            }
            fclose(archivo);
        }
    }
}

void ejecutar_unmount(UNMOUNT* tmp)
{
    for (int i = 0; i < 26; i++)
    {

        if (part_memoria[i].letra == tmp->id[2])
        {

            for (int j = 0; j < 25; j++)
            {

                if (strcmp(part_memoria[i].PART_MONTADA[j].id, tmp->id) == 0)
                {
                    MOUNT tmp_mount = particion_montada(tmp->id, part_memoria);
                    log_unmount(&tmp_mount);

                    //Limpia el id de la particion
                    part_memoria[i].PART_MONTADA[j].ban_ocupado = 0;
                    part_memoria[i].PART_MONTADA[j].id[0] = '\0';
                    part_memoria[i].PART_MONTADA[j].id[1] = '\0';
                    part_memoria[i].PART_MONTADA[j].id[2] = '\0';
                    part_memoria[i].PART_MONTADA[j].id[3] = '\0';

                    for (int h = 0; h < 50; h++) //Limpia el nombre de la particion
                    {
                        part_memoria[i].PART_MONTADA[j].nombre[h] = '\0';
                    }
                    borrar_disco();
                    fprintf(stderr, "****** LA PARTICION SE HA DESMONTADO CON EXITO ******\n");
                    return;
                }
            }
        }
        if (i == 7)
        {
            mensaje("PARTICION NO ENCONTRADA");
        }
    }
}

void rep_disk(REP* tmp)
{
    char path_disco[150];

        for (int i = 0; i < 26; i++)
        {

            if (part_memoria[i].letra == tmp->id[2])
            {

                for (int j = 0; j < 25; j++)
                {

                    if (strcmp(part_memoria[i].PART_MONTADA[j].id, tmp->id) == 0)
                    {
                        validar_dir(tmp->path);
                        strcpy(path_disco, part_memoria[i].path);
                        reporte_disco(path_disco, tmp->path);
                        mensaje("REPORTE DISK GENERADO CON EXITO");
                        return;
                    }
                }
                mensaje("NO HAY PARTICION MONTADA");
            }
            if (i == 7)
            {
                mensaje("EL DISCO NO ESTA MONTADO");
            }
        }
}

void rep_mbr(REP* tmp)
{
    char path_disco[150];
    for (int i = 0; i < 26; i++) {
        if (part_memoria[i].letra == tmp->id[2])
        {
            for (int j = 0; j < 25; j++)
            {
                if (strcmp(part_memoria[i].PART_MONTADA[j].id, tmp->id) == 0)
                {
                    validar_dir(tmp->path);
                    strcpy(path_disco, part_memoria[i].path);
                    reporte_mbr(path_disco, tmp->path);
                    mensaje("REPORTE MBR GENERADO CON EXITO");
                    return;
                }
            }
            mensaje("NO HAY PARTICION MONTADA");
        }
        if (i == 7)
        {
            mensaje("EL DISCO NO ESTA MONTADO");
        }
    }
}

void borrar_disco()
{
    for (int i = 0; i < 26; i++)
    {
        if (part_memoria[i].ban_vacia == 1)
        {
            for (int j = 0; j < 25; j++)
            {
                if (part_memoria[i].PART_MONTADA[j].ban_ocupado == 0)
                {
                    part_memoria[i].ban_vacia = 0;
                    for (int h = 0; h < 150; h++)
                    {
                        part_memoria[i].path[h] = '\0';
                    }
                }
                else
                {
                    return;
                }
            }
        }
    }
}

void ejecutar_exec(EXEC* tmp)
{
    leer_archivo(tmp->path);
}

void redim_fdisk(FDISK* tmp)
{
    if (tmp->add < 0) {
        reducir(tmp);
    } else {
        aumentar(tmp);
    }
}

void reducir(FDISK* tmp) {
    FILE* archivo;
    MBR tmp_mbr;
    EBR tmp_ebr;
    archivo = fopen(tmp->path, "rb+");

    if (archivo == NULL)
    {
        mensaje("EL DISCO NO ESTA EN LA RUTA");
        return;
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        for (int i = 0; i < 4; i++)
        {
            if (strcmp(tmp_mbr.mbr_particion[i].name, tmp->name) == 0)
            {
                int tam_restar = tam_particion(tmp->ban_unit, tmp->add);
                if (tmp_mbr.mbr_particion[i].type == 'E')
                {
                    int pos_inicio = tmp_mbr.mbr_particion[i].start;

                    while (pos_inicio != -1)
                    {
                        fseek(archivo, pos_inicio, SEEK_SET);
                        fread(&tmp_ebr, sizeof (EBR), 1, archivo);
                        pos_inicio = tmp_ebr.next;
                    }
                    if ((tmp_ebr.start + tmp_ebr.size) < (tmp_mbr.mbr_particion[i].start + tmp_mbr.mbr_particion[i].size + tam_restar))
                    {
                        tmp_mbr.mbr_particion[i].size = (tmp_mbr.mbr_particion[i].size + tam_restar);
                        fprintf(stderr, "PARTICION REDUCIDA CON EXITO --- tam anterior: %d, tam nuevo: %d\n", (tmp_mbr.mbr_particion[i].size - tam_restar), tmp_mbr.mbr_particion[i].size);
                        break;
                    }
                    else
                    {
                        mensaje("EXCEDE EL INICIO DE LA PARTICION");
                    }

                }
                else
                {
                    if (tmp_mbr.mbr_particion[i].size > ((-1) * tam_restar))
                    {
                        tmp_mbr.mbr_particion[i].size = (tmp_mbr.mbr_particion[i].size + tam_restar);
                        fprintf(stderr, "PARTICION REDUCIDA CON EXITO --- tam anterior: %d, tam nuevo: %d\n", (tmp_mbr.mbr_particion[i].size - tam_restar), tmp_mbr.mbr_particion[i].size);
                        break;
                    }
                    else
                    {
                        mensaje("EXCEDE EL INICIO DE LA PARTICION");
                    }
                }
            }

            if (i == 3) //Busca en las particiones logicas
            {

                for (int i = 0; i < 4; i++)
                {
                    if (tmp_mbr.mbr_particion[i].type == 'E')
                    {
                        int pos_inicio = tmp_mbr.mbr_particion[i].start;

                        while (pos_inicio != -1)
                        {
                            fseek(archivo, pos_inicio, SEEK_SET);
                            fread(&tmp_ebr, sizeof (EBR), 1, archivo);

                            if (strcmp(tmp_ebr.name, tmp->name) == 0)
                            {
                                int tam_restar = tam_particion(tmp->ban_unit, tmp->add);

                                if (tmp_ebr.size > ((-1) * tam_restar))
                                {
                                    tmp_ebr.size = (tmp_ebr.size + tam_restar);

                                    fseek(archivo, pos_inicio, SEEK_SET);
                                    fwrite(&tmp_ebr, sizeof (EBR), 1, archivo);

                                    fprintf(stderr, "PARTICION REDUCIDA CON EXITO --- tam anterior: %d, tam nuevo: %d\n", (tmp_ebr.size - tam_restar), tmp_ebr.size);

                                } else {
                                    mensaje("EXCEDE EL INICIO DE LA PARTICION");
                                }
                                fclose(archivo);
                                return;
                            }

                            pos_inicio = tmp_ebr.next;
                        }
                    }
                }
                mensaje("PARTICION NO ENCONTRADA");
            }
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}

void aumentar(FDISK* tmp) {
    FILE* archivo;
    MBR tmp_mbr;
    EBR tmp_ebr;

    archivo = fopen(tmp->path, "rb+");
    if (archivo == NULL)
    {
        mensaje("EL DISCO NO ESTA EN LA RUTA");
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        for (int i = 0; i < 4; i++)
        {

            if (strcmp(tmp_mbr.mbr_particion[i].name, tmp->name) == 0)
            {
                int tam_aumentar = tam_particion(tmp->ban_unit, tmp->add);

                if (aumentar_primaria(&tmp_mbr, i, tam_aumentar) == 0)
                {
                    mensaje("NO ES POSBILE AUMENTAR");
                }
                else
                {
                    tmp_mbr.mbr_particion[i].size = (tmp_mbr.mbr_particion[i].size + tam_aumentar);
                    fprintf(stderr, "PARTICION AUMENTADA CON EXITO --- tam anterior: %d, tam nuevo: %d\n", (tmp_mbr.mbr_particion[i].size - tam_aumentar), tmp_mbr.mbr_particion[i].size);
                    break;
                }
            }
        }

        fseek(archivo, 0, SEEK_SET);
        fwrite(&tmp_mbr, sizeof (MBR), 1, archivo);
        fclose(archivo);
    }
}


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

void ejecutar_mkfs(MKFS *tmp)
{
    MOUNT tmp_mount = particion_montada(tmp->id, part_memoria);

    MOUNT_USUARIO = tmp_mount;

    if(tmp_mount.ban_error == 1)
    {
        mensaje("LA PARTICION NO ESTA MONTADA");
    }
    else
    {
        FILE *ARCHIVO;
        MBR tmp_mbr;
        PARTICION tmp_particion;
        char caracter = '0';

        ARCHIVO = fopen(tmp_mount.path, "rb+");

        if(ARCHIVO == NULL)
        {
            mensaje("ERROR CON RUTA");
        }
        else
        {
            fseek(ARCHIVO, 0, SEEK_SET);
            fread(&tmp_mbr, sizeof(MBR), 1, ARCHIVO);

            for(int i = 0; i < 4; i++)
            {
                if (strcmp(tmp_mbr.mbr_particion[i].name, tmp_mount.name) == 0)
                {
                    tmp_particion = tmp_mbr.mbr_particion[i];

                    SUPER_BLOQUE tmp_super_bloque;

                    fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
                    fread(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

                    if (tmp_super_bloque.s_filesystem_type == 0)
                    {
                        formatear_particion(tmp->ban_fs, &tmp_particion, &tmp_super_bloque);
                        fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
                        fwrite(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

                        fseek(ARCHIVO, tmp_super_bloque.s_bm_inode_start, SEEK_SET);

                        for (int j = 0; j < tmp_super_bloque.s_inodes_count; j++)
                        {
                            fwrite(&caracter, sizeof (char), 1, ARCHIVO);
                        }

                        fseek(ARCHIVO, tmp_super_bloque.s_block_start, SEEK_SET);

                        for (int j = 0; j < tmp_super_bloque.s_blocks_count; j++)
                        {
                            fwrite(&caracter, sizeof (char), 1, ARCHIVO);
                        }

                        fclose(ARCHIVO);

                        escribir_inodo_carpeta(&tmp_mount, "/", 1, 1, '0', 664);
                        escribir_inodo_archivo(&tmp_mount, "/users.txt", "1, G, root  \n1, U, root  , root  , 123  \n2, G, usuario   \n2, U, usuario, chicas, chicas123\n", 1, 1, '1', 664);
                        fprintf(stderr, "FORMATEO EXITOSO\n");
                        return;
                    }
                    else
                    {
                        fprintf(stderr, "PARTICION ESTA FORMATIADA\n");
                        break;
                    }
                }
            }
        }

    }
}
