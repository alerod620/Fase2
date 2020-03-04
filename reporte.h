#ifndef REPORTE_H
#define REPORTE_H

void reporte_disco(char* path_disco,char* path_destino_imagen);
void reporte_mbr(char* path_disco,char* path_destino_imagen);

double porcentaje_p(double tam_part, double tam_mbr);
double porcentaje(double pos_inicio,double pos_fin,double tam_mbr);

void obtener_formato(char path[]);

void limpiar();

#endif // REPORTE_H
