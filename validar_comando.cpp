#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <libgen.h>

#include <validar_comando.h>

void mensaje(char* mensaje)
{
    fprintf(stderr, "****** %s ******\n", mensaje);
}


void validar_dir(char* tmp)
{
    DIR *directorio;
    int pos_tmp_path = 0;
    char tmp_path[255];
    for(int i = 0; i < 255; i++)
    {
        tmp_path[i] = '\0';
    }

    for(int i = 0; i <255; i++)
    {
        if(tmp[i] > 0)
        {
            tmp_path[pos_tmp_path] = tmp[i];
            if(tmp_path[pos_tmp_path] == '/')
            {
                if((directorio = opendir(tmp_path)) == NULL) //Verifica si el directorio existe y de no hacerlo, lo crea
                {
                    mkdir(tmp_path, 0777);
                }
                else
                {
                    closedir(directorio);
                }
            }
            pos_tmp_path++;
        }
    }
}

int posicion_disk(MBR *tmp, int no_particion)
{
    int posicion = sizeof (MBR);
    switch(no_particion)
    {
    case 0:
        break;

    case 1:
        if(tmp->mbr_particion[0].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[0].size;
        }
        break;

    case 2:
        if(tmp->mbr_particion[0].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[0].size;
        }

        if(tmp->mbr_particion[1].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[1].size;
        }
        break;

    case 3:
        if(tmp->mbr_particion[0].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[0].size;
        }

        if(tmp->mbr_particion[1].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[1].size;
        }

        if(tmp->mbr_particion[2].status == '1')
        {
            posicion = posicion + tmp->mbr_particion[2].size;
        }
        break;
    }
    posicion++;
    return posicion;
}

int tam_particion(int ban_unit, int size)
{
    int tam_part = 0;
    switch (ban_unit)
    {
    case 1:
        tam_part = size*1024;
        break;
    case 2:
        tam_part = size*1024*1024;
        break;
    case 3:
        tam_part = size;
        break;
    }
    return tam_part;
}

void escribir_particion_mbr(MBR *tmp_mbr, FDISK *tmp_fdisk, int no_particion, int pos_inicio, int t_particion)
{
    tmp_mbr->mbr_particion[no_particion].status = '1';

    switch (tmp_fdisk->ban_type) //Seleccionara el tipo de particion que es
    {
    case 1: //Particion primaria
        tmp_mbr->mbr_particion[no_particion].type = 'P';
        break;
    case 2: //Particion extendida
        tmp_mbr->mbr_particion[no_particion].type = 'E';
        break;
    case 3: //Particion logica
        tmp_mbr->mbr_particion[no_particion].type = 'L';
        break;
    }

    switch (tmp_fdisk->ban_fit) //Seleccionara que ajuste tendra la particion
    {
    case 1: //Mejor ajuste
        tmp_mbr->mbr_particion[no_particion].fit = 'B';
        break;
    case 2: //Primer ajuste
        tmp_mbr->mbr_particion[no_particion].fit = 'F';
        break;

    case 3: //Peor ajuste
        tmp_mbr->mbr_particion[no_particion].fit = 'W';
        break;
    }
    tmp_mbr->mbr_particion[no_particion].start = pos_inicio;
    tmp_mbr->mbr_particion[no_particion].size = t_particion;
    strcpy(tmp_mbr->mbr_particion[no_particion].name, tmp_fdisk->name);
    fprintf(stderr, "****** SE HA CREADO LA PARTICION CON EXITO ******\n");
}

void escribir_particion_ebr(EBR *tmp_ebr, FDISK *tmp_fdisk, int pos_inicio, int t_particion, int next)
{
    tmp_ebr->status = '1';
    tmp_ebr->start = pos_inicio;
    tmp_ebr->size = t_particion;
    tmp_ebr->next = next;

    switch (tmp_fdisk->ban_fit) //Indica el ajuste de la particion
    {
    case 1: //Mejor ajuste
        tmp_ebr->fit = 'B';
        break;
    case 2: //Primer ajuste
        tmp_ebr->fit = 'F';
        break;
    case 3: //Peor ajuste
        tmp_ebr->fit = 'W';
        break;
    }

    strcpy(tmp_ebr->name, tmp_fdisk->name);
    fprintf(stderr, "****** SE HA CREADO LA PARTICION CON EXITO ******\n");
}

int validar_p1(MBR *tmp, int tam_part, int *pos_inicio)
{

    if (tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0') //Verifica si existen las otras particiones
    {
        return 1;
    } else if (tmp->mbr_particion[1].status == '1') //Si la particion 2 existe
    {
        if ((*pos_inicio + tam_part) > tmp->mbr_particion[1].start)
        {
            *pos_inicio = (tmp->mbr_particion[1].start + tmp->mbr_particion[1].size + 1);

            if (tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0')
            {
                return 1;
            } else if (tmp->mbr_particion[2].status == '1')
            {

                if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                {
                    *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                    if (tmp->mbr_particion[3].status == '0')
                    {
                        return 1;
                    }
                    else
                    {
                        if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                        {
                            *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                            if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                            {
                                mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                                return 0;
                            } else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            } else
            {

                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {

                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if (tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0')
        {
            return 1;
        }
        else if (tmp->mbr_particion[2].status == '1')
        {

            if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
            {
                *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {
                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if (tmp->mbr_particion[3].status == '0')
            {
                return 1;
            }
            else
            {
                if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                {
                    *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                    if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                    {
                        mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }

}

int validar_p2(MBR *tmp, int tam_part, int *pos_inicio)
{
    if (tmp->mbr_particion[0].status == '0' && tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0')
    {
        return 1;
    }
    else if (tmp->mbr_particion[0].status == '1')
    {

        if ((*pos_inicio + tam_part) > tmp->mbr_particion[0].start)
        {
            *pos_inicio = (tmp->mbr_particion[0].start + tmp->mbr_particion[0].size + 1);

            if (tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0')
            {
                return 1;
            }
            else if (tmp->mbr_particion[2].status == '1')
            {
                if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                {
                    *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                    if (tmp->mbr_particion[3].status == '0')
                    {
                        return 1;
                    }
                    else
                    {
                        if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                        {
                            *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                            if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                            {
                                mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {

                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {

                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {

        if (tmp->mbr_particion[2].status == '0' && tmp->mbr_particion[3].status == '0')
        {
            return 1;
        }
        else if (tmp->mbr_particion[2].status == '1')
        {

            if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
            {
                *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {
                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if (tmp->mbr_particion[3].status == '0')
            {
                return 1;
            }
            else
            {
                if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                {
                    *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                    if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                    {
                        mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

int validar_p3(MBR *tmp, int tam_part, int *pos_inicio)
{
    if (tmp->mbr_particion[0].status == '0' && tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[3].status == '0')
    {
        return 1;
    }
    else if (tmp->mbr_particion[0].status == '1')
    {

        if ((*pos_inicio + tam_part) > tmp->mbr_particion[0].start)
        {
            *pos_inicio = (tmp->mbr_particion[0].start + tmp->mbr_particion[0].size + 1);

            if (tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[3].status == '0')
            {
                return 1;
            }
            else if (tmp->mbr_particion[1].status == '1')
            {
                if ((*pos_inicio + tam_part) > tmp->mbr_particion[1].start)
                {
                    *pos_inicio = (tmp->mbr_particion[1].start + tmp->mbr_particion[1].size + 1);

                    if (tmp->mbr_particion[3].status == '0')
                    {
                        return 1;
                    }
                    else
                    {
                        if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                        {
                            *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                            if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                            {
                                mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                                return 0;
                            }
                            else
                                return 1;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {
                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {
                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {

        if (tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[3].status == '0')
        {
            return 1;
        }
        else if (tmp->mbr_particion[1].status == '1')
        {

            if ((*pos_inicio + tam_part) > tmp->mbr_particion[1].start)
            {
                *pos_inicio = (tmp->mbr_particion[1].start + tmp->mbr_particion[1].size + 1);

                if (tmp->mbr_particion[3].status == '0')
                {
                    return 1;
                }
                else
                {

                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {
            if (tmp->mbr_particion[3].status == '0')
            {
                return 1;
            }
            else
            {

                if ((*pos_inicio + tam_part) > tmp->mbr_particion[3].start)
                {
                    *pos_inicio = (tmp->mbr_particion[3].start + tmp->mbr_particion[3].size + 1);

                    if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                    {
                        mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

int validar_p4(MBR *tmp, int tam_part, int *pos_inicio)
{
    if (tmp->mbr_particion[0].status == '0' && tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[2].status == '0')
    {
        return 1;
    }
    else if (tmp->mbr_particion[0].status == '1')
    {

        if ((*pos_inicio + tam_part) > tmp->mbr_particion[0].start)
        {
            *pos_inicio = (tmp->mbr_particion[0].start + tmp->mbr_particion[0].size + 1);

            if (tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[2].status == '0')
            {
                return 1;
            }
            else if (tmp->mbr_particion[1].status == '1')
            {

                if ((*pos_inicio + tam_part) > tmp->mbr_particion[1].start)
                {
                    *pos_inicio = (tmp->mbr_particion[1].start + tmp->mbr_particion[1].size + 1);

                    if (tmp->mbr_particion[2].status == '0')
                    {
                        return 1;
                    }
                    else
                    {

                        if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                        {
                            *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                            if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                            {
                                mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                                return 0;
                            }
                            else
                            {
                                return 1;
                            }
                        }
                        else
                        {
                            return 1;
                        }
                    }
                }
                else
                {
                    return 1;
                }
            }
            else
            {

                if (tmp->mbr_particion[2].status == '0')
                {
                    return 1;
                }
                else
                {

                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
        }
        else
        {
            return 1;
        }
    }
    else
    {

        if (tmp->mbr_particion[1].status == '0' && tmp->mbr_particion[2].status == '0')
        {
            return 1;
        }
        else if (tmp->mbr_particion[1].status == '1')
        {

            if ((*pos_inicio + tam_part) > tmp->mbr_particion[1].start)
            {
                *pos_inicio = (tmp->mbr_particion[1].start + tmp->mbr_particion[1].size + 1);

                if (tmp->mbr_particion[2].status == '0')
                {
                    return 1;
                }
                else
                {

                    if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                    {
                        *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                        if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                        {
                            mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                    }
                    else
                    {
                        return 1;
                    }
                }
            }
            else
            {
                return 1;
            }
        }
        else
        {

            if (tmp->mbr_particion[2].status == '0')
            {
                return 1;
            }
            else
            {

                if ((*pos_inicio + tam_part) > tmp->mbr_particion[2].start)
                {
                    *pos_inicio = (tmp->mbr_particion[2].start + tmp->mbr_particion[2].size + 1);

                    if ((*pos_inicio + tam_part) > tmp->mbr_tam)
                    {
                        mensaje("ERROR NO HAY SUFICIENTE ESPACIO");
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                }
                else
                {
                    return 1;
                }
            }
        }
    }
}

int validar_ext(MBR *tmp)
{
    for(int i = 0; i < 4; i++)
    {
        if(tmp->mbr_particion[i].type == 'E') //Compara el tipo para encontrar E
        {
            return 1;
        }
    }
    return 0;
}

int validar_pri(MBR* tmp)
{
    int cont = 0;
    for(int i = 0; i < 4; i++)
    {
        if(tmp->mbr_particion[i].type == 'P')
        {
            cont=cont+1;
        }
    }
    if(cont == 3)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int existe_part(MOUNT *tmp)
{
    FILE* archivo;
    EBR tmp_ebr;
    MBR tmp_mbr;
    int pos_inicio = 0;
    archivo = fopen(tmp->path, "rb");

    if(archivo == NULL)
    {
        mensaje("RUTA INCORRECTA");
    }
    else
    {
        fseek(archivo, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo);

        for (int i = 0; i < 4; i++) //Recorre las particiones para saber si es una particion primaria o extendida
        {
            if (strcmp(tmp_mbr.mbr_particion[i].name, tmp->name) == 0)
            {
                fclose(archivo);
                return 1;
            }
        }

        //De no estar en las particiones primarias o extendidas

        for (int i = 0; i < 4; i++) //Recorre las particiones para encontrar la particion extendida
        {
            if (tmp_mbr.mbr_particion[i].type == 'E')
            {
                pos_inicio = tmp_mbr.mbr_particion[i].start;

                while (1)
                {
                    fseek(archivo, pos_inicio, SEEK_SET);
                    fread(&tmp_ebr, sizeof (EBR), 1, archivo);

                    if (strcmp(tmp_ebr.name, tmp->name) == 0) //Compara el nombre con el de las particiones logicas
                    {
                        fclose(archivo);
                        return 1;
                    } else
                    {
                        if (tmp_ebr.next == -1) //Compara para ver si es la ultima particion logica
                        {
                            mensaje("NO EXISTE LA PARTICION");
                            fclose(archivo);
                            return 0;
                        }
                        else
                        {
                            pos_inicio = tmp_ebr.next;
                        }
                    }
                }
            }
        }
        return 0;

    }

}

int aumentar_primaria(MBR *tmp_mbr, int no_part, int tam_aumentar)
{
    int aumentar = 0; //1 = se puede aumentar **  0 = no se puede aumentar
    int pos_disk = tmp_mbr->mbr_particion[no_part].start + tmp_mbr->mbr_particion[no_part].size + tam_aumentar; //Suma la posicion donde inicia la particion + el tamaño de la particion + el tamaño que se desea aumentar

    switch (no_part) //Comparara la posicion en donde quedara el disco con la posicion de inicio de las demas particiones, segun sea el caso
    {
    case 0:
        if(pos_disk > tmp_mbr->mbr_particion[1].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[2].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[3].start)
        {
            aumentar = 0;
            break;
        }
        aumentar = 1;
        break;

    case 1:
        if(pos_disk > tmp_mbr->mbr_particion[0].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[2].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[3].start)
        {
            aumentar = 0;
            break;
        }
        aumentar = 1;
        break;

    case 2:
        if(pos_disk > tmp_mbr->mbr_particion[0].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[1].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[3].start)
        {
            aumentar = 0;
            break;
        }
        aumentar = 1;
        break;

    case 3:
        if(pos_disk > tmp_mbr->mbr_particion[0].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[1].start)
        {
            aumentar = 0;
            break;
        }
        if(pos_disk > tmp_mbr->mbr_particion[2].start)
        {
            aumentar = 0;
            break;
        }
        aumentar = 1;
        break;
    }
}



/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

void log_mount(MOUNT *tmp_mount) {
    FILE *ARCHIVO;

    ARCHIVO = fopen(tmp_mount->path, "rb+");

    if (ARCHIVO == NULL) {
        mensaje("ERROR CON PATH");
    } else {
        SUPER_BLOQUE tmp_super_bloque;
        PARTICION tmp_particion = buscar_particion_montada(tmp_mount);

        fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
        fread(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

        tmp_super_bloque.s_mtime = obtener();
        tmp_super_bloque.s_mnt_count++;

        fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
        fwrite(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

        fclose(ARCHIVO);
    }
}

void log_unmount(MOUNT *tmp_mount) {
    FILE *ARCHIVO;

    ARCHIVO = fopen(tmp_mount->path, "rb+");

    if (ARCHIVO == NULL) {
        mensaje("ERROR CON PATH");
    } else {
        SUPER_BLOQUE tmp_super_bloque;
        PARTICION tmp_particion = buscar_particion_montada(tmp_mount);

        fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
        fread(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

        tmp_super_bloque.s_umtime = obtener();

        fseek(ARCHIVO, tmp_particion.start, SEEK_SET);
        fwrite(&tmp_super_bloque, sizeof (SUPER_BLOQUE), 1, ARCHIVO);

        fclose(ARCHIVO);
    }
}
