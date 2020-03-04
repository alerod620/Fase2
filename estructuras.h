#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

/*
***********************************
**** VALORES PARA LAS BANDERAS ****
***********************************

FIT
*** 1 = mejor ajuste
*** 2 = primer ajuste
*** 3 = peor ajuste
*** 4 = error

UNIT
*** 1 = kilobytes
*** 2 = megabytes
*** 3 = bytes
*** 4 = error

SIZE
*** 0 = no se agrego
*** 1 = agrega valor
*** 2 = error

TYPE
*** 1 = primaria
*** 2 = extendida
*** 3 = logica
*** 4 = error

DELETE
*** 1 = fast
*** 2 = full
*** 3 = error

PATH - NAME - ID - ADD
*** 1 = si ingreso parametro
*** 0 = no ingreso el parametro

*/

typedef struct mkdisk
{
    char path[255];
    int ban_fit;
    int ban_size;
    int ban_path;
    int ban_unit;
    int size;
    int ban_error;
} MKDISK;

typedef struct rmdisk
{
    char path[255];
    int ban_path;
    int ban_error;
} RMDISK;

typedef struct fdisk
{
    char path[255];
    char name[255];
    int ban_size;
    int ban_unit;
    int ban_type;
    int ban_path;
    int ban_fit;
    int ban_delete;
    int ban_name;
    int ban_add;
    int ban_s_d_a;
    int size;
    int add;
    int ban_error;
} FDISK;

typedef struct mount
{
    char path[255];
    char name[255];
    int ban_name;
    int ban_path;
    int ban_error;
} MOUNT;

typedef struct unmount
{
    char id[10];
    int ban_id;
    int ban_error;
} UNMOUNT;

typedef struct rep
{
    char path[255];
    char name[255];
    char id[255];
    int ban_name;
    int ban_path;
    int ban_id;
    int ban_error;
} REP;

typedef struct exec
{
    char path[255];
    int ban_path;
    int ban_error;
} EXEC;

MKDISK nuevo_mkdisk();
RMDISK nuevo_rmdisk();
FDISK nuevo_fdisk();
MOUNT nuevo_mount();
UNMOUNT nuevo_unmount();
REP nuevo_rep();
EXEC nuevo_exec();

void impr_mkdisk(MKDISK* tmp);
void impr_rmdisk(RMDISK* tmp);
void impr_fdisk(FDISK* tmp);
void impr_mount(MOUNT* tmp);
void impr_unmount(UNMOUNT* tmp);
void impr_rep(REP* tmp);
void impr_exec(EXEC* tmp);

void particiones_memoria();



/*
 ************************************************
 ******************** FASE 2 ********************
 ************************************************
*/

typedef struct mkfs
{
    char id[10];
    int ban_id;
    int ban_type;
    int ban_fs;
    int ban_error;
}MKFS;

#endif // ESTRUCTURAS_H
