#ifndef ESTRUCTURAS_DISCO_H
#define ESTRUCTURAS_DISCO_H

#include <time.h>

#define tam_buffer 1024

typedef struct particion
{
    char name[16];
    char status;
    char type;
    char fit;
    int start;
    int size;
}PARTICION;

typedef struct mbr
{
    int mbr_tam;
    time_t mbr_fecha;
    int mbr_disk_sig;
    char mbr_fit;
    PARTICION mbr_particion[4];
}MBR;

typedef struct ebr
{
    char name[16];
    char status;
    char fit;
    int start;
    int size;
    int next;
}EBR;

typedef struct buffer
{
    char info[1024];
}BUFFER_CONT_DISK;

time_t obtener();
char* formato_fecha(time_t);
int id_random();

BUFFER_CONT_DISK new_buffer();
MBR new_mbr(int tam, int fit);
PARTICION new_part(char status, char type, char fit, int start, int size);
EBR new_ebr(char status, char fit, int start, int size);

void impr_mbr(MBR* tmp);
void impr_ebr(int pos_inicio, char disco[]);
void impr_part(PARTICION* tmp);

typedef struct part_mount
{
    int ban_ocupado;
    char nombre[50];
    char id[10];
}PART_MOUNT;

typedef struct particiones_montadas
{
    int ban_vacia;
    char letra;
    char path[150];
    PART_MOUNT PART_MONTADA[25];
}PARTICIONES_MONTADAS;


#endif // ESTRUCTURAS_DISCO_H
