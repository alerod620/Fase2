#include <stdio.h>
#include <stdlib.h>

#include <comando.h>
#include <ejecutar_comando.h>
#include <validar_comando.h>
#include <analizar_parametro.h>

char raid_path[255];

void mkdisk(char parametro[])
{
    MKDISK tmp = parametros_mkdisk(parametro);
    if(tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1)
        {
            switch(tmp.ban_size)
            {
            case 0:
                fprintf(stderr, "****** DEBE INGRESAR TAMAÑO PARA EL DISCO ******\n");
                return;
                break;

            case 1:
                if(tmp.ban_unit == 3 || tmp.ban_unit == 4)
                {
                    fprintf(stderr, "****** UNIDAD INCORRECTA ******\n");
                }
                else
                {
                    ejecutar_mkdisk(&tmp);

                    //Ejecutar RAID
                    path_raid(tmp.path);
                    strcpy(tmp.path, raid_path);
                    ejecutar_mkdisk(&tmp);
                }
                break;

            case 2:
                fprintf(stderr, "****** SIZE NO VALIDO ******\n");
                return;
                break;
            }
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA PARA EL DISCO ******\n");
        }
    }
}

void rmdisk(char parametro[])
{
    RMDISK tmp = parametros_rmdisk(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_path == 1) {
            ejecutar_rmdisk(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA DEL DISCO ******\n");
        }
    }
}

void fdisk(char parametro[]) {
    FDISK tmp = parametros_fdisk(parametro);
    FILE* archivo; //Variable para saber si debe ejecuar eliminar y redim en el raid

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1) {
            switch (tmp.ban_s_d_a) {
                case 1:
                    crear_fdisk(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        crear_fdisk(&tmp);
                    }

                    break;
                case 2:
                    ejecutar_eliminar(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        ejecutar_eliminar(&tmp);
                    }
                    break;
                case 3:
                    redim_fdisk(&tmp);

                    archivo = fopen(tmp.path, "rb+");
                    if(archivo != NULL)
                    {
                        //Ejecutar RAID
                        path_raid(tmp.path);
                        strcpy(tmp.path, raid_path);
                        redim_fdisk(&tmp);
                    }


                    break;
            }
        } else if (tmp.ban_path != 1 && tmp.ban_name == 1) {
            fprintf(stderr, "****** DEBE INGRESAR PATH DE DISCO ******\n");
        } else if (tmp.ban_path == 1 && tmp.ban_name != 1) {
            fprintf(stderr, "****** FALTA NOMBRE DE PARTICION ******\n");
        } else {
            fprintf(stderr, "****** FALTA NOMBRE Y RUTA ******\n");
        }
    }
}

void mount(char parametro[]) {
    MOUNT tmp = parametros_mount(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1) {
            ejecutar_mount(&tmp);
        } else if (tmp.ban_path != 1 && tmp.ban_name == 1) {
            fprintf(stderr, "****** DEBE INGRESAR PATH DE DISCO ******\n");
        } else if (tmp.ban_path == 1 && tmp.ban_name != 1) {
            fprintf(stderr, "****** DEBE INGRESAR NOMBRE DE DISCO ******\n");
        } else {
            fprintf(stderr, "****** DEBE INGRESAR NOMBRE Y RUTA DEL DISCO ******\n");
        }
    }
}

void unmount(char parametro[])
{

    UNMOUNT tmp = parametros_unmount(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_id == 1) {
            ejecutar_unmount(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE DE INGRESAR ID DE PARTICION ******\n");
        }
    }
}

void rep(char parametro[])
{
    REP tmp = parametros_rep(parametro);

    if (tmp.ban_error == 0) {
        if (tmp.ban_path == 1 && tmp.ban_name == 1 && tmp.ban_id == 1) {
            mayusculas(tmp.name);
            if (strcmp(tmp.name, "DISK") == 0) {
                rep_disk(&tmp);
            }
            else if (strcmp(tmp.name, "MBR") == 0)
            {
                rep_mbr(&tmp);
            }
            else if (strcmp(tmp.name, "INODE") == 0)
            {
                ejecutar_rep_inodes(&tmp);
            }
            else if (strcmp(tmp.name, "JOURNALING") == 0)
            {
                //print_rep(&tmp);
            }
            else if (strcmp(tmp.name, "BLOCK") == 0)
            {
                ejecutar_rep_blocks(&tmp);
            }
            else if (strcmp(tmp.name, "BM_INODE") == 0)
            {
                ejecutar_rep_bm_inodos(&tmp);
            }
            else if (strcmp(tmp.name, "BM_BLOCK") == 0)
            {
                ejecutar_rep_bm_bloques(&tmp);
            }
            else if (strcmp(tmp.name, "TREE") == 0)
            {
                ejecutar_rep_tree(&tmp);
            }
            else if (strcmp(tmp.name, "SB") == 0)
            {
                ejecutar_rep_sb(&tmp);
            }
            else if (strcmp(tmp.name, "FILE") == 0)
            {
                if(tmp.ruta == 0){
                    fprintf(stderr, "-----------------------ERROR DEBE DE INGRESAR LA RUTA DE ARCHIVO-----------------------------------------------\n");
                }
                else
                {
                    ejecutar_rep_file(&tmp);
                }
            } else if (strcmp(tmp.name, "LS") == 0)
            {
                //print_rep(&tmp);
            }
        } else {
            fprintf(stderr, "****** DEBE DE INGRESAR NOMBRE, PATH Y ID ******\n");
        }
    }
}

void exec(char parametro[])
{
    EXEC tmp = parametros_exec(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_path == 1)
        {
            ejecutar_exec(&tmp);
        }
        else
        {
            fprintf(stderr, "****** DEBE INGRESAR RUTA ******\n");
        }
    }
}

void path_raid(char tmp[])
{
    int pos_tmp_path = 0;
    for(int i = 0; i < 255; i++)
    {
        raid_path[i] = '\0';
    }

    for(int i = 0; i < 255; i++)
    {
        if(tmp[i] > 0)
        {
            if(tmp[pos_tmp_path] != '.')
            {
                raid_path[pos_tmp_path] = tmp[i];
                pos_tmp_path++;
            }
            else
            {

            }
        }
    }

    strcat(raid_path, "_ra1.disk");
}


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

void mkfs(char parametro[])
{
    MKFS tmp = parametros_mkfs(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_id == 0)
        {
            mensaje("ERROR DEBE DE INGRESAR UN ID");
        }
        else
        {
            ejecutar_mkfs(&tmp);
            //print_mkfs(&tmp);
        }
    }

}

void login(char parametro[])
{
    LOGIN tmp = parametros_login(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_id == 1 && tmp.ban_pwd == 1 && tmp.ban_usr == 1)
        {
            ejecutar_login(&tmp);
        }
        else
        {
            mensaje("ERROR FALTA PARAMETROS");
        }
    }

}

void logout(char parametro[]) {
    LOGOUT tmp = parametros_logout(parametro);

    if (tmp.ban_error == 0)
    {
        ejecutar_logout(&tmp);
    }

}

void mkgrp(char parametro[]) {
    MKGRP tmp = parametros_mkgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_name == 0)
        {
            mensaje("ERROR FALTA PARAMETRO NAME");
        }
        else
        {
            //print_mkgrp(&tmp);
        }
    }

}

void rmgrp(char parametro[])
{
    RMGRP tmp = parametros_rmgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_name == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO NAME");
        }
        else
        {
            //print_rmgrp(&tmp);
        }
    }

}

void mkusr(char parametro[]) {
    MKUSR tmp = parametros_mkusr(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_usr == 1 && tmp.ban_pwd == 1 && tmp.ban_grp == 1)
        {
            //print_mkusr(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void rmusr(char parametro[])
{
    RMUSR tmp = parametros_rmusr(parametro);

    if (tmp.ban_error == 0)
    {
        if (tmp.ban_usr == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO USR");
        }
        else
        {
            //print_rmusr(&tmp);
        }
    }

}

void chmod(char parametro[])
{
    CHMOD tmp = parametros_chmod(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_path == 1 && tmp.ban_ugo == 1)
        {
            //print_chmod(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mkfile(char parametro[])
{
    MKFILE tmp = parametros_mkfile(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_path == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO PATH");
        }
        else
        {
            ejecutar_mkfile(&tmp);
            //print_mkfile(&tmp);
        }
    }

}

void cat(char parametro[])
{
    CAT tmp = parametros_cat(parametro);

    if (tmp.ban_error == 0)
    {

        if (tmp.ban_file == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO FILE");
        }
        else
        {
            ejecutar_cat(&tmp);
        }
    }
}

void rem(char parametro[])
{
    REM tmp = parametros_rem(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO PATH");
        }
        else
        {
            //print_rem(&tmp);
        }
    }

}

void edit(char parametro[])
{
    EDIT tmp = parametros_edit(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0 || tmp.ban_cont == 0)
        {
            mensaje("ERROR FALTA EL PARAMETRO PATH");
        }
        else
        {

            //print_edit
        }
    }

}

void ren(char parametro[])
{
    REN tmp = parametros_ren(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_name == 1)
        {
            //print_ren(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mkdir(char parametro[])
{
    MKDIR tmp = parametros_mkdir(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 0)
        {
            mensaje("ERROR HACE FALTA EL PARAMETRO PATH");
        }
        else
        {
            ejecutar_mkdir(&tmp);
        }
    }

}

void cp(char parametro[])
{
    CP tmp = parametros_cp(parametro);

    if (tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1 && tmp.ban_dest == 1)
        {
            //print_cp(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void mv(char parametro[])
{
    MV tmp = parametros_mv(parametro);

    if (tmp.ban_error == 0)
    {
        if(tmp.ban_path == 1 && tmp.ban_dest == 1)
        {
            //print_mv(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void find(char parametro[])
{
    FIND tmp = parametros_find(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_name == 1)
        {
            //print_find(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void chown(char parametro[])
{
    CHOWN tmp = parametros_chown(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_path == 1 && tmp.ban_usr == 1)
        {
            //print_chown(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void chgrp(char parametro[])
{
    CHGRP tmp = parametros_chgrp(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_usr ==  1 && tmp.ban_grp == 1)
        {
            //print_chgrp(&tmp);
        }
        else
        {
            mensaje("ERROR HACE FALTA PARAMETROS");
        }
    }

}

void recovery(char parametro[])
{
    RECOVERY tmp = parametros_recovery(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_id == 0)
        {
            mensaje("ERROR HACE FALTA ID DE PARTICION");
        }
        else
        {
            //print_recovery(&tmp);
        }
    }

}

void loss(char parametro[])
{
    LOSS tmp = parametros_loss(parametro);

    if (tmp.ban_error == 0)
    {

        if(tmp.ban_id == 0)
        {
            mensaje("ERROR HACE FALTA ID DE PARTICION");
        }
        else
        {
            //print_loss(&tmp);
        }
    }

}



