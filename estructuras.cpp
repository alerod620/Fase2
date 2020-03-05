#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <estructuras.h>

MKDISK nuevo_mkdisk()
{
    MKDISK nuevo;
    nuevo.ban_fit = 0;
    nuevo.ban_size = 0;
    nuevo.ban_path = 0;
    nuevo.ban_unit = 0;
    nuevo.ban_error = 0;
    nuevo.size = 0;
    for(int i = 0; i < 255; i++)
    {
        nuevo.path[i] = '\0';
    }
    return nuevo;
}

RMDISK nuevo_rmdisk()
{
    RMDISK nuevo;
    nuevo.ban_path = 0;
    nuevo.ban_error = 0;
    for(int i = 0; i < 255; i++)
    {
        nuevo.path[i] = '\0';
    }
    return nuevo;
}

FDISK nuevo_fdisk()
{
    FDISK nuevo;
    nuevo.ban_size = 0;
    nuevo.ban_path = 0;
    nuevo.ban_unit = 0;
    nuevo.ban_type = 0;
    nuevo.ban_fit = 0;
    nuevo.ban_delete = 0;
    nuevo.ban_name = 0;
    nuevo.ban_add = 0;
    nuevo.ban_s_d_a = 0;
    nuevo.ban_error = 0;
    nuevo.size = 0;
    nuevo.add = 0;
    for(int i = 0; i < 255; i++)
    {
        nuevo.name[i] = '\0';
        nuevo.path[i] = '\0';
    }
    return nuevo;
}

MOUNT nuevo_mount()
{
    MOUNT nuevo;
    nuevo.ban_name = 0;
    nuevo.ban_path = 0;
    nuevo.ban_error = 0;
    for(int i = 0; i < 255; i++)
    {
        nuevo.name[i] = '\0';
        nuevo.path[i] = '\0';
    }
    return nuevo;
}

UNMOUNT nuevo_unmount()
{
    UNMOUNT nuevo;
    nuevo.ban_id = 0;
    nuevo.ban_error = 0;
    for(int i = 0; i < 10; i++)
    {
        nuevo.id[i] = '\0';
    }
    return nuevo;
}

REP nuevo_rep()
{
    REP nuevo;
    nuevo.ban_id = 0;
    nuevo.ban_name = 0;
    nuevo.ban_path = 0;
    nuevo.ban_error = 0;
    for(int i = 0; i < 255; i++)
    {
        nuevo.id[i] = '\0';
        nuevo.name[i] = '\0';
        nuevo.path[i] = '\0';
    }
    return nuevo;
}

EXEC nuevo_exec() {
    EXEC nuevo;
    nuevo.ban_path = 0;
    nuevo.ban_error = 0;
    for (int i = 0; i < 255; i++)
        nuevo.path[i] = '\0';
    return nuevo;
}

void impr_mkdisk(MKDISK* tmp)
{
    fprintf(stderr, "B_fit: %d\n", tmp->ban_fit);
    fprintf(stderr, "B_size: %d\n", tmp->ban_size);
    fprintf(stderr, "B_unit: %d\n", tmp->ban_unit);
    fprintf(stderr, "B_path: %d\n", tmp->ban_path);
    fprintf(stderr, "Size  : %d\n", tmp->size);
    fprintf(stderr, "Path  : %s\n", tmp->path);
}

void impr_rmdisk(RMDISK* tmp)
{
    fprintf(stderr, "B_path: %d\n", tmp->ban_path);
    fprintf(stderr, "Path  : %s\n", tmp->path);
}

void impr_fdisk(FDISK* tmp)
{
    fprintf(stderr, "B_size  : %d\n", tmp->ban_size);
    fprintf(stderr, "B_unit  : %d\n", tmp->ban_unit);
    fprintf(stderr, "B_type  : %d\n", tmp->ban_type);
    fprintf(stderr, "B_fit   : %d\n", tmp->ban_fit);
    fprintf(stderr, "B_add   : %d\n", tmp->ban_add);
    fprintf(stderr, "B_delete: %d\n", tmp->ban_delete);
    fprintf(stderr, "B_ex    : %d\n", tmp->ban_s_d_a);
    fprintf(stderr, "B_path  : %d\n", tmp->ban_path);
    fprintf(stderr, "Size    : %d\n", tmp->ban_size);
    fprintf(stderr, "Add     : %d\n", tmp->add);
    fprintf(stderr, "Name    : %s\n", tmp->ban_name);
    fprintf(stderr, "Path    : %s\n", tmp->path);
}

void impr_mount(MOUNT* tmp)
{
    fprintf(stderr, "B_path: %d\n", tmp->ban_path);
    fprintf(stderr, "B_name: %d\n", tmp->ban_name);
    fprintf(stderr, "Path  : %s\n", tmp->path);
    fprintf(stderr, "Name  : %s\n", tmp->name);
}

void impr_unmount(UNMOUNT* tmp)
{
    fprintf(stderr, "B_id: %d\n", tmp->ban_id);
    fprintf(stderr, "Id  : %s\n", tmp->id);
}

void impr_rep(REP* tmp) {
    fprintf(stderr, "B_id  : %d\n", tmp->ban_id);
    fprintf(stderr, "B_name: %d\n", tmp->ban_name);
    fprintf(stderr, "B_path: %d\n", tmp->ban_path);
    fprintf(stderr, "Id    : %s\n", tmp->id);
    fprintf(stderr, "Name  : %s\n", tmp->name);
    fprintf(stderr, "Path  : %s\n", tmp->path);
}

void impr_exec(EXEC* tmp) {
    fprintf(stderr, "B_path: %d\n", tmp->ban_path);
    fprintf(stderr, "Path  : %s\n", tmp->path);
}

MKFS nuevo_mkfs()
{
    MKFS tmp;
    tmp.ban_id = 0;
    tmp.ban_fs = 0;
    tmp.ban_type = 0;
    tmp.ban_error = 0;
    for(int i = 0; i < 10; i++)
    {
        tmp.id[i] = '\0';
    }
    return tmp;
}

LOGIN nuevo_login()
{
    LOGIN tmp;
    tmp.ban_usr = 0;
    tmp.ban_pwd = 0;
    tmp.ban_id = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.usr[i] = '\0';
        tmp.pwd[i] = '\0';
        tmp.id[i] = '\0';
    }
    return tmp;
}

LOGOUT nuevo_logout()
{
    LOGOUT tmp;
    tmp.ban_logout = 0;
    tmp.ban_error =0;
    return tmp;
}

MKGRP nuevo_mkgrp()
{
    MKGRP tmp;
    tmp.ban_name = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.name[i] = '\0';
    }
    return tmp;
}

RMGRP nuevo_rmgrp()
{
    RMGRP tmp;
    tmp.ban_name = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.name[i] = '\0';
    }
    return tmp;
}

MKUSR nuevo_mkusr()
{
    MKUSR tmp;
    tmp.ban_usr = 0;
    tmp.ban_pwd = 0;
    tmp.ban_grp = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.usr[i] = '\0';
        tmp.pwd[i] = '\0';
        tmp.grp[i] = '\0';
    }
    return tmp;
}

RMUSR nuevo_rmusr()
{
    RMUSR tmp;
    tmp.ban_usr = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.usr[i] = '\0';
    }
    return tmp;
}

CHMOD nuevo_chmod()
{
    CHMOD tmp;
    tmp.ban_path = 0;
    tmp.ban_ugo = 0;
    tmp.ban_r = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        if (i < 3)
        {
            tmp.ugo[i] = '\0';
        }
    }
    return tmp;
}

MKFILE nuevo_mkfile()
{
    MKFILE tmp;
    tmp.ban_path = 0;
    tmp.ban_p = 0;
    tmp.ban_size = 0;
    tmp.ban_cont = 0;
    tmp.size = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        tmp.cont[i] = '\0';
    }
    return tmp;
}

CAT nuevo_cat()
{
    CAT tmp;
    tmp.ban_file = 0;
    tmp.ban_error = 0;
    for(int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
    }
    return tmp;
}

REM nuevo_rem() {
    REM tmp;
    tmp.ban_path = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
    }
    return tmp;
}

EDIT nuevo_edit() {
    EDIT tmp;
    tmp.ban_path = 0;
    tmp.ban_cont = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.path[i] = '\0';
        tmp.cont[i] = '\0';
    }
    return tmp;
}

REN nuevo_ren() {
    REN tmp;
    tmp.ban_path = 0;
    tmp.ban_name = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        if (i < 100)
        {
            tmp.name[i] = '\0';
        }
    }
    return tmp;
}

MKDIR nuevo_mkdir() {
    MKDIR tmp;
    tmp.ban_path = 0;
    tmp.ban_p = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
    }
    return tmp;
}

CP nuevo_cp() {
    CP tmp;
    tmp.ban_path = 0;
    tmp.ban_dest = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        tmp.dest[i] = '\0';
    }
    return tmp;
}

MV nuevo_mv() {
    MV tmp;
    tmp.ban_path = 0;
    tmp.ban_dest = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        tmp.dest[i] = '\0';
    }
    return tmp;
}

FIND nuevo_find()
{
    FIND tmp;
    tmp.ban_path = 0;
    tmp.ban_name = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        if (i < 100)
        {
            tmp.name[i] = '\0';
        }
    }
    return tmp;
}

CHOWN nuevo_chown() {
    CHOWN tmp;
    tmp.ban_path = 0;
    tmp.ban_r = 0;
    tmp.ban_usr = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 255; i++)
    {
        tmp.path[i] = '\0';
        if (i < 12)
        {
            tmp.usr[i] = '\0';
        }
    }
    return tmp;
}

CHGRP nuevo_chgrp() {
    CHGRP tmp;
    tmp.ban_usr = 0;
    tmp.ban_grp = 0;
    tmp.ban_error = 0;
    for (int i = 0; i < 12; i++)
    {
        tmp.usr[i] = '\0';
        tmp.grp[i] = '\0';
    }
    return tmp;
}

RECOVERY nuevo_recovery() {
    RECOVERY tmp;
    tmp.ban_id = 0;
    tmp.ban_error = 0;
    for (int i; i < 10; i++)
    {
        tmp.id[i] = '\0';
    }
    return tmp;
}

LOSS nuevo_loss() {
    LOSS tmp;
    tmp.ban_id = 0;
    tmp.ban_error = 0;
    for (int i; i < 10; i++)
    {
        tmp.id[i] = '\0';
    }
    return tmp;
}
