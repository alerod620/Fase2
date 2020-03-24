#ifndef COMANDO_H
#define COMANDO_H

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

void path_raid(char tmp[]);

/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/

//*********************************************
//****************** Comandos *****************
//*********************************************
void mkfs(char parametro[]);
void login(char parametro[]);
void logout(char parametro[]);
void mkgrp(char parametro[]);
void rmgrp(char parametro[]);
void mkusr(char parametro[]);
void rmusr(char parametro[]);
void chmod(char parametro[]);
void mkfile(char parametro[]);
void cat(char parametro[]);
void rem(char parametro[]);
void edit(char parametro[]);
void ren(char parametro[]);
void mkdir(char parametro[]);
void cp(char parametro[]);
void mv(char parametro[]);
void find(char parametro[]);
void chown(char parametro[]);
void chgrp(char parametro[]);
void recovery(char parametro[]);
void loss(char parametro[]);

#endif // COMANDO_H
