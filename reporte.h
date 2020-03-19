#ifndef REPORTE_H
#define REPORTE_H

#include <escribir_disco.h>

void reporte_disco(char* path_disco,char* path_destino_imagen);
void reporte_mbr(char* path_disco,char* path_destino_imagen);

double porcentaje_p(double tam_part, double tam_mbr);
double porcentaje(double pos_inicio,double pos_fin,double tam_mbr);

void obtener_formato(char path[]);

void limpiar();

/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/



void reporte_bm_inodos(char* path_disco, int bm_start, int bm_count, char* path_destino_reporte);
void reporte_bm_bloques(char* path_disco, int bm_start, int bm_count, char* path_destino_reporte);
void reporte_sb(SUPER_BLOQUE *sb, char *path_destino);
void reporte_inodes(char* path_disco, char* path_destino, int start_particion);
void reporte_blocks(char* path_disco, char* path_destino, int start_particion);
void reporte_tree(char* path_disco, char* path_destino, int start_particion);
void tree_recursivo(char* path_disco, int start_inodos, int start_bloques, int numero_inodo);
void reporte_file(char* path_destino, MI_ARCHIVO contenido);

char* get_name_disk(char* path);

void limpiar_tree();

#endif // REPORTE_H
