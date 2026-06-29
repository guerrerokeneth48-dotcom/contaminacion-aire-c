#include <stdio.h>
#include <string.h>

struct Zona {
    char nombre[30];
    float co2;
    float so2;
    float no2;
    float pm25;
    float temperatura;
    float viento;
    float humedad;
    float historico[30];
};

struct Zona zonas[5];
int total = 5;

float limitePM25 = 15;
float limiteNO2 = 25;
float limiteSO2 = 40;
float limiteCO2 = 1000;

int cargarDatos() {
    int i, d;
    float bases[5] = {17, 12, 20, 10, 14};

    strcpy(zonas[0].nombre, "Centro");
    zonas[0].co2 = 950; zonas[0].so2 = 35; zonas[0].no2 = 28; zonas[0].pm25 = 18;
    zonas[0].temperatura = 22; zonas[0].viento = 8; zonas[0].humedad = 70;

    strcpy(zonas[1].nombre, "Norte");
    zonas[1].co2 = 800; zonas[1].so2 = 20; zonas[1].no2 = 15; zonas[1].pm25 = 12;
    zonas[1].temperatura = 20; zonas[1].viento = 14; zonas[1].humedad = 65;

    strcpy(zonas[2].nombre, "Sur");
    zonas[2].co2 = 1100; zonas[2].so2 = 45; zonas[2].no2 = 30; zonas[2].pm25 = 22;
    zonas[2].temperatura = 24; zonas[2].viento = 6; zonas[2].humedad = 75;

    strcpy(zonas[3].nombre, "Valle");
    zonas[3].co2 = 700; zonas[3].so2 = 18; zonas[3].no2 = 12; zonas[3].pm25 = 9;
    zonas[3].temperatura = 19; zonas[3].viento = 16; zonas[3].humedad = 60;

    strcpy(zonas[4].nombre, "Cumbaya");
    zonas[4].co2 = 880; zonas[4].so2 = 30; zonas[4].no2 = 20; zonas[4].pm25 = 14;
    zonas[4].temperatura = 23; zonas[4].viento = 11; zonas[4].humedad = 68;

    for (i = 0; i < total; i++) {
        for (d = 0; d < 30; d++) {
            zonas[i].historico[d] = bases[i] - 3 + (d % 7);
        }
    }
    return 0;
}

float predecir(struct Zona *z) {
    int d;
    int peso;
    float suma = 0;
    float pesos = 0;
    float resultado;
    float *p = &suma;

    for (d = 23; d < 30; d++) {
        peso = d - 22;
        *p = *p + (z->historico[d] * peso);
        pesos = pesos + peso;
    }
    resultado = suma / pesos;

    if (z->viento < 10) {
        resultado = resultado + resultado * 0.10;
    }
    if (z->temperatura > 30) {
        resultado = resultado + resultado * 0.05;
    }
    if (z->humedad > 80) {
        resultado = resultado - resultado * 0.05;
    }
    return resultado;
}

float promedioHistorico(struct Zona *z) {
    int d;
    float suma = 0;
    for (d = 0; d < 30; d++) {
        suma = suma + z->historico[d];
    }
    return suma / 30;
}

int monitoreo() {
    int i;
    printf("\nMonitoreo de contaminacion actual:\n");
    for (i = 0; i < total; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("CO2: %.1f ppm  ", zonas[i].co2);
        if (zonas[i].co2 > limiteCO2) printf("EXCEDE\n"); else printf("OK\n");
        printf("SO2: %.1f      ", zonas[i].so2);
        if (zonas[i].so2 > limiteSO2) printf("EXCEDE\n"); else printf("OK\n");
        printf("NO2: %.1f      ", zonas[i].no2);
        if (zonas[i].no2 > limiteNO2) printf("EXCEDE\n"); else printf("OK\n");
        printf("PM2.5: %.1f    ", zonas[i].pm25);
        if (zonas[i].pm25 > limitePM25) printf("EXCEDE\n"); else printf("OK\n");
    }
    return 0;
}

int prediccion() {
    int i;
    float pred;
    printf("\nPrediccion de PM2.5 para las proximas 24 horas:\n");
    for (i = 0; i < total; i++) {
        pred = predecir(&zonas[i]);
        printf("\nZona %s -> PM2.5 previsto: %.1f\n", zonas[i].nombre, pred);
        if (pred > limitePM25) {
            printf("ALERTA: se preve que se supere el limite (%.0f)\n", limitePM25);
        } else {
            printf("Nivel dentro de lo aceptable\n");
        }
    }
    return 0;
}

int promedios() {
    int i;
    float prom;
    printf("\nPromedios historicos de los ultimos 30 dias:\n");
    for (i = 0; i < total; i++) {
        prom = promedioHistorico(&zonas[i]);
        printf("Zona %s -> promedio PM2.5: %.1f  ", zonas[i].nombre, prom);
        if (prom > limitePM25) printf("supera el limite OMS\n");
        else printf("dentro del limite OMS\n");
    }
    return 0;
}

int recomendaciones() {
    int i;
    float pred;
    printf("\nRecomendaciones:\n");
    for (i = 0; i < total; i++) {
        pred = predecir(&zonas[i]);
        printf("\nZona %s:\n", zonas[i].nombre);
        if (pred > limitePM25 || zonas[i].pm25 > limitePM25) {
            printf("- Reducir el trafico vehicular en la zona\n");
            printf("- Suspender actividades al aire libre\n");
            printf("- Cierre temporal de industrias contaminantes\n");
        } else {
            printf("- Niveles aceptables, mantener el monitoreo\n");
        }
    }
    return 0;
}

int exportar() {
    int i;
    FILE *archivo;
    archivo = fopen("reporte.txt", "w");
    if (archivo == NULL) {
        printf("\nNo se pudo crear el archivo.\n");
        return 0;
    }

    fprintf(archivo, "REPORTE DE CONTAMINACION DEL AIRE\n");
    fprintf(archivo, "=================================\n\n");
    for (i = 0; i < total; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "CO2: %.1f  SO2: %.1f  NO2: %.1f  PM2.5: %.1f\n",
                zonas[i].co2, zonas[i].so2, zonas[i].no2, zonas[i].pm25);
        fprintf(archivo, "Promedio historico PM2.5: %.1f\n", promedioHistorico(&zonas[i]));
        fprintf(archivo, "PM2.5 previsto 24h: %.1f\n\n", predecir(&zonas[i]));
    }
    fclose(archivo);
    printf("\nReporte guardado en reporte.txt\n");
    return 0;
}

int main() {
    int opcion;

    cargarDatos();

    do {
        printf("\n1. Monitoreo de contaminacion actual\n");
        printf("2. Prediccion y alertas\n");
        printf("3. Promedios historicos\n");
        printf("4. Recomendaciones\n");
        printf("5. Exportar reporte\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) monitoreo();
        else if (opcion == 2) prediccion();
        else if (opcion == 3) promedios();
        else if (opcion == 4) recomendaciones();
        else if (opcion == 5) exportar();

    } while (opcion != 0);

    return 0;
}
