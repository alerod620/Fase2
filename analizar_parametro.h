#ifndef ANALIZAR_PARAMETRO_H
#define ANALIZAR_PARAMETRO_H

#include <estructuras.h>

void mayusculas(char* cadena);
void limpiar_buffer(int buffer);
void iniciar_buffer(char* parametro);

//*********************************************
//****** Metodos para obtener parametros ******
//*********************************************
void get_id(char* valor, int* ban);
void get_name(char* valor, int* ban);
void get_path(char* valor, int* ban);
void get_type(int* ban);
void get_delete(int* ban);
void get_fit(int* ban);
void get_unit(int* ban);
void get_size(char* valor, int* ban);
void get_add(char* valor, int* ban);
void get_path(char* valor, int* ban);

void v_e_fdisk(char* parametro, int* ban);
void parametro_error(char *comando);

//*********************************************
//********** Metodos para analizar ************
//*********************************************
MKDISK parametros_mkdisk(char parametros[]);
RMDISK parametros_rmdisk(char parametros[]);
FDISK parametros_fdisk(char parametros[]);
MOUNT parametros_mount(char parametros[]);
UNMOUNT parametros_unmount(char parametros[]);
REP parametros_rep(char parametros[]);
EXEC parametros_exec(char parametros[]);

/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

//*********************************************
//****** Metodos para obtener parametros ******
//*********************************************
void get_usr(char valor[], int* ban);
void get_pwd(char valor[], int* ban);
void get_grp(char valor[], int* ban);
void get_ugo(char valor[], int* ban);
void get_fs(int* ban);
void get_particion(int* ban);
void get_formateo(int* ban);

//*********************************************
//********** Metodos para analizar ************
//*********************************************
MKFS parametros_mkfs(char parametros[]);
LOGIN parametros_login(char parametros[]);
LOGOUT parametros_logout(char parametros[]);
MKGRP parametros_mkgrp(char parametros[]);
RMGRP parametros_rmgrp(char parametros[]);
MKUSR parametros_mkusr(char parametros[]);
RMUSR parametros_rmusr(char parametros[]);
CHMOD parametros_chmod(char parametros[]);
MKFILE parametros_mkfile(char parametros[]);
CAT parametros_cat(char parametros[]);
REM parametros_rem(char parametros[]);
EDIT parametros_edit(char parametros[]);
REN parametros_ren(char parametros[]);
MKDIR parametros_mkdir(char parametros[]);
CP parametros_cp(char parametros[]);
MV parametros_mv(char parametros[]);
FIND parametros_find(char parametros[]);
CHOWN parametros_chown(char parametros[]);
CHGRP parametros_chgrp(char parametros[]);
RECOVERY parametros_recovery(char parametros[]);
LOSS parametros_loss(char parametros[]);

#endif // ANALIZAR_PARAMETRO_H
