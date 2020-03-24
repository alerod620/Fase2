#include <string.h>

#include <escribir_disco.h>
#include <validar_comando.h>
#include <analizar_parametro.h>

/*

Los valores de la variable existe nodo sirve para:

**** 0 -> No existe inodo
**** 1 -> El inodo carpeta existe, en busqueda de carpeta
**** 2 -> Se llego a la ultima carpeta en el sistema y se puede generar las necesarias con p en archivos para crear el archivo
**** 3 -> No existe archivo
**** 4 -> Existe archivo

*/

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


void limpiar_variables_buscar_inodo()
{
    for (int i = 0; i < 1024; i++)
    {
        tmp_path_inodo[i] = '\0';
    }
}

void nombre_inodo()
{
    int indice = 0;

    limpiar_variables_buscar_inodo();

    while (path_contenido[indice_path_contenido] != '\0')
    {
        if (path_contenido[indice_path_contenido] == '"' && indice_path_contenido != 0)
        {
            break;
        }
        else if (path_contenido[indice_path_contenido] == '"' && indice_path_contenido == 0)
        {
            indice_path_contenido++;
        }
        else if (path_contenido[indice_path_contenido] == '/' && path_contenido[indice_path_contenido + 1] == '\0' && indice_path_contenido == 0)
        {
            break;
        }
        else if (path_contenido[indice_path_contenido] == '/' && path_contenido[indice_path_contenido + 1] != '\0' && indice_path_contenido == 0)
        {
            indice_path_contenido++;
        }
        else if (path_contenido[indice_path_contenido] == '/' && path_contenido[indice_path_contenido + 1] != '\0')
        {
            indice_path_contenido++;
            break;
        }
        else
        {
            tmp_path_inodo[indice] = path_contenido[indice_path_contenido];
            indice_path_contenido++;
            indice++;
        }
    }
}

void buscar_inodo_carpeta(int numero_inodo, char *name)
{
    int i = 0;
    fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_inode_start + (numero_inodo * sizeof (INODO)), SEEK_SET);
    fread(&TMP_INODO, sizeof (INODO), 1, ARCHIVO_DISCO);

    for (i = 0; i < 12; i++)
    {

        if (TMP_INODO.i_block[i] != -1)
        {
            fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (sizeof (BLOQUE_ARCHIVO) * TMP_INODO.i_block[i]), SEEK_SET);

            if (TMP_INODO.i_type == '0')
            {
                fread(&TMP_CARPETA, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);

                if (strcmp(name, "") == 0)
                {
                    num_bloque_inodo = i;
                    existe_inodo = 1;
                    return;
                }
                else
                {

                    for (int j = 0; j < 4; j++)
                    {
                        //Compara si la ruta es la misma
                        if (strcmp(name, TMP_CARPETA.b_content[j].b_name) == 0)
                        {
                            nombre_inodo();
                            buscar_inodo_carpeta(TMP_CARPETA.b_content[j].b_inodo, tmp_path_inodo);
                            if (existe_inodo == 1)
                                return;
                        }
                        else
                        {
                            if (TMP_CARPETA.b_content[j].b_inodo == -1)
                            {
                                num_bloque_inodo = i;
                                existe_inodo = 1;
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}

void limpiar_variables_escribir()
{
    indice_path_contenido = 0;

    for (int i = 0; i < 10000; i++)
    {

        if (i < 1024)
        {
            path_contenido[i] = '\0';
            file_name[i] = '\0';
            tmp_path_inodo[i] = '\0';
        }
        contenido[i] = '\0';
    }
}

void inicializar_variables_escribir(MOUNT* particion, char* path_cont, char *cont)
{
    limpiar_variables_escribir();
    PARTICION_USO = buscar_particion_montada(particion);

    if (particion->ban_error == 0)
    {
        ARCHIVO_DISCO = fopen(particion->path, "rb+");

        if (ARCHIVO_DISCO == NULL)
        {
            mensaje("ERROR CON LA RUTA");
        }
        else
        {
            fseek(ARCHIVO_DISCO, PARTICION_USO.start, SEEK_SET);
            fread(&SUPER_BLOQUE_PARTICION, sizeof (SUPER_BLOQUE), 1, ARCHIVO_DISCO);

            strcpy(path_contenido, path_cont);

            for (int i = 0; i < 10000; i++)
            {

                if (cont[i] == '\0')
                {
                    contenido[i] = cont[i];
                    tam_contenido = i;
                    break;
                }
                contenido[i] = cont[i];
            }
            nombre_archivo();
        }
    }
}

void inicializar_variables_leer(MOUNT* particion, char* path_cont)
{
    limpiar_variables_escribir();
    PARTICION_USO = buscar_particion_montada(particion);

    if (particion->ban_error == 0)
    {
        ARCHIVO_DISCO = fopen(particion->path, "rb");

        if (ARCHIVO_DISCO == NULL)
        {
            mensaje("ERROR CON PATH");
        }
        else
        {
            fseek(ARCHIVO_DISCO, PARTICION_USO.start, SEEK_SET);
            fread(&SUPER_BLOQUE_PARTICION, sizeof (SUPER_BLOQUE), 1, ARCHIVO_DISCO);

            strcpy(path_contenido, path_cont);
        }
    }
}

void nombre_archivo()
{
    int indice = 0;
    int indice_name = 0;

    if (path_contenido[indice] != '"')
    {

        while (path_contenido[indice] != ' ' && path_contenido[indice] != '\n' && path_contenido[indice] != '\0')
        {

            if (path_contenido[indice] == '/')
            {

                if (indice == 0 && path_contenido[indice + 1] == '\0')
                {
                    file_name[indice_name] = path_contenido[indice];
                    break;
                }
                else
                {
                    limpiar_variable_file_name();
                    indice++;
                    indice_name = 0;
                }

            }
            else
            {
                file_name[indice_name] = path_contenido[indice];
                indice_name++;
                indice++;
            }
        }
    } else {
        indice++;
        while (path_contenido[indice] != '"') {
            if (path_contenido[indice] == '/') {
                if (indice == 0 && path_contenido[indice + 1] == '\0') {
                    file_name[indice_name] = path_contenido[indice];
                    break;
                } else {
                    limpiar_variable_file_name();
                    indice++;
                    indice_name = 0;
                }
            } else {
                file_name[indice_name] = path_contenido[indice];
                indice_name++;
                indice++;
            }
        }
    }
}

void limpiar_variable_file_name()
{
    for (int i = 0; i < 1024; i++)
    {
        file_name[i] = '\0';
    }
}

void actualizar_super_bloque()
{
    fseek(ARCHIVO_DISCO, PARTICION_USO.start, SEEK_SET);
    fwrite(&SUPER_BLOQUE_PARTICION, sizeof (SUPER_BLOQUE_PARTICION), 1, ARCHIVO_DISCO);
}

void actualizar_bm_inodos(char caracter, int numero_inodo)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_bm_inode_start + numero_inodo;
    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&caracter, sizeof (char), 1, ARCHIVO_DISCO);
}

void actualizar_bm_bloques(char caracter, int numero_bloque)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_bm_block_start + numero_bloque;
    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&caracter, sizeof (char), 1, ARCHIVO_DISCO);
}

void actualizar_inodos(int numero_inodo)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_inode_start + (numero_inodo * sizeof (INODO));
    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&TMP_INODO, sizeof (INODO), 1, ARCHIVO_DISCO);
}

void actualizar_bloque_apuntador(int numero_bloque)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_block_start + (numero_bloque * sizeof (BLOQUE_APUNTADOR));
    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&TMP_APUNTADOR, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);
}

/*
****************************************
*************** CARPETAS ***************
****************************************
*/


void escribir_inodo_carpeta(MOUNT* particion, char* path_cont, int i_uid, int i_gid, char i_type, int i_perm)
{
    inicializar_variables_escribir(particion, path_cont, "");

    if (ARCHIVO_DISCO != NULL) {

        if (SUPER_BLOQUE_PARTICION.s_first_ino == 0) {
            //NO HAY INODOS EN EL SISTEMA DE ARCHIVOS
            TMP_INODO = new_inodo();
            TMP_INODO.i_uid = i_uid;
            TMP_INODO.i_gid = i_gid;
            TMP_INODO.i_size = tam_contenido;
            TMP_INODO.i_type = i_type;
            TMP_INODO.i_perm = i_perm;
            TMP_INODO.i_ctime = obtener();
            TMP_INODO.i_mtime = obtener();
            TMP_INODO.i_atime = obtener();
            TMP_INODO.i_block[0] = SUPER_BLOQUE_PARTICION.s_first_blo;
            escribir_bloque_carpeta(0, SUPER_BLOQUE_PARTICION.s_first_ino, 0);
        } else {
            existe_inodo = 0;
            nombre_inodo();
            buscar_inodo_carpeta(0, tmp_path_inodo);
            actualizar_inodo_bloque_carpeta();

            TMP_INODO = new_inodo();
            TMP_INODO.i_uid = i_uid;
            TMP_INODO.i_gid = i_gid;
            TMP_INODO.i_size = tam_contenido;
            TMP_INODO.i_type = i_type;
            TMP_INODO.i_perm = i_perm;
            TMP_INODO.i_ctime = obtener();
            TMP_INODO.i_mtime = obtener();
            TMP_INODO.i_atime = obtener();
            TMP_INODO.i_block[0] = SUPER_BLOQUE_PARTICION.s_first_blo;
            escribir_bloque_carpeta(TMP_CARPETA.b_content[0].b_inodo, SUPER_BLOQUE_PARTICION.s_first_ino, 0);
        }

        caracter_archivo = '1';
        actualizar_bm_inodos(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_ino);

        actualizar_inodos(SUPER_BLOQUE_PARTICION.s_first_ino);

        SUPER_BLOQUE_PARTICION.s_first_ino++;
        SUPER_BLOQUE_PARTICION.s_free_inodes_count--;
        actualizar_super_bloque();

        fclose(ARCHIVO_DISCO);
    }

}

void escribir_bloque_carpeta(int inodo_padre, int inodo_actual, int numero_apuntador)
{
    TMP_CARPETA = new_bloque_carpeta();

    if (numero_apuntador == 0) {
        TMP_CARPETA.b_content[0].b_inodo = inodo_actual;
        TMP_CARPETA.b_content[0].b_name[0] = '.';
        TMP_CARPETA.b_content[1].b_inodo = inodo_padre;
        strcpy(TMP_CARPETA.b_content[1].b_name, "..");
    }

    caracter_archivo = '1';
    actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);

    actualizar_bloque_carpeta(SUPER_BLOQUE_PARTICION.s_first_blo);

    SUPER_BLOQUE_PARTICION.s_first_blo++;
    SUPER_BLOQUE_PARTICION.s_free_blocks_count--;
}

void actualizar_inodo_bloque_carpeta()
{
    int i = 0;
    if (TMP_INODO.i_type == '0') {
        for (i = 0; i < 4; i++) {
            if (TMP_CARPETA.b_content[i].b_inodo == -1) {
                TMP_CARPETA.b_content[i].b_inodo = SUPER_BLOQUE_PARTICION.s_first_ino;
                strcpy(TMP_CARPETA.b_content[i].b_name, file_name);\
                actualizar_bloque_carpeta(TMP_INODO.i_block[num_bloque_inodo]);
                break;
            }
        }
        if (i == 4) {
            for (i = 0; i < 12; i++) {
                if (TMP_INODO.i_block[i] == -1) {
                    TMP_INODO.i_block[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                    actualizar_inodos(TMP_CARPETA.b_content[0].b_inodo);
                    escribir_bloque_carpeta(TMP_CARPETA.b_content[1].b_inodo, TMP_CARPETA.b_content[0].b_inodo, i);

                    TMP_CARPETA.b_content[0].b_inodo = SUPER_BLOQUE_PARTICION.s_first_ino;
                    strcpy(TMP_CARPETA.b_content[0].b_name, file_name);\
                    actualizar_bloque_carpeta(TMP_INODO.i_block[i]);

                    actualizar_super_bloque();
                    return;
                }
            }
            /*
            if (i == 12) {
                if (TMP_INODO.i_block[12] != -1) {
                    fseek(ARCHIVO_DISCO, 4741600, SEEK_SET);
                    fread(&TMP_APUNTADOR, sizeof (BLOQUE_APUNTADOR), 1, ARCHIVO_DISCO);
                    if (TMP_INODO.i_type == '0') {
                        for (i = 0; i < 16; i++) {
                            if (TMP_APUNTADOR.b_pointers[i] != 1) {
                                fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (TMP_APUNTADOR.b_pointers[i] * sizeof (BLOQUE_CARPETA)), SEEK_SET);
                                fread(&TMP_CARPETA, sizeof (BLOQUE_APUNTADOR), 1, ARCHIVO_DISCO);
                                for (i = 0; i < 4; i++) {
                                    if (TMP_CARPETA.b_content[i].b_inodo == -1) {
                                        TMP_CARPETA.b_content[i].b_inodo = SUPER_BLOQUE_PARTICION.s_first_ino;
                                        strcpy(TMP_CARPETA.b_content[i].b_name, file_name);
                                        actualizar_bloque_carpeta(TMP_APUNTADOR.b_pointers[i]);
                                        return;
                                    }
                                }
                            } else {
                                TMP_APUNTADOR.b_pointers[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                                actualizar_bloque_apuntador(TMP_INODO.i_block[12]);

                                escribir_bloque_carpeta(TMP_CARPETA.b_content[1].b_inodo, TMP_CARPETA.b_content[0].b_inodo);

                                TMP_CARPETA.b_content[2].b_inodo = SUPER_BLOQUE_PARTICION.s_first_ino;
                                strcpy(TMP_CARPETA.b_content[2].b_name, file_name);\
                                actualizar_bloque_carpeta(TMP_APUNTADOR.b_pointers[i]);

                                actualizar_super_bloque();
                                return;
                            }
                        }
                    }
                } else {
                    TMP_INODO.i_block[12] = SUPER_BLOQUE_PARTICION.s_first_blo;
                    actualizar_inodos(TMP_CARPETA.b_content[0].b_inodo);
                    escribir_bloque_apuntador_carpeta(TMP_INODO.i_block[12], TMP_CARPETA.b_content[1].b_inodo, TMP_CARPETA.b_content[0].b_inodo);
                    return;
                }
            }
             */
        }
    }
}

void escribir_bloque_apuntador_carpeta(int numero_apuntador_de_inodo, int inodo_padre, int inodo_actual)
{
    TMP_APUNTADOR = new_bloque_apuntador();

    TMP_APUNTADOR.b_pointers[0] = SUPER_BLOQUE_PARTICION.s_first_blo + 1;

    caracter_archivo = '1';
    actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);
    actualizar_bloque_apuntador(numero_apuntador_de_inodo);

    SUPER_BLOQUE_PARTICION.s_first_blo++;
    SUPER_BLOQUE_PARTICION.s_free_blocks_count--;

    escribir_bloque_carpeta(inodo_padre, inodo_actual, 12);

    TMP_CARPETA.b_content[2].b_inodo = SUPER_BLOQUE_PARTICION.s_first_ino;
    strcpy(TMP_CARPETA.b_content[2].b_name, file_name);\
    actualizar_bloque_carpeta(TMP_APUNTADOR.b_pointers[0]);

    actualizar_super_bloque();
}

void actualizar_bloque_carpeta(int numero_bloque)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_block_start + (numero_bloque * sizeof (BLOQUE_CARPETA));
    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&TMP_CARPETA, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);
}


/*
****************************************
*************** ARCHIVOS ***************
****************************************
*/

void escribir_inodo_archivo(MOUNT* particion, char* path_cont, char* cont, int i_uid, int i_gid, char i_type, int i_perm) {
    inicializar_variables_escribir(particion, path_cont, cont);

    if (ARCHIVO_DISCO != NULL) {
        existe_inodo = 0;
        nombre_inodo();
        buscar_inodo_carpeta(0, tmp_path_inodo);
        actualizar_inodo_bloque_carpeta();

        TMP_INODO = new_inodo();
        TMP_INODO.i_uid = i_uid;
        TMP_INODO.i_gid = i_gid;
        TMP_INODO.i_size = tam_contenido;
        TMP_INODO.i_type = i_type;
        TMP_INODO.i_perm = i_perm;
        TMP_INODO.i_ctime = obtener();
        TMP_INODO.i_mtime = obtener();
        TMP_INODO.i_atime = obtener();

        int numero_block_ocupar = (tam_contenido) / 64;
        indice_contenido = 0;

        if (numero_block_ocupar < 12) {
            for (int i = 0; i <= numero_block_ocupar; i++) {
                TMP_INODO.i_block[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                escribir_bloque_archivo();
            }
        } else {
            for (int i = 0; i < 12; i++) {
                TMP_INODO.i_block[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                escribir_bloque_archivo();
                numero_block_ocupar--;
            }
            if (TMP_INODO.i_block[12] == -1) {
                TMP_INODO.i_block[12] = SUPER_BLOQUE_PARTICION.s_first_blo;

                TMP_APUNTADOR = new_bloque_apuntador();
                caracter_archivo = '1';
                actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);
                actualizar_bloque_apuntador(TMP_INODO.i_block[12]);
                SUPER_BLOQUE_PARTICION.s_first_blo++;
                SUPER_BLOQUE_PARTICION.s_free_blocks_count--;
                actualizar_super_bloque();

                for (int i = 0; i < 16; i++) {
                    if (numero_block_ocupar < 0) {
                        break;
                    } else {
                        if (TMP_APUNTADOR.b_pointers[i] == -1) {
                            TMP_APUNTADOR.b_pointers[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                            escribir_bloque_archivo();
                            actualizar_bloque_apuntador(TMP_INODO.i_block[12]);
                            numero_block_ocupar--;
                        }
                    }
                }
            }

            //BLOQUE 13
            /*if (numero_block_ocupar >= 0) {
                BLOQUE_APUNTADOR tmp_apuntador;

                if (TMP_INODO.i_block[13] == -1) {
                    TMP_INODO.i_block[13] = SUPER_BLOQUE_PARTICION.s_first_blo;

                    TMP_APUNTADOR = new_bloque_apuntador();
                    caracter_archivo = '1';
                    actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);
                    actualizar_bloque_apuntador(TMP_INODO.i_block[12]);
                    SUPER_BLOQUE_PARTICION.s_first_blo++;
                    SUPER_BLOQUE_PARTICION.s_free_blocks_count--;
                    actualizar_super_bloque();
                    tmp_apuntador = TMP_APUNTADOR;

                    for (int i = 0; i < 16; i++) {
                        if (numero_block_ocupar < 0) {
                            break;
                        } else {
                            if (tmp_apuntador.b_pointers[i] == -1) {
                                tmp_apuntador.b_pointers[i] = SUPER_BLOQUE_PARTICION.s_first_blo;
                                actualizar_bloque_apuntador(TMP_INODO.i_block[13]);

                                TMP_APUNTADOR = new_bloque_apuntador();
                                caracter_archivo = '1';
                                actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);
                                actualizar_bloque_apuntador(tmp_apuntador.b_pointers[i]);
                                SUPER_BLOQUE_PARTICION.s_first_blo++;
                                SUPER_BLOQUE_PARTICION.s_free_blocks_count--;
                                actualizar_super_bloque();

                                for (int j = 0; j < 16; j++) {
                                    if (numero_block_ocupar < 0) {
                                        break;
                                    } else {
                                        if (TMP_APUNTADOR.b_pointers[j] == -1) {
                                            TMP_APUNTADOR.b_pointers[j] = SUPER_BLOQUE_PARTICION.s_first_blo;
                                            escribir_bloque_archivo();
                                            actualizar_bloque_apuntador(tmp_apuntador.b_pointers[i]);
                                            numero_block_ocupar--;
                                        }
                                    }
                                }

                            }
                        }
                    }
                }
            }*/

        }


        caracter_archivo = '1';
        actualizar_bm_inodos(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_ino);
        actualizar_inodos(SUPER_BLOQUE_PARTICION.s_first_ino);

        SUPER_BLOQUE_PARTICION.s_first_ino++;
        SUPER_BLOQUE_PARTICION.s_free_inodes_count--;
        actualizar_super_bloque();

        fclose(ARCHIVO_DISCO);
    }
}

void escribir_bloque_archivo() {
    TMP_ARCHIVO = new_bloque_archivo();

    for (int i = 0; i < 64; i++)
    {
        if (contenido[indice_contenido] == '\0')
        {
            break;
        }
        else
        {
            TMP_ARCHIVO.b_content[i] = contenido[indice_contenido];
            indice_contenido++;
        }
    }

    caracter_archivo = '1';
    actualizar_bm_bloques(caracter_archivo, SUPER_BLOQUE_PARTICION.s_first_blo);

    actualizar_bloque_archivo(SUPER_BLOQUE_PARTICION.s_first_blo);

    SUPER_BLOQUE_PARTICION.s_first_blo++;
    SUPER_BLOQUE_PARTICION.s_free_blocks_count--;
}

void actualizar_bloque_archivo(int numero_bloque)
{
    int posicion = SUPER_BLOQUE_PARTICION.s_block_start + (numero_bloque * sizeof (BLOQUE_CARPETA));

    fseek(ARCHIVO_DISCO, posicion, SEEK_SET);
    fwrite(&TMP_ARCHIVO, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);
}

MI_ARCHIVO buscar_archivo_disco(MOUNT* particion, char* path_cont)
{
    inicializar_variables_leer(particion, path_cont);

    existe_inodo = 0;
    nombre_inodo();
    buscar_inodo_archivo(0, tmp_path_inodo);

    MI_ARCHIVO tmp = new_archivo();

    if (existe_inodo == 3)
    {
        mensaje("EL ARCHIVO NO EXISTE");
        return tmp;
    }
    else if (existe_inodo == 2)
    {
        mensaje("SE LLEGO A LA ULTIMA CARPETA, SIN PODER ENCONTRAR ARCHIVO");
        return tmp;
    }
    else if (existe_inodo == 4)
    {
        return obtener_contenido_archivo_lectura();
    }

}

MI_ARCHIVO obtener_contenido_archivo_lectura()
{
    MI_ARCHIVO tmp = new_archivo();
    BLOQUE_ARCHIVO tmp_b_a;
    BLOQUE_APUNTADOR tmp_b_ap;
    int posicion = 0;

    if (TMP_INODO.i_block[12] == -1)
    {

        for (int i = 0; i <= 12; i++)
        {

            if (TMP_INODO.i_block[i] != -1)
            {
                fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (TMP_INODO.i_block[i] * sizeof (BLOQUE_ARCHIVO)), SEEK_SET);
                fread(&tmp_b_a, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);

                for (int i = 0; i < 64; i++)
                {
                    tmp.contenido[posicion] = tmp_b_a.b_content[i];
                    posicion++;
                }
            }
        }
    }
    else
    {

        for (int i = 0; i < 12; i++)
        {
            fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (TMP_INODO.i_block[i] * sizeof (BLOQUE_ARCHIVO)), SEEK_SET);
            fread(&tmp_b_a, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);

            for (int i = 0; i < 64; i++)
            {
                tmp.contenido[posicion] = tmp_b_a.b_content[i];
                posicion++;
            }
        }

        if (TMP_INODO.i_block[12] != -1)
        {
            fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (TMP_INODO.i_block[12] * sizeof (BLOQUE_ARCHIVO)), SEEK_SET);
            fread(&tmp_b_ap, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);

            for (int i = 0; i < 16; i++)
            {
                if (tmp_b_ap.b_pointers[i] != -1)
                {
                    fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (tmp_b_ap.b_pointers[i] * sizeof (BLOQUE_ARCHIVO)), SEEK_SET);
                    fread(&tmp_b_a, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);

                    for (int i = 0; i < 64; i++)
                    {
                        tmp.contenido[posicion] = tmp_b_a.b_content[i];
                        posicion++;
                    }
                }
            }
        }
    }

    TMP_INODO.i_atime = obtener();
    actualizar_inodos(num_bloque_inodo);

    tmp.existe_archivo = 1;
    return tmp;
}

void buscar_inodo_archivo(int numero_ino, char name[])
{
    int i = 0;
    fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_inode_start + (numero_ino * sizeof (INODO)), SEEK_SET);
    fread(&TMP_INODO, sizeof (INODO), 1, ARCHIVO_DISCO);

    if (TMP_INODO.i_type == '1')
    {
        num_bloque_inodo = i;
        existe_inodo = 4;
        return;
    }
    else
    {

        for (i = 0; i < 12; i++)
        {

            if (TMP_INODO.i_block[i] != -1)
            {
                fseek(ARCHIVO_DISCO, SUPER_BLOQUE_PARTICION.s_block_start + (sizeof (BLOQUE_ARCHIVO) * TMP_INODO.i_block[i]), SEEK_SET);

                if (TMP_INODO.i_type == '0')
                {
                    fread(&TMP_CARPETA, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);

                    if (strcmp(name, "") == 0)
                    {
                        num_bloque_inodo = i;
                        existe_inodo = 3;
                        return;
                    }
                    else
                    {

                        for (int j = 0; j < 4; j++)
                        {
                            //Compara si la ruta es la misma
                            if (strcmp(name, TMP_CARPETA.b_content[j].b_name) == 0)
                            {
                                nombre_inodo();
                                buscar_inodo_archivo(TMP_CARPETA.b_content[j].b_inodo, tmp_path_inodo);
                                return;
                            }
                        }
                    }
                }
                else
                {
                    num_bloque_inodo = i;
                    existe_inodo = 4;
                    return;
                }
            }
            else
            {
                num_bloque_inodo = i;
                existe_inodo = 2;
                return;
            }
        }
    }
}

