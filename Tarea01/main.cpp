/* 
 * Tarea 01 - Ingeniería de Software
 *
 * Profesor:    Sebastian Salazar Molina.
 * Integrantes: Manuel Irarrázaval Gálvez.
 *              Fabian Cordovez.
 *              Luis Sandoval Romero.
 * 
 * Fecha entrega: 05 de Octubre de 2014.
 */

//Librerías
#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <postgresql/libpq-fe.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include "gd.h"
#include "conexion.h"
#include "gdfonts.h"

#define LargoConsulta 200
#define _TIME_
#define N 15
#define IMG_WIDTH 600
#define IMG_HEIGHT 180
#define BORDE_ANCHO 580
#define BORDE_ALTO 150

 /*
  *  Instrucciones de compilacion:
  * Ejecutar el siguiente código en la consola
g++ tarea01.cpp -o test -I/usr/include/postgresql -I/usr/include -lpq -lgd
*/

 using namespace std;
 
 /* 
  * Variables Globales 
  */
char Consulta[LargoConsulta]; //Cadena que permite la consulta
char *fecha_i,*fecha_f; 	 //atributos de respaldo de argumentos para la direccion y fechas.
struct tm fecha_desde; 			// estructura para trabajar la fecha de inicio
struct tm fecha_hasta; 			// estructura para trabajar la fecha de termino

 /* 
  * FUNCION QUE PERMITE IR LIBERANDO LA VARIABLE DE CONSULTA PARA EVITAR ERRORES */

void LimpiarConsulta(void)
{
    for (int i = 0; i < LargoConsulta; i++ )
    Consulta[i] = '\0';
}
 /* 
  * FUNCION PARA MOSTRAR LOS INTEGRANTES DEL GRUPO */
void integrantes(){
    printf("\nIntegrantes:\nIrarrazaval Galvez, Manuel\nCordovez, Fabian\nSandoval Romero, Luis");
    printf("Fecha de compilacion: %s - %s\n\n ", __DATE__, __TIME__);//Fecha de compilacion. 
}
 /*
  *  Funcion para corroborar fechas */
void Comprobar_Fechas(char *Inicio, char *Termino)
{
    char chartoint[5]; 			// char para respaldo y apoyo para traspasar las fechas a digitos individuales
    fecha_i=Inicio;
    fecha_f=Termino;
  /*
   *  comprobamos la estructura de las fechas. */
    if((strlen(fecha_i)!=10)||(fecha_i[4]!='-')||(fecha_i[7]!='-')) {
        cout<<"Fecha Inicial Erronea"<<endl;
        exit(0);
        //return EXIT_SUCCESS;
    }
    if((strlen(fecha_f)!=10)||(fecha_f[4]!='-')||(fecha_f[7]!='-')) {
        cout<<"Fecha Final Erronea"<<endl;
        exit(0);
        //return EXIT_SUCCESS;
    }
  /* 
   * respaldamos las fechas divididas en año, mes y dia de las fechas de inicio y fin. */
    chartoint[0]=fecha_i[0];
    chartoint[1]=fecha_i[1];
    chartoint[2]=fecha_i[2];
    chartoint[3]=fecha_i[3];
    chartoint[4]='\n';
    fecha_desde.tm_year=atoi(chartoint);
    chartoint[0]=fecha_i[5];
    chartoint[1]=fecha_i[6];
    chartoint[2]='\n';
    fecha_desde.tm_mon=atoi(chartoint)-1;
    chartoint[0]=fecha_i[8];
    chartoint[1]=fecha_i[9];
    chartoint[2]='\n';
    fecha_desde.tm_mday=atoi(chartoint);
    chartoint[0]=fecha_f[0];
    chartoint[1]=fecha_f[1];
    chartoint[2]=fecha_f[2];
    chartoint[3]=fecha_f[3];
    chartoint[4]='\n';
    fecha_hasta.tm_year=atoi(chartoint);
    chartoint[0]=fecha_f[5];
    chartoint[1]=fecha_f[6];
    chartoint[2]='\n';
    fecha_hasta.tm_mon=atoi(chartoint)-1;
    chartoint[0]=fecha_f[8];
    chartoint[1]=fecha_f[9];
    chartoint[2]='\n';
    fecha_hasta.tm_mday=atoi(chartoint);
    /* comprobamos si las fechas son reales o correctas. */
    if((fecha_desde.tm_mon>11)||(fecha_desde.tm_mday>31)) {
        cout << "Fecha Inicial Erronea" << endl;
    exit(0);
    //return EXIT_SUCCESS;
    }
    if((fecha_hasta.tm_mon>11)||(fecha_hasta.tm_mday>31)) {
        cout<<"Fecha Final Erronea"<<endl;
        exit(0);
    //return EXIT_SUCCESS;
    }
    cout << "Fechas Correctamente Validadas" << endl;
}



int main(int argc, char *argv[])
{
/*Variables de la funcion principal*/
    char chartoint[5]; 			// char para respaldo y apoyo para traspasar las fechas a digitos individuales
    int lineas_totales=0; 			// entero que permite crear el largo total de registro de ventas
    int i=0; 			// contador
    ofstream archivoCSV; // variable tipo archivo para generar CSV con peticiones
    int CantidadxMes [12]; // Array que almacena la cantidad de peticiones por mes

    //Conexión con BD postgresql //
    PGconn *conexion = NULL;
    PGresult *resultado_SQL = NULL;
    const char *servidor = "146.83.181.4";
    const char *puerto = "6432";
    const char *esquema = "isw";
    const char *baseDato = "iswdb";
    const char *usuario = "estisw";
    const char *contrasena = "estisw";
 /* Variables usadas para mostrar datos en JPEG */
    
    gdImagePtr imagen;
    FILE *archivo;
    gdFontPtr fuente = gdFontGetSmall();
    imagen = gdImageCreateTrueColor(IMG_WIDTH, IMG_HEIGHT);
    int blanco, negro;
    char titulo[513];
 /* Validación de argumentos de entrada con opciones -g -t -a -v */
        if((argc==2)||(argc==4))
        {
          if((argv[1][0]!='-')){
            cout<<"Error en ingreso de argumentos."<<endl;
            return EXIT_SUCCESS;
        }
        if (argv[1][1] == 'g'){
        //opcion "g", respaldamos los argumentos.
        Comprobar_Fechas(argv[2], argv[3]);
        LimpiarConsulta();
        const char* queryG = ("SELECT estado AS \"Estado\", COUNT(peticion) AS \"Cantidad de peticiones\" FROM isw.accesos GROUP BY fecha"); 
          /*Conexion con la base de datos*/
        conexion* = new conexion(servidor,puerto,baseDato,usuario,contrasena);
        // muestra la consulta
        consulta->showQuery(queryG);
          if (PQstatus(conexion) != CONNECTION_BAD) {
            /*Seccion que genera el JPG*/
            blanco = gdImageColorAllocate(imagen, 255, 255, 255);
            negro = gdImageColorAllocate(imagen, 0, 0, 0);
            // Se pinta el fondo Blanco
            gdImageFill(imagen, 0, 0, blanco);
            // Se imprime el titulo
            memset(titulo, 0, 513);
            snprintf(titulo, 512, "LAS PETICIONES ENTRE LAS FECHAS INGRESADAS [%s] a [%s] SE DEFINE", fecha_i, fecha_f);
            gdImageString(imagen, fuente, (int) IMG_WIDTH * 0.15, 10, (unsigned char *) titulo, negro);
            fflush(stdout);
            // Se enmarcan los valores en un rectangulo
            gdImageLine(imagen, BORDE_ANCHO, BORDE_ALTO, (IMG_WIDTH - BORDE_ANCHO),BORDE_ALTO, negro);
            gdImageLine(imagen, BORDE_ANCHO, (IMG_HEIGHT - BORDE_ALTO), (IMG_WIDTH - BORDE_ANCHO), (IMG_HEIGHT - BORDE_ALTO), negro);
            gdImageLine(imagen, BORDE_ANCHO, BORDE_ALTO, BORDE_ANCHO, (IMG_HEIGHT - BORDE_ALTO), negro);
            gdImageLine(imagen, (IMG_WIDTH - BORDE_ANCHO), BORDE_ALTO, (IMG_WIDTH - BORDE_ANCHO), (IMG_HEIGHT - BORDE_ALTO), negro);
            // Se guarda la imagen en un archivo
            archivo = fopen("PeticionesMes.jpg", "wb");
          if (archivo != NULL) {
              gdImageJpeg(imagen, archivo, 100);
              fclose(archivo);
          }
          else{
              cout << "\n\n No se pudo crear el archivo jpg" << endl;
          }
          gdImageDestroy(imagen);
            /*FIN Seccion que genera el JPG*/
          }
          else cout<<"Hubo un error en la conexion a la base de datos"<<endl;
          PQfinish(conexion);
        /* Fin de la conexion con la base de datos*/
        }//fin opcion -g
          
        if (argv[1][1] == 't'){//opcion "t", respaldamos los argumentos.
            Comprobar_Fechas(argv[2], argv[3]);
            LimpiarConsulta();
        }
        if (argv[1][1] == 'a'){ //opcion "a", respaldamos los argumentos.
            Comprobar_Fechas(argv[2], argv[3]);
            LimpiarConsulta();
          /*Creación de Archivo CSV con las 100 peticiones*/
            archivoCSV.open ("CienPeticiones.csv");
            archivoCSV << "IP;FECHA;PETICION;ESTADO;TAMANO;REFERER;USERAGENT";
            const char* queryA = ("SELECT accesos.peticion, Count(accesos.peticion) INTO OUTFILE '/CienPeticiones.cvs' FIELDS TERMINATED BY ';' BY ',' LINES TERMINATED BY ' '  FROM isw.accesos WHERE accesos.fecha BETWEEN 'argv[2]' and 'argv[3]' GROUP BY accesos.peticion, accesos.fecha ORDER BY Count(accesos.peticion) DESC");
            
          /*Conexion con la base de datos*/
          /*Fin de conexion con la base de datos*/
          archivoCSV.close();
        }
            if (argv[1][1] == 'v'){
                integrantes();
            }//fin if de argumentos validos
        }
        else {
            cout << "Opcion no valida, ingrese correctamente los parametros" << endl;
        }
return EXIT_SUCCESS;
}