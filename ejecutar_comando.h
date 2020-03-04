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

#endif // EJECUTAR_COMANDO_H
