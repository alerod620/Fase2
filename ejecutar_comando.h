#ifndef EJECUTAR_COMANDO_H
#define EJECUTAR_COMANDO_H

#include <estructuras.h>

void ejecutar_mkdisk(MKDISK* tmp);
void ejecutar_rmdisk(RMDISK* tmp);
void ejecutar_mount(MOUNT* tmp);
void ejecutar_unmount(UNMOUNT* tmp);
void ejecutar_eliminar(FDISK* tmp);

//Comandos de FDISK
void crear_fdisk(FDISK* tmp);
void crear_primaria(FDISK* tmp);
void crear_extendida(FDISK* tmp);
void crear_logica(FDISK* tmp);
void redim_fdisk(FDISK* tmp);
void aumentar(FDISK* tmp);
void reducir(FDISK* tmp);

//Reportes
void rep_disk(REP* tmp);
void rep_mbr(REP* tmp);

void ejecutar_exec(EXEC* tmp);
void borrar_disco();


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

void ejecutar_mkfs(MKFS* tmp);
void ejecutar_login(LOGIN* tmp);
void ejecutar_logout(LOGOUT* tmp);
void ejecutar_mkgrp(MKGRP* tmp);
void ejecutar_rmgrp(RMGRP* tmp);
void ejecutar_mkusr(MKUSR* tmp);
void ejecutar_rmusr(RMUSR* tmp);
void ejecutar_rmusr(RMUSR* tmp);
void ejecutar_chmod(CHMOD* tmp);
void ejecutar_mkfile(MKFILE* tmp);
void ejecutar_cat(CAT* tmp);
void ejecutar_rem(REM* tmp);
void ejecutar_edit(EDIT* tmp);
void ejecutar_ren(REN* tmp);
void ejecutar_mkdir(MKDIR* tmp);
void ejecutar_cp(CP* tmp);
void ejecutar_mv(MV* tmp);
void ejecutar_find(FIND* tmp);
void ejecutar_chown(CHOWN* tmp);
void ejecutar_chgrp(CHGRP* tmp);
void ejecutar_recovery(RECOVERY* tmp);
void ejecutar_loss(LOSS* tmp);


void ejecutar_rep_bm_inodos(REP* tmp);
void ejecutar_rep_bm_bloques(REP* tmp);
void ejecutar_rep_sb(REP* tmp);
void ejecutar_rep_inodes(REP* tmp);
void ejecutar_rep_blocks(REP* tmp);
void ejecutar_rep_tree(REP* tmp);
void ejecutar_rep_file(REP* tmp);


#endif // EJECUTAR_COMANDO_H
