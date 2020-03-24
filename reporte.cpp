#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <reporte.h>
#include <ejecutar_comando.h>
#include <validar_comando.h>

FILE* REPORTE_TREE;

char dot[2014];
char formato[10];

void reporte_disco(char* path_disco, char* path_destino_imagen)
{
    FILE *archivo_mbr;
    FILE *reporte;
    MBR tmp_mbr;
    PARTICION  tmp_particion;
    EBR tmp_ebr;
    int pos_inicio;
    double tam_part;
    archivo_mbr = fopen(path_disco, "rb+");

    if (archivo_mbr == NULL)
    {
        fprintf(stderr, "****** DISCO EN RUTA NO ENCONTRADO ******\n");
    }
    else
    {
        fseek(archivo_mbr, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo_mbr);

        reporte = fopen("imagen_disk.dot", "w");

        if (reporte == NULL)
        {
            fprintf(stderr, "****** ERROR ARCHIVO DE REPORTE ******\n");
        }
        else
        {
            fprintf(reporte, "digraph DISCO{\n\tnode [ shape=record];\n\t\tnodo_mbr[shape=record, label=\" ");
            fprintf(reporte, "M\\nB\\nR");


            for (int i = 0; i < 4; i++)
            {
                tmp_particion = tmp_mbr.mbr_particion[i];

                if (tmp_particion.type == 'P')
                {
                    tam_part = porcentaje_p((double) tmp_particion.size, (double) tmp_mbr.mbr_tam);
                    fprintf(reporte, " | Primaria\\n%d %% del disco", (int) tam_part);
                }
                else if (tmp_particion.type == 'E')
                {
                    fprintf(reporte, " | {Extendida | {");

                    pos_inicio = tmp_particion.start;
                    fseek(archivo_mbr, pos_inicio, SEEK_SET);
                    fread(&tmp_ebr, sizeof (EBR), 1, archivo_mbr);

                    if (tmp_ebr.status == '0')
                    {
                        fprintf(reporte, "E\\nB\\nR\\n");
                    }
                    else
                    {
                        tam_part = porcentaje_p( (double) tmp_ebr.size, (double) tmp_mbr.mbr_tam);
                        fprintf(reporte, "E\\nB\\nR\\n | Logica\\n%d %% del disco", (int) tam_part);
                    }
                    pos_inicio = tmp_ebr.next;

                    while (pos_inicio != -1)
                    {
                        fseek(archivo_mbr, pos_inicio, SEEK_SET);
                        fread(&tmp_ebr, sizeof (EBR), 1, archivo_mbr);
                        tam_part = porcentaje_p((double) tmp_ebr.size, (double) tmp_mbr.mbr_tam);
                        fprintf(reporte, "| E\\nB\\nR\\n | Logica\\n%d %% del disco", (int) tam_part);
                        pos_inicio = tmp_ebr.next;
                    }
                    fprintf(reporte, "}}");

                }
                else
                {
                    tam_part = porcentaje_p((double) tmp_particion.size, (double) tmp_mbr.mbr_tam);
                    fprintf(reporte, "| Libre\\n%d %% del disco", (int) tam_part);
                }
            }



            fprintf(reporte, "\"];\n}");

            fclose(reporte);
            fclose(archivo_mbr);
            limpiar();

            obtener_formato(path_destino_imagen);

            strcpy(dot, "dot -T");
            strcat(dot, formato);
            strcat(dot,  " imagen_disk.dot -o ");
            strcat(dot, path_destino_imagen);
            system(dot);

        }
    }

}

void reporte_mbr(char* path_disco, char* path_destino_imagen)
{
    FILE* archivo_mbr;
    FILE* reporte;
    MBR tmp_mbr;
    EBR tmp_ebr;
    int pos_inicio = 0;
    int indice_logica = 1;
    archivo_mbr = fopen(path_disco, "rb+");

    if (archivo_mbr == NULL)
    {
        fprintf(stderr, "****** DISCO EN RUTA NO ENCONTRADO ******\n");
    } else {
        fseek(archivo_mbr, 0, SEEK_SET);
        fread(&tmp_mbr, sizeof (MBR), 1, archivo_mbr);

        reporte = fopen("imagen_mbr.dot", "w");

        if (reporte == NULL)
        {
            fprintf(stderr, "****** ERROR ARCHIVO DE REPORTE ******\n");
        }
        else
        {
            fprintf(reporte, "DIGRAPH MBR{\ndpi=500;\ngraph [fontsize=30 labelloc= \"t\" label= \"\"splines=true overlap=false rankdir=\"LR\"];\n");
            fprintf(reporte, "ratio=auto\n\t""nodo0""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n");
            fprintf(reporte, "label=<<table border = \"2\">\n");
            fprintf(reporte, "<tr>\n<td>Nombre</td>\n<td>%s</td>\n</tr>\n", path_disco);
            fprintf(reporte, "<tr>\n<td>tam_mbr</td>\n<td>%d</td>\n</tr>\n", tmp_mbr.mbr_tam);
            fprintf(reporte, "<tr>\n<td>fecha_creacion</td>\n<td>%s</td>\n</tr>\n", formato_fecha(tmp_mbr.mbr_fecha));
            fprintf(reporte, "<tr>\n<td>mbr_disk_signature</td>\n<td>%d</td>\n</tr>\n", tmp_mbr.mbr_disk_sig);
            fprintf(reporte, "<tr>\n<td>disk_fit</td>\n<td>%c</td>\n</tr>\n", tmp_mbr.mbr_fit);


            for (int i = 0; i < 4; i++) //Recorrera las particiones del disco
            {
                if (tmp_mbr.mbr_particion[i].status == '1') {
                    fprintf(reporte, "<tr><td>part_status_%d</td><td>%c</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].status);
                    fprintf(reporte, "<tr><td>part_type_%d</td><td>%c</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].type);
                    fprintf(reporte, "<tr><td>part_fit_%d</td><td>%c</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].fit);
                    fprintf(reporte, "<tr><td>part_start_%d</td><td>%d</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].start);
                    fprintf(reporte, "<tr><td>part_size_%d</td><td>%d</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].size);
                    fprintf(reporte, "<tr><td>part_name_%d</td><td>%s</td></tr>\n", (i + 1), tmp_mbr.mbr_particion[i].name);
                }
            }
            fprintf(reporte, "</table>>];\n");

            for (int i = 0; i < 4; i++)
            {

                if (tmp_mbr.mbr_particion[i].type == 'E')
                {
                    pos_inicio = tmp_mbr.mbr_particion[i].start;
                    while (pos_inicio != -1) {
                        fseek(archivo_mbr, pos_inicio, SEEK_SET);
                        fread(&tmp_ebr, sizeof (EBR), 1, archivo_mbr);

                        if (tmp_ebr.status != '0') {
                            fprintf(reporte, "\t""nodo%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", indice_logica);
                            fprintf(reporte, "label=<<table border = \"2\">\n");
                            fprintf(reporte, "<tr><td>Nombre</td><td>Valor</td></tr>\n");
                            fprintf(reporte, "<tr><td>Part_status_%d</td><td>%c</td></tr>\n", indice_logica, (int) tmp_ebr.status);
                            fprintf(reporte, "<tr><td>Part_fit_%d</td><td>%c</td></tr>\n", indice_logica, tmp_ebr.fit);
                            fprintf(reporte, "<tr><td>Part_start_%d</td><td>%d</td></tr>\n", indice_logica, tmp_ebr.start);
                            fprintf(reporte, "<tr><td>Part_size_%d</td><td>%d</td></tr>\n", indice_logica, tmp_ebr.size);
                            fprintf(reporte, "<tr><td>Part_next_%d</td><td>%d</td></tr>\n", indice_logica, tmp_ebr.next);
                            fprintf(reporte, "<tr><td>Part_name_%d</td><td>%s</td></tr>\n", indice_logica, tmp_ebr.name);
                            fprintf(reporte, "</table>>];\n");
                        }

                        pos_inicio = tmp_ebr.next;
                        indice_logica++;
                    }
                    for (int i = 1; i < indice_logica; i++)
                        fprintf(reporte, "nodo0 -> nodo%d;\n", i);
                    break;
                }

            }

            fprintf(reporte, "}");



            fclose(reporte);
            fclose(archivo_mbr);
            limpiar();

            obtener_formato(path_destino_imagen);

            strcpy(dot, "dot -T");
            strcat(dot, formato);
            strcat(dot,  " imagen_mbr.dot -o ");
            strcat(dot, path_destino_imagen);
            system(dot);

        }
    }
}

void limpiar()
{
    for(int i = 0; i < 1024; i++)
    {
        dot[i] = '\0';
    }
}

double porcentaje_p(double tam_part, double tam_mbr)
{
    int resultado = (int) ((tam_part * 100) / tam_mbr);
    return resultado;
}

double porcentaje(double pos_inicio, double pos_fin, double tam_mbr)
{
    double tam_real = pos_fin - pos_inicio;
    int resultado = (int) ((tam_real * 100) / tam_mbr);
    return resultado;
}

void obtener_formato(char tmp[])
{
    int pos_tmp_path = 0;
    int punto = 0;
    for(int i = 0; i < 10; i++)
    {
        formato[i] = '\0';
    }

    for(int i = 0; i < 255; i++)
    {
        if(tmp[i] > 0)
        {
            if(tmp[i] == '.')
            {
                punto = 1;
                i++;
            }

            if(punto == 1)
            {
                formato[pos_tmp_path] = tmp[i];
                pos_tmp_path++;
            }
        }
    }

}


/*
******************************************************************************
*********************************** FASE 2 ***********************************
******************************************************************************
*/


void reporte_bm_inodos(char* path_disco, int bm_start, int bm_count, char* path_destino_reporte)
{
    FILE *ARCHIVO_DISCO;
    FILE *REPORTE_BM;
    ARCHIVO_DISCO = fopen(path_disco, "rb");
    if (ARCHIVO_DISCO == NULL) {
        mensaje("ERROR CON PATH");
    } else {

        REPORTE_BM = fopen(path_destino_reporte, "w");

        char CARACTER = '\0';
        int contador = 0;
        fseek(ARCHIVO_DISCO, bm_start, SEEK_SET);
        fseek(REPORTE_BM, 0, SEEK_SET);

        for (int j = 0; j < bm_count; j++)
        {
            fread(&CARACTER, sizeof (char), 1, ARCHIVO_DISCO);

            if (contador < 20)
            {
                fprintf(REPORTE_BM, "%c", CARACTER);
                fprintf(REPORTE_BM, " ");
            }
            else
            {
                fprintf(REPORTE_BM, "\n");
                fprintf(REPORTE_BM, "%c", CARACTER);
                fprintf(REPORTE_BM, " ");
                contador = 0;
            }
            contador++;
        }
        fprintf(REPORTE_BM, "\n");
        fclose(ARCHIVO_DISCO);
        fclose(REPORTE_BM);
    }
}

void reporte_bm_bloques(char* path_disco, int bm_start, int bm_count, char* path_destino_reporte)
{
    FILE *ARCHIVO_DISCO;
    FILE *REPORTE_BM;
    ARCHIVO_DISCO = fopen(path_disco, "rb");

    if (ARCHIVO_DISCO == NULL)
    {
        mensaje("ERROR CON PATH");
    }
    else
    {

        REPORTE_BM = fopen(path_destino_reporte, "w");

        char CARACTER = '\0';
        int contador = 0;
        fseek(ARCHIVO_DISCO, bm_start, SEEK_SET);
        fseek(REPORTE_BM, 0, SEEK_SET);

        for (int j = 0; j < bm_count; j++)
        {
            fread(&CARACTER, sizeof (char), 1, ARCHIVO_DISCO);

            if (contador < 20)
            {
                fprintf(REPORTE_BM, "%c", CARACTER);
                fprintf(REPORTE_BM, " ");
            }
            else
            {
                fprintf(REPORTE_BM, "\n");
                fprintf(REPORTE_BM, "%c", CARACTER);
                fprintf(REPORTE_BM, " ");
                contador = 0;
            }
            contador++;
        }
        fprintf(REPORTE_BM, "\n");
        fclose(ARCHIVO_DISCO);
        fclose(REPORTE_BM);
    }
}

void reporte_sb(SUPER_BLOQUE *sb, char *path_destino)
{
    FILE* ARCHIVO_REPORTE;

    ARCHIVO_REPORTE = fopen("pdf_super_bloque.dot", "w");

    fprintf(ARCHIVO_REPORTE, "DIGRAPH SB{\ndpi=500;\ngraph [fontsize=30 labelloc= \"t\" label= \"\"splines=true overlap=false rankdir=\"LR\"];\n");
    fprintf(ARCHIVO_REPORTE, "ratio=auto\n\t""nodo0""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n");
    fprintf(ARCHIVO_REPORTE, "label=<<table border = \"2\">\n");
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>Nombre</td><td>Valor</td></tr>\n");
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_inodes_count", sb->s_inodes_count);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_blocks_count", sb->s_blocks_count);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_free_blocks_count", sb->s_free_blocks_count);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_free_inodes_count", sb->s_free_inodes_count);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%s</td></tr>\n", "s_mtime", formato_fecha(sb->s_mtime));
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_umtime", formato_fecha(sb->s_umtime));
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_mnt_count", sb->s_mnt_count);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_magic", sb->s_magic);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_inode_size", sb->s_inode_size);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_block_size", sb->s_block_size);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_first_inode", sb->s_first_ino);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_first_block", sb->s_first_blo);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_bm_inode_start", sb->s_bm_inode_start);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_bm_block_start", sb->s_bm_block_start);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_inode_start", sb->s_inode_start);
    fprintf(ARCHIVO_REPORTE, "\t<tr><td>%s</td><td>%d</td></tr>\n", "s_block_start", sb->s_block_start);


    fprintf(ARCHIVO_REPORTE, "</table>>];\n}");

    fclose(ARCHIVO_REPORTE);
    limpiar();

    obtener_formato(path_destino);

    strcpy(dot, "dot -T");
    strcat(dot, formato);
    strcat(dot,  " pdf_super_bloque.dot -o ");
    strcat(dot, path_destino);
    system(dot);

    /*
    strcpy(&EJECUTAR_DOT, "dot -Tpdf pdf_super_bloque.dot -o ");
    strcat(&EJECUTAR_DOT, path_destino);
    system(EJECUTAR_DOT);
    */

}

void reporte_inodes(char* path_disco, char* path_destino, int start_particion) {
    FILE* ARCHIVO_DISCO;
    FILE* REPORTE;
    SUPER_BLOQUE tmp_sb;
    INODO tmp_ino;
    char ESTADO_INO;
    int numero_inodos_usados = 0;

    ARCHIVO_DISCO = fopen(path_disco, "rb");

    if (ARCHIVO_DISCO == NULL)
    {
        mensaje("PATH DE DISCO, REPORTE");
    }
    else
    {
        REPORTE = fopen("pdf_inodos.dot", "w");
        fseek(ARCHIVO_DISCO, start_particion, SEEK_SET);
        fread(&tmp_sb, sizeof (SUPER_BLOQUE), 1, ARCHIVO_DISCO);

        fprintf(REPORTE, "DIGRAPH INO{\ndpi=500;\ngraph [fontsize=30 labelloc= \"t\" label= \"\"splines=true overlap=false rankdir=\"LR\"];\n");

        for (int i = 0; i < tmp_sb.s_inodes_count; i++)
        {
            fseek(ARCHIVO_DISCO, (tmp_sb.s_bm_inode_start + i), SEEK_SET);
            fread(&ESTADO_INO, sizeof (char), 1, ARCHIVO_DISCO);

            if (ESTADO_INO == '1')
            {
                fseek(ARCHIVO_DISCO, tmp_sb.s_inode_start + (sizeof (INODO) * i), SEEK_SET);
                fread(&tmp_ino, sizeof (INODO), 1, ARCHIVO_DISCO);
                fprintf(REPORTE, "\t""nodo%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", i);
                fprintf(REPORTE, "label=<<table border = \"2\">\n");
                fprintf(REPORTE, "<tr><td colspan = \"2\">Inodo %d</td></tr>\n", numero_inodos_usados);
                fprintf(REPORTE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_uid", tmp_ino.i_uid);
                fprintf(REPORTE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_gid", tmp_ino.i_gid);
                fprintf(REPORTE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_size", tmp_ino.i_size);
                fprintf(REPORTE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_atime", formato_fecha(tmp_ino.i_atime));
                fprintf(REPORTE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_ctime", formato_fecha(tmp_ino.i_ctime));
                fprintf(REPORTE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_mtime", formato_fecha(tmp_ino.i_mtime));

                for (int j = 0; j < 15; j++)
                {
                    if (tmp_ino.i_block[j] != -1)
                    {
                        fprintf(REPORTE, "<tr><td>%s%d</td><td>%d</td></tr>\n", "i_block_", j, tmp_ino.i_block[j]);
                    }
                }
                fprintf(REPORTE, "<tr><td>%s</td><td>%c</td></tr>\n", "i_type", tmp_ino.i_type);
                fprintf(REPORTE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_perm", tmp_ino.i_perm);
                fprintf(REPORTE, "</table>>];\n");
                numero_inodos_usados++;
            }
        }

        for (int i = 0; i < numero_inodos_usados; i++)
        {
            if ((i + 1) != numero_inodos_usados)
            {
                fprintf(REPORTE, "nodo%d -> nodo%d\n", i, (i + 1));
            }
        }
        fprintf(REPORTE, "}");
        fclose(ARCHIVO_DISCO);
        fclose(REPORTE);
        limpiar();

        obtener_formato(path_destino);

        strcpy(dot, "dot -T");
        strcat(dot, formato);
        strcat(dot,  " pdf_inodos.dot -o ");
        strcat(dot, path_destino);
        system(dot);

        /*
        strcpy(&EJECUTAR_DOT, "dot -Tpdf pdf_inodos.dot -o ");
        strcat(&EJECUTAR_DOT, path_destino);
        system(EJECUTAR_DOT);
        */
    }
}

void reporte_blocks(char* path_disco, char* path_destino, int start_particion)
{
    FILE* ARCHIVO_DISCO;
    FILE* REPORTE;
    SUPER_BLOQUE tmp_sb;
    INODO tmp_ino;
    BLOQUE_APUNTADOR tmp_aputador;
    BLOQUE_CARPETA tmp_carpeta;
    BLOQUE_ARCHIVO tmp_archivo;
    char ESTADO_INO;
    int numero_bloques_usados = 0;

    ARCHIVO_DISCO = fopen(path_disco, "rb");

    if (ARCHIVO_DISCO == NULL)
    {
        mensaje("ERROR PATH DE DISCO, REPORTE");
    }
    else
    {
        REPORTE = fopen("pdf_bloques.dot", "w");
        fseek(ARCHIVO_DISCO, start_particion, SEEK_SET);
        fread(&tmp_sb, sizeof (SUPER_BLOQUE), 1, ARCHIVO_DISCO);

        fprintf(REPORTE, "DIGRAPH INO{\ndpi=500;\ngraph [fontsize=30 labelloc= \"t\" label= \"\"splines=true overlap=false rankdir=\"LR\"];\n");

        for (int i = 0; i < tmp_sb.s_inodes_count; i++)
        {
            fseek(ARCHIVO_DISCO, (tmp_sb.s_bm_inode_start + i), SEEK_SET);
            fread(&ESTADO_INO, sizeof (char), 1, ARCHIVO_DISCO);

            if (ESTADO_INO == '1')
            {
                fseek(ARCHIVO_DISCO, tmp_sb.s_inode_start + (sizeof (INODO) * i), SEEK_SET);
                fread(&tmp_ino, sizeof (INODO), 1, ARCHIVO_DISCO);

                for (int j = 0; j < 15; j++)
                {

                    if (tmp_ino.i_block[j] != -1)
                    {

                        if (j < 13)
                        {

                            if (tmp_ino.i_type == '0')
                            {
                                fseek(ARCHIVO_DISCO, tmp_sb.s_block_start + (sizeof (BLOQUE_ARCHIVO) * tmp_ino.i_block[j]), SEEK_SET);
                                fread(&tmp_carpeta, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);
                                fprintf(REPORTE, "\t""nodo%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", numero_bloques_usados);
                                fprintf(REPORTE, "label=<<table border = \"2\">\n");
                                fprintf(REPORTE, "<tr><td colspan = \"2\">Bloque Carpeta %d</td></tr>\n", tmp_ino.i_block[j]);
                                fprintf(REPORTE, "<tr><td>b_name</td><td>b_inodo</td></tr>\n");

                                for (int k = 0; k < 4; k++)
                                {

                                    if (tmp_carpeta.b_content[k].b_inodo != -1)
                                    {
                                        fprintf(REPORTE, "<tr><td>%s</td><td>%d</td></tr>\n", tmp_carpeta.b_content[k].b_name, tmp_carpeta.b_content[k].b_inodo);
                                    }
                                }

                                fprintf(REPORTE, "</table>>];\n");
                                numero_bloques_usados++;
                            }
                            else
                            {
                                //ARCHIVO
                                fseek(ARCHIVO_DISCO, tmp_sb.s_block_start + (sizeof (BLOQUE_ARCHIVO) * tmp_ino.i_block[j]), SEEK_SET);
                                fread(&tmp_archivo, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);
                                fprintf(REPORTE, "\t""nodo%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", numero_bloques_usados);
                                fprintf(REPORTE, "label=<<table border = \"2\">\n");
                                fprintf(REPORTE, "<tr><td colspan = \"2\">Bloque Archivo %d</td></tr>\n", tmp_ino.i_block[j]);

                                fprintf(REPORTE, "<tr><td colspan=\"2\">%s</td></tr>\n", tmp_archivo.b_content);
                                fprintf(REPORTE, "</table>>];\n");
                                numero_bloques_usados++;
                            }
                        }
                        else
                        {
                            //INDIRECTOS
                        }
                    }
                }
            }
        }

        for (int i = 0; i < numero_bloques_usados; i++)
        {

            if ((i + 1) != numero_bloques_usados)
            {
                fprintf(REPORTE, "nodo%d -> nodo%d\n", i, (i + 1));
            }
        }
        fprintf(REPORTE, "}");
        fclose(ARCHIVO_DISCO);
        fclose(REPORTE);
        limpiar();

        obtener_formato(path_destino);

        strcpy(dot, "dot -T");
        strcat(dot, formato);
        strcat(dot,  " pdf_bloque.dot -o ");
        strcat(dot, path_destino);
        system(dot);

        /*
        strcpy(&EJECUTAR_DOT, "dot -Tpdf pdf_bloques.dot -o ");
        strcat(&EJECUTAR_DOT, path_destino);
        system(EJECUTAR_DOT);
        */
    }
}

void reporte_tree(char* path_disco, char* path_destino, int start_particion)
{
    FILE* ARCHIVO_DISCO;
    SUPER_BLOQUE tmp_sb;
    ARCHIVO_DISCO = fopen(path_disco, "rb");

    if (ARCHIVO_DISCO == NULL)
    {
        mensaje("ERROR PATH REPORTE TREE");
    }
    else
    {
        fseek(ARCHIVO_DISCO, start_particion, SEEK_SET);
        fread(&tmp_sb, sizeof (SUPER_BLOQUE), 1, ARCHIVO_DISCO);
        fclose(ARCHIVO_DISCO);

        REPORTE_TREE = fopen("tree.dot", "w");
        fprintf(REPORTE_TREE, "DIGRAPH TREE{\ngraph [fontsize=30 rankdir=\"LR\"];\n");
        tree_recursivo(path_disco, tmp_sb.s_inode_start, tmp_sb.s_block_start, 0);
        fprintf(REPORTE_TREE, "}");
        fclose(REPORTE_TREE);
        limpiar();

        obtener_formato(path_destino);

        strcpy(dot, "dot -T");
        strcat(dot, formato);
        strcat(dot,  " pdf_super_bloque.dot -o ");
        strcat(dot, path_destino);
        system(dot);

        /*
        strcpy(&EJECUTAR_DOT, "dot -Tpng tree.dot -o ");
        strcat(&EJECUTAR_DOT, path_destino);
        system(EJECUTAR_DOT);
        */
    }

}

void tree_recursivo(char* path_disco, int start_inodos, int start_bloques, int numero_inodo)
{
    FILE* ARCHIVO_DISCO;
    INODO tmp_ino;
    BLOQUE_APUNTADOR tmp_aputador;
    BLOQUE_APUNTADOR tmp_aputador_lvl1;
    BLOQUE_CARPETA tmp_carpeta;
    BLOQUE_ARCHIVO tmp_archivo;
    int i = 0;

    ARCHIVO_DISCO = fopen(path_disco, "rb");
    fseek(ARCHIVO_DISCO, start_inodos + (sizeof (INODO) * numero_inodo), SEEK_SET);
    fread(&tmp_ino, sizeof (INODO), 1, ARCHIVO_DISCO);


    fprintf(REPORTE_TREE, "\t""inodo%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", numero_inodo);
    fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
    fprintf(REPORTE_TREE, "<tr><td bgcolor = \"deepskyblue\" colspan = \"2\">Inodo %d</td></tr>\n", numero_inodo);
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_uid", tmp_ino.i_uid);
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_gid", tmp_ino.i_gid);
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_size", tmp_ino.i_size);
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_atime", formato_fecha(tmp_ino.i_atime));
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_ctime", formato_fecha(tmp_ino.i_ctime));
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%s</td></tr>\n", "i_mtime", formato_fecha(tmp_ino.i_mtime));

    for (int j = 0; j < 15; j++)
    {

        if (tmp_ino.i_block[j] != -1)
        {
            fprintf(REPORTE_TREE, "<tr><td>%s%d</td><td>%d</td></tr>\n", "i_block_", j, tmp_ino.i_block[j]);
        }
    }

    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%c</td></tr>\n", "i_type", tmp_ino.i_type);
    fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%d</td></tr>\n", "i_perm", tmp_ino.i_perm);
    fprintf(REPORTE_TREE, "</table>>];\n");

    for (int j = 0; j < 15; j++)
    {

        if (tmp_ino.i_block[j] != -1)
        {
            fprintf(REPORTE_TREE, "inodo%d -> bloque%d\n", numero_inodo, tmp_ino.i_block[j]);
        }
    }


    for (i = 0; i < 12; i++)
    {

        if (tmp_ino.i_block[i] != -1)
        {
            fseek(ARCHIVO_DISCO, start_bloques + (sizeof (BLOQUE_APUNTADOR) * tmp_ino.i_block[i]), SEEK_SET);

            if (tmp_ino.i_type == '0')
            {
                fread(&tmp_carpeta, sizeof (BLOQUE_CARPETA), 1, ARCHIVO_DISCO);

                fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_ino.i_block[i]);
                fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
                fprintf(REPORTE_TREE, "<tr><td bgcolor = \"lawngreen\" colspan = \"2\">Bloque Carpeta %d</td></tr>\n", tmp_ino.i_block[i]);

                for (int k = 0; k < 4; k++)
                {

                    if (tmp_carpeta.b_content[k].b_inodo != -1)
                    {
                        fprintf(REPORTE_TREE, "<tr><td>%s</td><td>%d</td></tr>\n", tmp_carpeta.b_content[k].b_name, tmp_carpeta.b_content[k].b_inodo);
                    }
                }

                fprintf(REPORTE_TREE, "</table>>];\n");

                if (i != 0)
                {

                    for (int j = 0; j < 4; j++)
                    {

                        if (tmp_carpeta.b_content[j].b_inodo != -1)
                        {
                            tree_recursivo(path_disco, start_inodos, start_bloques, tmp_carpeta.b_content[j].b_inodo);
                            fprintf(REPORTE_TREE, "bloque%d -> inodo%d\n", tmp_ino.i_block[i], tmp_carpeta.b_content[j].b_inodo);
                        }
                    }
                }
                else
                {

                    for (int j = 2; j < 4; j++)
                    {

                        if (tmp_carpeta.b_content[j].b_inodo != -1)
                        {
                            tree_recursivo(path_disco, start_inodos, start_bloques, tmp_carpeta.b_content[j].b_inodo);
                            fprintf(REPORTE_TREE, "bloque%d -> inodo%d\n", tmp_ino.i_block[i], tmp_carpeta.b_content[j].b_inodo);
                        }
                    }
                }
            }
            else
            {
                fread(&tmp_archivo, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);
                fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_ino.i_block[i]);
                fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
                fprintf(REPORTE_TREE, "<tr><td bgcolor = \"yellow\" colspan = \"2\">Bloque Archivo %d</td></tr>\n", tmp_ino.i_block[i]);
                fprintf(REPORTE_TREE, "<tr><td colspan=\"2\">%s</td></tr>\n", tmp_archivo.b_content);
                fprintf(REPORTE_TREE, "</table>>];\n");
            }
        }
    }

    if (tmp_ino.i_block[12] != -1)
    {
        fseek(ARCHIVO_DISCO, start_bloques + (tmp_ino.i_block[12] * sizeof (BLOQUE_APUNTADOR)), SEEK_SET);
        fread(&tmp_aputador, sizeof (BLOQUE_APUNTADOR), 1, ARCHIVO_DISCO);

        fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_ino.i_block[12]);
        fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
        fprintf(REPORTE_TREE, "<tr><td bgcolor = \"darkorange\" colspan = \"2\">Bloque Apuntador %d</td></tr>\n", tmp_ino.i_block[12]);

        for (int j = 0; j < 16; j++)
        {

            if (tmp_aputador.b_pointers[j] != -1)
            {
                fprintf(REPORTE_TREE, "<tr><td>%s%d</td><td>%d</td></tr>\n", "i_block_", j, tmp_aputador.b_pointers[j]);
            }
        }

        fprintf(REPORTE_TREE, "</table>>];\n");

        for (int j = 0; j < 16; j++)
        {

            if (tmp_aputador.b_pointers[j] != -1)
            {
                fprintf(REPORTE_TREE, "bloque%d -> bloque%d\n", tmp_ino.i_block[12], tmp_aputador.b_pointers[j]);
            }
        }

        if (tmp_ino.i_type == '0')
        {

        }
        else
        {

            for (int i = 0; i < 16; i++)
            {

                if (tmp_aputador.b_pointers[i] != -1)
                {
                    fseek(ARCHIVO_DISCO, start_bloques + (sizeof (BLOQUE_ARCHIVO) * tmp_aputador.b_pointers[i]), SEEK_SET);
                    fread(&tmp_archivo, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);
                    fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_aputador.b_pointers[i]);
                    fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
                    fprintf(REPORTE_TREE, "<tr><td bgcolor = \"yellow\" colspan = \"2\">Bloque Archivo %d</td></tr>\n", tmp_aputador.b_pointers[i]);
                    fprintf(REPORTE_TREE, "<tr><td colspan=\"2\">%s</td></tr>\n", tmp_archivo.b_content);
                    fprintf(REPORTE_TREE, "</table>>];\n");
                }
            }
        }
    }

    if (tmp_ino.i_block[13] != -1)
    {
        fseek(ARCHIVO_DISCO, start_bloques + (tmp_ino.i_block[13] * sizeof (BLOQUE_APUNTADOR)), SEEK_SET);
        fread(&tmp_aputador_lvl1, sizeof (BLOQUE_APUNTADOR), 1, ARCHIVO_DISCO);

        fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_ino.i_block[13]);
        fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
        fprintf(REPORTE_TREE, "<tr><td colspan = \"2\">Bloque %d</td></tr>\n", tmp_ino.i_block[13]);

        for (int j = 0; j < 16; j++)
        {

            if (tmp_aputador_lvl1.b_pointers[j] != -1)
            {
                fprintf(REPORTE_TREE, "<tr><td>%s%d</td><td>%d</td></tr>\n", "i_block_", j, tmp_aputador_lvl1.b_pointers[j]);
            }
        }
        fprintf(REPORTE_TREE, "</table>>];\n");

        for (int j = 0; j < 16; j++)
        {

            if (tmp_aputador_lvl1.b_pointers[j] != -1)
            {
                fprintf(REPORTE_TREE, "bloque%d -> bloque%d\n", tmp_ino.i_block[13], tmp_aputador_lvl1.b_pointers[j]);
            }
        }
        if (tmp_ino.i_type == '0')
        {

        }
        else
        {

            for (int i = 0; i < 16; i++)
            {

                if (tmp_aputador_lvl1.b_pointers[i] != -1)
                {
                    fseek(ARCHIVO_DISCO, start_bloques + (sizeof (BLOQUE_ARCHIVO) * tmp_aputador_lvl1.b_pointers[i]), SEEK_SET);
                    fread(&tmp_aputador, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);

                    for (int i = 0; i < 16; i++)
                    {

                        if (tmp_aputador.b_pointers[i] != -1)
                        {
                            fseek(ARCHIVO_DISCO, start_bloques + (sizeof (BLOQUE_ARCHIVO) * tmp_aputador.b_pointers[i]), SEEK_SET);
                            fread(&tmp_archivo, sizeof (BLOQUE_ARCHIVO), 1, ARCHIVO_DISCO);
                            fprintf(REPORTE_TREE, "\t""bloque%d""[style = \"filled, bold\" penwidth = 0 fillcolor = \"white\" fontname= \"Courier New\" shape=\"Mrecord\"\n", tmp_aputador.b_pointers[i]);
                            fprintf(REPORTE_TREE, "label=<<table border = \"1\">\n");
                            fprintf(REPORTE_TREE, "<tr><td colspan = \"2\">Bloque Archivo %d</td></tr>\n", tmp_aputador.b_pointers[i]);
                            fprintf(REPORTE_TREE, "<tr><td colspan=\"2\">%s</td></tr>\n", tmp_archivo.b_content);
                            fprintf(REPORTE_TREE, "</table>>];\n");
                        }
                    }


                }
            }
        }
    }



}

void reporte_file(char* path_destino, MI_ARCHIVO contenido)
{
    FILE *REPORTE_ARCHIVO;
    REPORTE_ARCHIVO = fopen(path_destino, "w");
    int posicion =0;

    fseek(REPORTE_ARCHIVO, 0, SEEK_SET);

    while(contenido.contenido[posicion] != '\0')
    {
        fprintf(REPORTE_ARCHIVO, "%c", contenido.contenido[posicion]);
        posicion++;
    }

    fprintf(REPORTE_ARCHIVO, "\n");
    fclose(REPORTE_ARCHIVO);

}
