#ifndef ESCRIBIR_DISCO_H
#define ESCRIBIR_DISCO_H

#include <stdio.h>
#include <estructuras_disco.h>
#include <estructuras.h>

FILE* ARCHIVO_DISCO;
SUPER_BLOQUE SUPER_BLOQUE_PARTICION;
PARTICION PARTICION_USO;
INODO TMP_INODO;
BLOQUE_CARPETA TMP_CARPETA;
BLOQUE_ARCHIVO TMP_ARCHIVO;
BLOQUE_APUNTADOR TMP_APUNTADOR;

char contenido[10000];
int tam_contenido;
char file_name[1024];

char caracter_archivo;

char path_contenido[1024];
char tmp_path_inodo[1024];
int indice_path_contenido;
int num_bloque_inodo;
int num_bloque_apuntador;

int indice_contenido;
int existe_inodo;

void buscar_inodo_carpeta(int numero_ino, char name[]);
void limpiar_variables_buscar_inodo();
void nombre_inodo();
void limpiar_variables_escribir();
void nombre_archivo();
void limpiar_variable_file_name();
void inicializar_variables_escribir(MOUNT* particion, char* path_contenido, char contenido[]);
void escribir_inodo_carpeta(MOUNT* particion, char* path_contenido, int i_uid, int i_gid, char i_type, int i_perm);
void escribir_inodo_archivo(MOUNT* particion, char* path_contenido, char* contenidio, int i_uid, int i_gid, char i_type, int i_perm);
void escribir_bloque_carpeta(int padre, int actual, int numero_apuntador);
void escribir_bloque_apuntador_carpeta(int numero_apuntador_de_inodo, int inodo_padre, int inodo_actual);
void escribir_bloque_archivo();

void actualizar_inodo_bloque_carpeta();
void actualizar_super_bloque();
void actualizar_bm_inodos(char caracter, int numero_inodo);
void actualizar_bm_bloques(char caracter, int numero_bloque);
void actualizar_inodos(int numero_inodo);
void actualizar_bloque_carpeta(int numero_bloque);
void actualizar_bloque_archivo(int numero_bloque);
void actualizar_bloque_apuntador(int numero_bloque);

void inicializar_variables_leer(MOUNT* particion, char* path_contenido);
void buscar_inodo_archivo(int numero_ino, char name[]);
MI_ARCHIVO buscar_archivo_disco(MOUNT* particion, char* path_contenido);
MI_ARCHIVO obtener_contenido_archivo_lectura();

#endif // ESCRIBIR_DISCO_H
