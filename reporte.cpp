#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <reporte.h>
#include <estructuras_disco.h>
#include <ejecutar_comando.h>

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
