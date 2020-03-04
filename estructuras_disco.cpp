#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <estructuras_disco.h>

time_t obtener()
{
    time_t t = time(0);
    return t;
}

char* formato_fecha(time_t t)
{
    char* f_h = (char*) malloc(sizeof (char)*128);
    struct tm *tiempo_local = localtime(&t);
    strftime(f_h, 128, "%d/%m/%y %H:%M", tiempo_local);
    return f_h;
}

int id_random()
{
    int random;
    srand(time(NULL));
    random = 1 + rand() % ((1 + 100) - 1);
    return random;
}

BUFFER_CONT_DISK new_buffer()
{
    BUFFER_CONT_DISK nuevo;
    for(int i = 0; i < 1024; i++)
    {
        nuevo.info[i] = '0';
    }
    return nuevo;
}

MBR new_mbr(int tam, int fit)
{
    MBR nuevo;
    nuevo.mbr_tam = tam;
    nuevo.mbr_fecha = obtener();
    nuevo.mbr_disk_sig = id_random();

    switch (fit) {
    case 1:
        nuevo.mbr_fit = 'B';
        break;
    case 2:
        nuevo.mbr_fit = 'F';
        break;
    case 3:
        nuevo.mbr_fit = 'W';
        break;
    }

    for(int i = 0; i < 4; i++)
    {
        nuevo.mbr_particion[i] = new_part('0',0,0,0,0);
    }
    return nuevo;
}

PARTICION new_part(char status, char type, char fit, int start, int size)
{
    PARTICION nueva;
    nueva.status = status;
    nueva.type = type;
    nueva.fit = fit;
    nueva.start = start;
    nueva.size = size;

    for(int i = 0; i < 16; i++)
    {
        nueva.name[i] = '\0';
    }

    //Poner nombre 'vacia' a la particion
    nueva.name[0] = 'v';
    nueva.name[1] = 'a';
    nueva.name[2] = 'c';
    nueva.name[3] = 'i';
    nueva.name[4] = 'a';
    return nueva;
}

EBR new_ebr(char status, char fit, int start, int size)
{
    EBR nuevo;
    nuevo.status = status;
    nuevo.fit = fit;
    nuevo.start = start;
    nuevo.size = size;
    nuevo.next = -1;

    for(int i = 0; i < 16; i++)
    {
        nuevo.name[i] = '\0';
    }

    //Poner nombre 'vacia' a la particion
    nuevo.name[0] = 'v';
    nuevo.name[1] = 'a';
    nuevo.name[2] = 'c';
    nuevo.name[3] = 'i';
    nuevo.name[4] = 'a';
    return nuevo;
}

void impr_mbr(MBR* tmp)
{
    int tam_disco = tmp->mbr_tam/1024;
    fprintf(stderr, "Tam      : %d KB\n", tam_disco);
    fprintf(stderr, "Fecha    : %s\n", formato_fecha(tmp->mbr_fecha));
    fprintf(stderr, "Signature: %d\n", tmp->mbr_disk_sig);

    for(int i = 0; i < 4; i++)
    {
        fprintf(stderr, "Particion %d     : %s\n", tmp->mbr_particion[i].name);
    }

    for(int i = 0; i < 4; i++)
    {
        if(tmp->mbr_particion[i].status == '1')
        {
            fprintf(stderr, "%s\n", "-----------");
            impr_part(&tmp->mbr_particion[i]);
        }
    }
}

void impr_ebr(int pos_inicio, char disco[])
{
    EBR tmp;
    FILE* archivo;
    archivo = fopen(disco, "rb+");
    if(archivo == NULL)
    {

    }
    else
    {
        while (1)
        {
            fseek(archivo, pos_inicio, SEEK_SET)    ;
            fread(&tmp, sizeof (EBR), 1, archivo);
            if(tmp.status == '1')
            {
                fprintf(stderr, "Status : %c\n", tmp.start);
                fprintf(stderr, "Fit    : %c\n", tmp.fit);
                fprintf(stderr, "Start  : %d\n", tmp.start);
                fprintf(stderr, "Size   : %d\n", tmp.size);
                fprintf(stderr, "Name   : %s\n", tmp.name);
                fprintf(stderr, "Next   : %d\n", tmp.next);
                fprintf(stderr, "--------------------------------\n");
                if(tmp.next == -1)
                {
                    break;
                }
                else
                {
                    pos_inicio = tmp.next;
                }
            }
            else
            {
                break;
            }
        }
        fclose(archivo);
    }
}

void impr_part(PARTICION* tmp)
{
    if(tmp->status == '1')
    {
        fprintf(stderr, "Status: %c\n", tmp->start);
        fprintf(stderr, "Type  : %c\n", tmp->type);
        fprintf(stderr, "Fit   : %c\n", tmp->fit);
        fprintf(stderr, "Start : %d\n", tmp->start);
        fprintf(stderr, "Size  : %d %s\n", tmp->size, "Bytes");
        fprintf(stderr, "Name  : %s\n", tmp->name);
        fprintf(stderr, "---------------------------------\n");
    }
}
