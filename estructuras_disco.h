#ifndef ESTRUCTURAS_DISCO_H
#define ESTRUCTURAS_DISCO_H

#include <time.h>
#include <estructuras.h>

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


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

typedef struct super_bloque
{
    int s_filesystem_type;
    int s_inodes_count;
    int s_blocks_count;
    int s_free_blocks_count;
    int s_free_inodes_count;
    time_t s_mtime;
    time_t s_umtime;
    int s_mnt_count;
    int s_magic;
    int s_inode_size;
    int s_block_size;
    int s_first_ino;
    int s_first_blo;
    int s_bm_inode_start;
    int s_bm_block_start;
    int s_inode_start;
    int s_block_start;
} SUPER_BLOQUE;

typedef struct journaling {
    int j_tipo_operacion;
    int j_tipo;
    int j_permisos;
    char j_nombre[100];
    char j_contenido[100];
    time_t j_fecha;
    char j_propietario[12];
} JOURNALING;

typedef struct inodo
{
    int i_uid;
    int i_gid;
    int i_size;
    time_t i_atime;
    time_t i_ctime;
    time_t i_mtime;
    int i_block[15];
    char i_type;
    int i_perm;
} INODO;

typedef struct content {
    char b_name[12];
    int b_inodo;
} CONTENT;

typedef struct bloque_carpeta {
    CONTENT b_content[4];
} BLOQUE_CARPETA;

typedef struct bloque_archivo {
    char b_content[64];
} BLOQUE_ARCHIVO;

typedef struct bloque_apuntador {
    int b_pointers[16];
} BLOQUE_APUNTADOR;

SUPER_BLOQUE new_super_bloque();
JOURNALING new_journaling();
INODO new_inodo();
CONTENT new_content();
BLOQUE_CARPETA new_bloque_carpeta();
BLOQUE_ARCHIVO new_bloque_archivo();
BLOQUE_APUNTADOR new_bloque_apuntador();

//USUARIO LOGIADO

typedef struct usuario_sistema
{
    int ban_usuario;
    int numero_usuario;
    int numero_grupo;
    char nombre[12];
    char grupo[12];
    char password[12];
    char id_particion[12];
    MOUNT mount_particion;
    PARTICION particion_uso;
} USUARIO_SISTEMA;

USUARIO_SISTEMA new_usuario_sistema();


//ARCHIVO

typedef struct mi_archivo
{
    int existe_archivo;
    char contenido[5000];
} MI_ARCHIVO;

MI_ARCHIVO new_archivo();


#endif // ESTRUCTURAS_DISCO_H
