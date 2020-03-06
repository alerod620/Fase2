#ifndef VALIDAR_COMANDO_H
#define VALIDAR_COMANDO_H

#include <estructuras.h>
#include <estructuras_disco.h>

void mensaje(char* mensaje);
void validar_dir(char* path);

int posicion_disk(MBR* tmp, int no_particion);
int tam_particion(int ban_unit, int size);

void escribir_particion_mbr(MBR* tmp_mbr, FDISK* tmp_fdisk, int no_particion, int pos_inicio, int t_particion);
void escribir_particion_ebr(EBR* tmp_ebr, FDISK* tmp_fdisk, int pos_inicio, int t_particion, int next);

int validar_p1(MBR* tmp, int tam_part, int* pos_inicio);
int validar_p2(MBR* tmp, int tam_part, int* pos_inicio);
int validar_p3(MBR* tmp, int tam_part, int* pos_inicio);
int validar_p4(MBR* tmp, int tam_part, int* pos_inicio);
int validar_pri(MBR* tmp);
int validar_ext(MBR* tmp);
int existe_part(MOUNT* tmp);
int aumentar_primaria(MBR *tmp_mbr, int no_part, int tam_aumentar); //Verifica si se puede aumentar

/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

void log_mount(MOUNT* tmp);
void log_unmount(MOUNT* tmp);

MOUNT particion_montada(char id[], PARTICIONES_MONTADAS lista[]);
PARTICION buscar_particion_montada(MOUNT* tmp);
int numero_estructura(int tam_particion, int formato);


void formatear_particion(int ban_fs, PARTICION *tmp_particion, SUPER_BLOQUE* tmp_super_bloque);

LOGIN obtener_usuario(char contenido[], int *posicion);
void obtener_usuario_login(LOGIN* tmp_usuario, char contenido[], LOGIN* tmp);
void obtener_grupo_usuario(char contenido[], char nombre_grupo[], int* ban_grupo);

#endif // VALIDAR_COMANDO_H
