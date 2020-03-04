#ifndef COMANDO_H
#define COMANDO_H

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
//****************** Comandos *****************
//*********************************************
void mkdisk(char parametro[]);
void rmdisk(char parametro[]);
void fdisk(char parametro[]);
void mount(char parametro[]);
void unmount(char parametro[]);
void rep(char parametro[]);
void exec(char parametro[]);

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

void path_raid(char tmp[]);

#endif // COMANDO_H
