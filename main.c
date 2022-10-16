#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "includes/includes.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char *nombre_archivo = NULL;
bool descendente = false;

int bandera = 0;
int opc = 0;

void intercambiarFecha(int *fecha){
		
	int anio = fecha[0];
	int mes = fecha[1];
	int dia = fecha[2];
	
	int fechaAux[2];
	
	fechaAux[2] = anio;
	fechaAux[1] = mes;
	fechaAux[0] = dia;
	
	fecha = fechaAux;
	
}

void particionarFecha(char *fecha, int *date){

	int i = 0;
	
    char *palabra = strtok(fecha, "-");
        
    while(palabra != NULL) {		
		
		date[i] = atoi(palabra);
        palabra = strtok(NULL, "-");
        ++i;
    }
}

void obtenerFechaActual(int *fechaActual){
	time_t t;
	struct tm *tm;
	char fechayhora[50];
	
	t=time(NULL);
	tm=localtime(&t);
	
	strftime(fechayhora, 50, "%d", tm);
	fechaActual[0] = atoi(fechayhora);

	strftime(fechayhora, 50, "%m", tm);
	fechaActual[1] = atoi(fechayhora);
	
	strftime(fechayhora, 50, "%Y", tm);
	fechaActual[2] = atoi(fechayhora);

}

int esFechaMenor(char *fecha){
	
	char fechaAux1[MAXFECHA];
	strcpy(fechaAux1, fecha);
	int fechaAux[2];
	particionarFecha(fechaAux1, fechaAux);
	
	char fechaActual1[MAXFECHA];
	strcpy(fechaActual1, getFecha());
	int fechaActual[2];
	particionarFecha(fechaActual1, fechaActual);

	int estado;
	
	//SI EL ANIO INGRESADO ES MAYOR QUE EL ANIO ACTUAL 
	if (fechaAux[0] > fechaActual[0])
		estado = 0;
	else{
		//SI ESTAMOS EN EL MISMO ANIO PERO EL MES ES MAYOR RETORNA
		if ((fechaAux[0] == fechaActual[0]) && (fechaAux[1] > fechaActual[1]))
			estado = 0;
		else{
			if ((fechaAux[0] == fechaActual[0]) && (fechaAux[1] == fechaActual[1]) && (fechaAux[2] > fechaActual[2]))
				estado = 0;
			else
				estado = 1;
		}
	}
	
	if (estado == 0){
		printf("ERROR: La fecha ingresada es MAYOR o IGUAL que la fecha actual");
		system("pause");
	}
	return estado;
}

int esFechaMayor(char *fecha){

	char fechaAux1[MAXFECHA];
	strcpy(fechaAux1, fecha);
	int fechaAux[2];
	particionarFecha(fechaAux1, fechaAux);
	
	char fechaActual1[MAXFECHA];
	strcpy(fechaActual1, getFecha());
	int fechaActual[2];
	particionarFecha(fechaActual1, fechaActual);
	
	//SI EL ANIO INGRESADO ES MENOR QUE EL ANIO ACTUAL 
	if (fechaAux[0] < fechaActual[0])
		return 0;
	else{
		//SI ESTAMOS EN EL MISMO ANIO PERO EL MES ES MENOR RETORNA
		if ((fechaAux[0] == fechaActual[0]) && (fechaAux[1] < fechaActual[1]))
			return 0;
		else{
			if ((fechaAux[0] == fechaActual[0]) && (fechaAux[1] == fechaActual[1]) && (fechaAux[2] < fechaActual[2]))
				return 0;
			else
				return 1;
		}
	}
}

bool verificarFecha(char* fecha){
	
    int date[2];  
	
	char fechaAux[MAXFECHA];

	strcpy(fechaAux,fecha);

	if ((fechaAux[4] == '-') && (fechaAux[7] == '-') && (fechaAux[9] >=0)){
    
	    particionarFecha(fecha, date); 
	    
	    if ((date[2]>0) && (date[2]<=31)){
	    	
			if (date[1] >= 1 && date[1] <= 12 ){
		        switch (date[1] ){
					case  1 :
		            case  3 :
		            case  5 :
		            case  7 :
		            case  8 :
		            case 10 :
					case 12 :
						if ( date[2] < 1 && date[2] > 31 )
		                	return 0;
		                break;
		            case  4 :
		            case  6 :
		            case  9 :				
					case 11 :
						if ( date[2] < 1 && date[2] > 30 )
		            		return 0;
		               	break;				
					
					case  2 :
						if( date[0] % 4 != 0 && date[0] % 100 == 0 || date[0] % 400 != 0 )
		                	if ( date[2] < 1 && date[2] > 29 )
								return 0;
		                else
		                    if ( date[2] < 1 && date[2] > 28 )
		                    	return 0;
		                
		               	break;
		        }
		    }else 
				return 0;
	    }else
	    	return 0;
	}else
		return 0;
			    
    return 1; 
}


void ingresarNumero(char *pregunta, char *cadena){
	
	int size;
	
	do{
		fflush( stdin ); // limpia el buffer
		printf("%s", pregunta);
		scanf("%s",cadena);
		size = strlen(cadena)-1;
		bandera = verificarNum(cadena,size);
	}while(bandera == 0);
}

int verificarCadena (char *texto){
	
	texto[strcspn(texto, "\r\n")] = 0;
		
	if (texto[0] == '\0'){
		printf("\nERROR: La cadena esta vacia\n\n");
		system("pause\n\n");
		return 0;   
	}else
		return 1; 	
}

void ingresarCadena(char *texto, char *cadena){
	
	do{
		fflush( stdin ); // limpia el buffer
		printf("%s", texto);
		fgets(cadena);	
		bandera = verificarCadena(cadena);
	}while(bandera == 0);
}



void ingresarFecha(char *pregunta, char *cadena){

	char fecha[MAXFECHA];
	
	do{
		ingresarCadena(pregunta, cadena);
		strcpy(fecha, cadena);
		bandera = verificarFecha(fecha);
		if(bandera == 0){
			printf("\nERROR: Formato de fecha incorrecto.\n");
			printf("Reintente la operacion. Ejemplo de fecha: 1994-05-01\n\n");
			system("pause");
		}
	}while(bandera == 0);			
}


bool continuar (char *pregunta)
{
	char respuesta[4]="";
	int size;	
	
	do{
		bandera = 0;
		printf("%s",pregunta);
		scanf("%s",respuesta);//(cadena);
		size = strlen(respuesta)-1;
		
		if (size == 0)
		{
			if ((respuesta[size] == 's') || (respuesta[size] == 'S') || (respuesta[size]== 'n') || (respuesta[size] == 'N'))
			{
				if ((respuesta[size] == 's') || (respuesta[size] == 'S'))
					return true;
				else
					return false;
			}else{
				printf("\n\nERROR: El dato ingresado es incorrecto\n\n");
				system("pause");
			}
		}else
			printf("ERROR: longitud invalida\n\n");
	}while(bandera == 0);
}

int verificarNum (char *numero, int longitud)
{
	int i=0;
	
	while(i <= longitud){
		if (isdigit(numero[i]))
			i++;
		else {
			printf("\nERROR: El dato ingresado es incorrecto.\n\n");
			system("pause\n\n");
			return 0;
		}
	}
	return 1;
}
//********************************************************************************
static void opcionActualizaciones(){
		
	do{
		system("cls");
		opc = menuActualizaciones();
		system("cls");
		switch (opc){
			case 1:
				actualizarLocalidad();			
				break;
			case 2:
				actualizarTipoVacuna();
				break;
			case 3:
				actualizarVacuna();
				break;
			case 4:
				actualizarPaciente();
				break;
			case 5:
				actualizarEnfermero();
				break;
			case 6:
				actualizarCentroAtencion();
				break;
			case 7:
				actualizarTurno();
				break;
			case 8:  
				break;
			default:
				printf("Opcion incorrecta\n\n");
				system("pause");
				break;
		}
	}while(opc!=8);
}

static void opcionListados(){
		
	do{ 
		system("cls");
		opc = menuListados();
		system("cls");
		switch (opc){
			case 1:
				listarLocalidades(nombre_archivo, descendente);
				system("pause");
				break;
			case 2:
				listarTipoVacunas(nombre_archivo, descendente);
				system("pause");
				break;
			case 3:
				listarVacunas(nombre_archivo, descendente);
				system("pause");
				break;
			case 4:
				listarPacientes(nombre_archivo, descendente);
				system("pause");
				break;
			case 5:
				listarEnfermeros(nombre_archivo, descendente);
				system("pause");
				break;
			case 6:
				listarCentrosAtencion(nombre_archivo, descendente);
				system("pause");
				break;
			case 7:
				listarTurnos(nombre_archivo, descendente);
				system("pause");
				break;
			case 8:
				listarPacientesPorLocalidad(nombre_archivo,descendente);
				system("pause");
				break;
			case 9:
				listarPacientesPorVacuna(nombre_archivo,descendente);
				system("pause");
				break;
			case 10:
				listarEnfermerosVacuna(nombre_archivo,descendente);
				system("pause");
				break;
			case 11:
				listadoTurnosCentroAtencion(nombre_archivo,descendente);
				system("pause");
				break;
			case 12: 
				break;
			default:
				printf("Opcion incorrecta\n\n");
				system("pause");
				break;
		}
	}while(opc!=12);
}

static void opcionIngresos(){
		
	do{
		opc = menuIngresos();
		system("cls");
		switch (opc){
			case 1:
				registrarLocalidad();			
				break;
			case 2:
				registrarTipoVacuna();
				break;
			case 3:
				registrarVacuna();
				break;
			case 4:
				registrarPaciente();
				break;
			case 5:
				registrarEnfermero();
				break;
			case 6:
				registrarCentroAtencion();
				break;
			case 7:
				registrarTurno();
				break;
			case 8:  
				break;
			default:
				printf("Opcion incorrecta\n\n");
				system("pause");
				break;
		}
	}while(opc!=8);
}

int main(int argc, char *argv[])
{
	int i=0,size=0;
        
    //Se conecta con la base
	if(!init_config(argv[POS_CONFIG]))
		exit(-1);  
		
	if (argc > 2){
        if (strcmp(argv[2], "-f") == 0){
            nombre_archivo = argv[3];
        	if ((strcmp(argv[4], "-d") == 0) || (strcmp(argv[2], "-d") == 0))
            	descendente = true;
    	}
  	}
	//registrarTurno();
	do{ 
		opc = menuPrincipal();
		system("cls");

		switch (opc){
			case 1:
				opcionIngresos();
				break;
			case 2:
				opcionActualizaciones();
				break;
			case 3:
				opcionListados();
				break;
			case 4: 
				break;
			default:
				printf("Opcion incorrecta\n\n");
				system("pause");
				break;
		}
	}while(opc!=4);
	
	return 0;
}
