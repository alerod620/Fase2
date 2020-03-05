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

typedef struct login
{
    char usr[12];
    char pwd[12];
    char fs[12];
    int ban_usr;
    int ban_pwd;
    int ban_id;
    int ban_error;
} LOGIN;

typedef struct logout
{
    int ban_logout;
    int ban_error;
} LOGOUT;

typedef struct mkgrp
{
    char name[12];
    int ban_name;
    int ban_error;
} MKGRP;

typedef struct rmgrp
{
    char name[12];
    int ban_name;
    int ban_error;
} RMGRP;


typedef struct mkusr
{
    char usr[12];
    char pwd[12];
    char grp[12];
    int ban_usr;
    int ban_pwd;
    int ban_grp;
    int ban_error;
} MKUSR;

typedef struct rmusr
{
    char usr[12];
    int ban_usr;
    int ban_error;
} RMUSR;

typedef struct chmod
{
    char path[255];
    char ugo[3];
    int ban_path;
    int ban_ugo;
    int ban_r;
    int ban_error;
} CHMOD;

typedef struct mkfile
{
    char path[255];
    char cont[255];
    int size;
    int ban_path;
    int ban_p;
    int ban_size;
    int ban_cont;
    int ban_error;
} MKFILE;

typedef struct cat
{
    char path[255];
    int ban_file;
    int ban_error;
} CAT;

typedef struct rem
{
    char path[255];
    int ban_path;
    int ban_error;
} REM;

typedef struct edit
{
    char path[255];
    char cont[255];
    int ban_path;
    int ban_cont;
    int ban_error;
} EDIT;

typedef struct ren
{
    char path[255];
    char name[100];
    int ban_path;
    int ban_name;
    int ban_error;
} REN;

typedef struct mkdir
{
    char path[255];
    int ban_path;
    int ban_p;
    int ban_error;
} MKDIR;

typedef struct cp
{
    char path[255];
    char dest[255];
    int ban_path;
    int ban_dest;
    int ban_error;
} CP;

typedef struct mv
{
    char path[255];
    char dest[255];
    int ban_path;
    int ban_dest;
    int ban_error;
} MV;

typedef struct find
{
    char path[255];
    char name[100];
    int ban_path;
    int ban_name;
    int ban_error;
} FIND;

typedef struct chown
{
    char path[255];
    char usr[12];
    int ban_path;
    int ban_r;
    int ban_usr;
    int ban_error;
} CHOWN;

typedef struct chgrp
{
    char usr[12];
    char grp[12];
    int ban_usr;
    int ban_grp;
    int ban_error;
} CHGRP;

typedef struct recovery
{
    char id[10];
    int ban_id;
    int ban_error;
} RECOVERY;

typedef struct loss
{
    char id[10];
    int ban_id;
    int ban_error;
} LOSS;

MKFS nuevo_mkfs();
LOGIN nuevo_login();
LOGOUT nuevo_logout();
MKGRP nuevo_mkgrp();
RMGRP nuevo_rmgrp();
MKUSR nuevo_mkusr();
RMUSR nuevo_rmusr();
CHMOD nuevo_chmod();
MKFILE nuevo_mkfile();
CAT nuevo_cat();
REM nuevo_rem();
EDIT nuevo_edit();
REN nuevo_ren();
MKDIR nuevo_mkdir();
CP nuevo_cp();
MV nuevo_mv();
FIND nuevo_find();
CHOWN nuevo_chown();
CHGRP nuevo_chgrp();
RECOVERY nuevo_recovery();
LOSS nuevo_loss();

#endif // ESTRUCTURAS_H
