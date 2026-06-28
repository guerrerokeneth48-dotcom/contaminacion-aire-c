/*
   Sistema de Gestion y Prediccion de Contaminacion del Aire
   Programacion 1 - ISWZ1102
   Integrantes:
   -
   -
   -
*/

#include <stdio.h>
#include <string.h>

#define NUM_ZONAS 5
#define DIAS 30

// Limites aceptables segun la OMS (ug/m3 y ppm)
#define LIM_PM25 15
#define LIM_NO2 25
#define LIM_SO2 40
#define LIM_CO2 1000

// Estructura para guardar los datos de cada zona
struct Zona {
    char nombre[30];
    float co2;
    float so2;
    float no2;
    float pm25;
    float temperatura;
    float viento;
    float humedad;
    float historico[DIAS];  // PM2.5 de los ultimos 30 dias
};

// Carga los datos de ejemplo de las 5 zonas
void cargarDatos(struct Zona zonas[]) {
    float bases[NUM_ZONAS] = {17, 12, 20, 10, 14};
    int i, d;

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

    // Llenamos el historico de los ultimos 30 dias de cada zona
    for (i = 0; i < NUM_ZONAS; i++) {
        for (d = 0; d < DIAS; d++) {
            zonas[i].historico[d] = bases[i] - 3 + (d % 7);
        }
    }
}

void mostrarMenu() {
    printf("\n=============================================\n");
    printf("  SISTEMA DE CONTAMINACION DEL AIRE\n");
    printf("=============================================\n");
    printf("1. Monitoreo de contaminacion actual\n");
    printf("2. Prediccion de niveles (24 horas) y alertas\n");
    printf("3. Promedios historicos (30 dias)\n");
    printf("4. Recomendaciones\n");
    printf("5. Exportar reporte a archivo\n");
    printf("6. Salir\n");
    printf("Elija una opcion: ");
}

// Requerimiento 1: muestra los niveles actuales y los compara con los limites
void monitoreoActual(struct Zona zonas[]) {
    int i;
    printf("\n--- MONITOREO DE CONTAMINACION ACTUAL ---\n");
    for (i = 0; i < NUM_ZONAS; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        printf("  CO2 : %.1f ppm   ", zonas[i].co2);
        if (zonas[i].co2 > LIM_CO2) printf("(EXCEDE)\n"); else printf("(OK)\n");
        printf("  SO2 : %.1f       ", zonas[i].so2);
        if (zonas[i].so2 > LIM_SO2) printf("(EXCEDE)\n"); else printf("(OK)\n");
        printf("  NO2 : %.1f       ", zonas[i].no2);
        if (zonas[i].no2 > LIM_NO2) printf("(EXCEDE)\n"); else printf("(OK)\n");
        printf("  PM2.5: %.1f      ", zonas[i].pm25);
        if (zonas[i].pm25 > LIM_PM25) printf("(EXCEDE)\n"); else printf("(OK)\n");
    }
}

// Requerimiento 2: predice el PM2.5 de las proximas 24h con promedio ponderado
// Usa un puntero a la zona
float predecir(struct Zona *z) {
    int d;
    float suma = 0, pesos = 0, base, factor = 1.0;
    int peso;

    // Promedio ponderado de los ultimos 7 dias (mas peso a los dias recientes)
    for (d = DIAS - 7; d < DIAS; d++) {
        peso = d - (DIAS - 7) + 1;   // 1,2,3,4,5,6,7
        suma = suma + z->historico[d] * peso;
        pesos = pesos + peso;
    }
    base = suma / pesos;

    // Ajuste por el clima actual
    if (z->viento < 10) factor = factor + 0.10;     // poco viento concentra la contaminacion
    if (z->temperatura > 30) factor = factor + 0.05;
    if (z->humedad > 80) factor = factor - 0.05;

    return base * factor;
}

// Requerimiento 3: muestra la prediccion y emite alertas
void prediccionYAlertas(struct Zona zonas[]) {
    int i;
    float pred;
    printf("\n--- PREDICCION 24H Y ALERTAS ---\n");
    for (i = 0; i < NUM_ZONAS; i++) {
        pred = predecir(&zonas[i]);
        printf("\nZona %s -> PM2.5 previsto: %.1f\n", zonas[i].nombre, pred);
        if (pred > LIM_PM25) {
            printf("  ALERTA: se preve que la contaminacion supere el limite (%d)\n", LIM_PM25);
        } else {
            printf("  Nivel dentro de lo aceptable\n");
        }
    }
}

// Requerimiento 4: promedio del historico y comparacion con limite OMS
// Usa un puntero a la zona
float promedioHistorico(struct Zona *z) {
    int d;
    float suma = 0;
    for (d = 0; d < DIAS; d++) {
        suma = suma + z->historico[d];
    }
    return suma / DIAS;
}

void promediosHistoricos(struct Zona zonas[]) {
    int i;
    float prom;
    printf("\n--- PROMEDIOS HISTORICOS (30 DIAS) ---\n");
    for (i = 0; i < NUM_ZONAS; i++) {
        prom = promedioHistorico(&zonas[i]);
        printf("Zona %s -> promedio PM2.5: %.1f  ", zonas[i].nombre, prom);
        if (prom > LIM_PM25) printf("(supera el limite OMS)\n");
        else printf("(dentro del limite OMS)\n");
    }
}

// Requerimiento 5: recomendaciones de mitigacion
void recomendaciones(struct Zona zonas[]) {
    int i;
    float pred;
    printf("\n--- RECOMENDACIONES ---\n");
    for (i = 0; i < NUM_ZONAS; i++) {
        pred = predecir(&zonas[i]);
        printf("\nZona %s:\n", zonas[i].nombre);
        if (pred > LIM_PM25 || zonas[i].pm25 > LIM_PM25) {
            printf("  - Reducir el trafico vehicular en la zona\n");
            printf("  - Suspender actividades al aire libre\n");
            printf("  - Cierre temporal de industrias contaminantes\n");
        } else {
            printf("  - Los niveles son aceptables, mantener el monitoreo\n");
        }
    }
}

// Requerimiento 6: exporta los datos y predicciones a un archivo
void exportarReporte(struct Zona zonas[]) {
    int i;
    FILE *archivo;
    archivo = fopen("reporte.txt", "w");
    if (archivo == NULL) {
        printf("\nNo se pudo crear el archivo.\n");
        return;
    }

    fprintf(archivo, "REPORTE DE CONTAMINACION DEL AIRE\n");
    fprintf(archivo, "=================================\n\n");
    for (i = 0; i < NUM_ZONAS; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombre);
        fprintf(archivo, "  CO2: %.1f  SO2: %.1f  NO2: %.1f  PM2.5: %.1f\n",
                zonas[i].co2, zonas[i].so2, zonas[i].no2, zonas[i].pm25);
        fprintf(archivo, "  Promedio historico PM2.5: %.1f\n", promedioHistorico(&zonas[i]));
        fprintf(archivo, "  PM2.5 previsto 24h: %.1f\n\n", predecir(&zonas[i]));
    }
    fclose(archivo);
    printf("\nReporte guardado en 'reporte.txt'\n");
}

int main() {
    struct Zona zonas[NUM_ZONAS];
    int opcion;

    cargarDatos(zonas);

    do {
        mostrarMenu();
        scanf("%d", &opcion);

        switch (opcion) {
            case 1: monitoreoActual(zonas); break;
            case 2: prediccionYAlertas(zonas); break;
            case 3: promediosHistoricos(zonas); break;
            case 4: recomendaciones(zonas); break;
            case 5: exportarReporte(zonas); break;
            case 6: printf("\nSaliendo del programa...\n"); break;
            default: printf("\nOpcion no valida\n");
        }
    } while (opcion != 6);

    return 0;
}
