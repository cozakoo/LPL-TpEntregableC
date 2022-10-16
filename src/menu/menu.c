#include <stdio.h>
#include <stdlib.h>

static int menuPrincipal()
{
	system("cls");

	int opcion=0;
	
	printf("BIENVENIDO AL SISTEMA DE REGISTRO DE VACUNACION COVID-19!\n\n");
	printf("MENU DE OPCIONES\n");
	printf("1- INGRESOS\n"); 
	printf("2- ACTUALIZACIONES\n"); 
	printf("3- LISTADOS\n"); 
	printf("4- SALIR\n\n");
	
	printf("INDIQUE LA OPERACION A REALIZAR\n");
	scanf("%d", &opcion);

	return opcion;
}

static int menuIngresos()
{
	int opcion=0;	

	printf("BIENVENIDO AL SISTEMA DE REGISTRO DE VACUNACION COVID-19!\n\n");
	printf("MENU DE INGRESOS\n");
	printf("1- INGRESAR LOCALIDAD\n");
	printf("2- INGRESAR TIPO DE VACUNA\n");
	printf("3- INGRESAR VACUNA\n"); 
	printf("4- INGRESAR PACIENTE\n"); 
	printf("5- INGRESAR ENFERMERO/E\n"); 
	printf("6- INGRESAR CENTRO DE ATENCION\n"); 
	printf("7- INGRESAR TURNO\n");	
	printf("8- VOLVER AL MENU PRINCIPAL\n\n");
		
	printf("INDIQUE LA OPERACION A REALIZAR\n");
	scanf("%d",&opcion);

	return opcion;
}

static int menuActualizaciones()
{
	int opcion=0;
	
	printf("BIENVENIDO AL SISTEMA DE REGISTRO DE VACUNACION COVID-19!\n\n");
	printf("MENU DE ACTUALIZACIONES\n");
	printf("1- ACTUALIZAR LOCALIDAD\n");
	printf("2- ACTUALIZAR TIPO DE VACUNA\n");
	printf("3- ACTUALIZAR VACUNA\n"); 
	printf("4- ACTUALIZAR PACIENTE\n"); 
	printf("5- ACTUALIZAR ENFERMERO/A\n"); 
	printf("6- ACTUALIZAR CENTRO DE ATENCION\n"); 
	printf("7- ACTUALIZAR TURNO\n");	
	printf("8- VOLVER AL MENU PRINCIPAL\n\n");
		
	printf("INDIQUE LA OPERACION A REALIZAR\n");
	scanf("%d",&opcion);
	return opcion;
}

static int menuListados()
{	
	int opcion;

	printf("BIENVENIDO AL SISTEMA DE REGISTRO DE VACUNACION COVID-19\n\n");
	printf("MENU DE LISTADOS\n");
	printf("1- LISTADO DE LOCALIDAD\n");
	printf("2- LISTADO DE TIPO DE VACUNA\n");
	printf("3- LISTADO DE VACUNA\n"); 
	printf("4- LISTADO DE PACIENTE\n"); 
	printf("5- LISTADO DE ENFERMERO/ENFERMERA \n"); 
	printf("6- LISTADO DE CENTRO DE ATENCION\n"); 
	printf("7- LISTADO DE TURNO DE VACUNA\n");
	printf("8- LISTADO DE PACIENTES VACUNADOS POR LOCALIDAD\n");
	printf("9- LISTADO DE TODOS LOS PACIENTES QUE SE VACUNARON CON SPUTINK (PRIMERA Y SEGUNDA DOSIS)\n");
	printf("10- LISTADO DE LOS ENFERMEROS QUE DIERON VACUNAS DE DETERMINADA FECHA\n");	
	printf("11- LISTADO DE LOS TURNOS PARA UN CENTRO DE ATENCION DADO UNA FECHA \n");
	printf("12- VOLVER AL MENU PRINCIPAL\n\n");
		
	printf("INDIQUE LA OPERACION A REALIZAR\n");
	scanf("%d",&opcion);
	return opcion;
}
