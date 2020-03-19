#ifndef ESCRIBIR_DISCO_H
#define ESCRIBIR_DISCO_H

#include <estructuras_disco.h>
#include <estructuras.h>

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
