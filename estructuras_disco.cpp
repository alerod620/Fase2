#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <estructuras_disco.h>

time_t obtener()
{
    time_t t = time(0);
    return t;
}

char* formato_fecha(time_t t)
{
    //char *f_h = (char*) malloc(sizeof (char)*128);
    char f_h[128];
    struct tm *tiempo_local;
    tiempo_local = localtime(&t);
    strftime(f_h, 128, "%d/%m/%y %H:%M", tiempo_local);
    return f_h;
}

int id_random()
{
    int random;
    srand(time(NULL));
    random = 1 + rand() % ((1 + 100) - 1);
    return random;
}

BUFFER_CONT_DISK new_buffer()
{
    BUFFER_CONT_DISK nuevo;
    for(int i = 0; i < 1024; i++)
    {
        nuevo.info[i] = '0';
    }
    return nuevo;
}

MBR new_mbr(int tam, int fit)
{
    MBR nuevo;
    nuevo.mbr_tam = tam;
    nuevo.mbr_fecha = obtener();
    nuevo.mbr_disk_sig = id_random();

    switch (fit) {
    case 1:
        nuevo.mbr_fit = 'B';
        break;
    case 2:
        nuevo.mbr_fit = 'F';
        break;
    case 3:
        nuevo.mbr_fit = 'W';
        break;
    }

    for(int i = 0; i < 4; i++)
    {
        nuevo.mbr_particion[i] = new_part('0',0,0,0,0);
    }
    return nuevo;
}

PARTICION new_part(char status, char type, char fit, int start, int size)
{
    PARTICION nueva;
    nueva.status = status;
    nueva.type = type;
    nueva.fit = fit;
    nueva.start = start;
    nueva.size = size;

    for(int i = 0; i < 16; i++)
    {
        nueva.name[i] = '\0';
    }

    //Poner nombre 'vacia' a la particion
    nueva.name[0] = 'v';
    nueva.name[1] = 'a';
    nueva.name[2] = 'c';
    nueva.name[3] = 'i';
    nueva.name[4] = 'a';
    return nueva;
}

EBR new_ebr(char status, char fit, int start, int size)
{
    EBR nuevo;
    nuevo.status = status;
    nuevo.fit = fit;
    nuevo.start = start;
    nuevo.size = size;
    nuevo.next = -1;

    for(int i = 0; i < 16; i++)
    {
        nuevo.name[i] = '\0';
    }

    //Poner nombre 'vacia' a la particion
    nuevo.name[0] = 'v';
    nuevo.name[1] = 'a';
    nuevo.name[2] = 'c';
    nuevo.name[3] = 'i';
    nuevo.name[4] = 'a';
    return nuevo;
}

void impr_mbr(MBR* tmp)
{
    int tam_disco = tmp->mbr_tam/1024;
    fprintf(stderr, "Tam      : %d KB\n", tam_disco);
    fprintf(stderr, "Fecha    : %s\n", formato_fecha(tmp->mbr_fecha));
    fprintf(stderr, "Signature: %d\n", tmp->mbr_disk_sig);

    for(int i = 0; i < 4; i++)
    {
        fprintf(stderr, "Particion %d     : %s\n", tmp->mbr_particion[i].name);
    }

    for(int i = 0; i < 4; i++)
    {
        if(tmp->mbr_particion[i].status == '1')
        {
            fprintf(stderr, "%s\n", "-----------");
            impr_part(&tmp->mbr_particion[i]);
        }
    }
}

void impr_ebr(int pos_inicio, char disco[])
{
    EBR tmp;
    FILE* archivo;
    archivo = fopen(disco, "rb+");
    if(archivo == NULL)
    {

    }
    else
    {
        while (1)
        {
            fseek(archivo, pos_inicio, SEEK_SET)    ;
            fread(&tmp, sizeof (EBR), 1, archivo);
            if(tmp.status == '1')
            {
                fprintf(stderr, "Status : %c\n", tmp.start);
                fprintf(stderr, "Fit    : %c\n", tmp.fit);
                fprintf(stderr, "Start  : %d\n", tmp.start);
                fprintf(stderr, "Size   : %d\n", tmp.size);
                fprintf(stderr, "Name   : %s\n", tmp.name);
                fprintf(stderr, "Next   : %d\n", tmp.next);
                fprintf(stderr, "--------------------------------\n");
                if(tmp.next == -1)
                {
                    break;
                }
                else
                {
                    pos_inicio = tmp.next;
                }
            }
            else
            {
                break;
            }
        }
        fclose(archivo);
    }
}

void impr_part(PARTICION* tmp)
{
    if(tmp->status == '1')
    {
        fprintf(stderr, "Status: %c\n", tmp->start);
        fprintf(stderr, "Type  : %c\n", tmp->type);
        fprintf(stderr, "Fit   : %c\n", tmp->fit);
        fprintf(stderr, "Start : %d\n", tmp->start);
        fprintf(stderr, "Size  : %d %s\n", tmp->size, "Bytes");
        fprintf(stderr, "Name  : %s\n", tmp->name);
        fprintf(stderr, "---------------------------------\n");
    }
}



/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

SUPER_BLOQUE new_super_bloque()
{
    SUPER_BLOQUE tmp;
    tmp.s_filesystem_type = 0;
    tmp.s_inodes_count = 0;
    tmp.s_blocks_count = 0;
    tmp.s_free_blocks_count = 0;
    tmp.s_free_inodes_count = 0;
    tmp.s_mtime = NULL;
    tmp.s_umtime = NULL;
    tmp.s_mnt_count = 0;
    tmp.s_magic = 0;
    tmp.s_inode_size = 0;
    tmp.s_block_size = 0;
    tmp.s_first_ino = 0;
    tmp.s_first_blo = 0;
    tmp.s_bm_inode_start = 0;
    tmp.s_bm_block_start = 0;
    tmp.s_inode_start = 0;
    tmp.s_block_start = 0;
    return tmp;
}

JOURNALING new_journaling() {
    JOURNALING tmp;
    tmp.j_tipo_operacion = 0;
    tmp.j_tipo = 0;
    tmp.j_permisos = 0;
    tmp.j_fecha = NULL;

    for (int i = 0; i < 100; i++)
    {
        if (i < 12)
        {
            tmp.j_propietario[i] = '\0';
        }
        tmp.j_contenido[i] = '\0';
        tmp.j_nombre[i] = '\0';
    }
    return tmp;
}

INODO new_inodo()
{
    INODO tmp;
    tmp.i_uid = 0;
    tmp.i_gid = 0;
    tmp.i_size = 0;
    tmp.i_atime = NULL;
    tmp.i_ctime = NULL;
    tmp.i_mtime = NULL;

    for (int i = 0; i < 15; i++)
    {
        tmp.i_block[i] = -1;
    }

    tmp.i_type = '\0';
    tmp.i_perm = 0;
    return tmp;
}

CONTENT new_content()
{
    CONTENT tmp;

    for (int i = 0; i < 12; i++)
    {
        tmp.b_name[i] = '\0';
    }

    tmp.b_inodo = -1;
    return tmp;
}

BLOQUE_CARPETA new_bloque_carpeta()
{
    BLOQUE_CARPETA tmp;

    for (int i = 0; i < 4; i++)
    {
        tmp.b_content[i] = new_content();
    }
    return tmp;
}

BLOQUE_ARCHIVO new_bloque_archivo()
{
    BLOQUE_ARCHIVO tmp;

    for (int i = 0; i < 64; i++)
    {
        tmp.b_content[i] = '\0';
    }
    return tmp;
}

BLOQUE_APUNTADOR new_bloque_apuntador()
{
    BLOQUE_APUNTADOR tmp;

    for (int i = 0; i < 16; i++)
    {
        tmp.b_pointers[i] = -1;
    }

    return tmp;
}

USUARIO_SISTEMA new_usuario_sistema()
{
    USUARIO_SISTEMA tmp;
    tmp.ban_usuario = 0;
    tmp.numero_usuario = 0;
    tmp.numero_grupo = 0;

    for (int i = 0; i < 12; i++)
    {
        tmp.grupo[i] = '\0';
        tmp.nombre[i] = '\0';
        tmp.password[i] = '\0';
        tmp.id_particion[i] = '\0';
    }
    return tmp;
}

MI_ARCHIVO new_archivo()
{
    MI_ARCHIVO tmp;
    tmp.existe_archivo = 0;

    for (int i = 0; i < 5000; i++)
    {
        tmp.contenido[i] = '\0';
    }

    return tmp;
}
