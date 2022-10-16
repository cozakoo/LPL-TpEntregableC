#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../../includes/includelib.h"
#include "localidad.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n:\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo;

bool confirmar;	//permitira controlar si el usuario quiere reintentar operacion
bool registrar; //permitira controlar si el usuario quiere seguir registrando

THIS(obj_Localidad)// crea definicion de funcion this para este modulo. .. Macro en config.h
//----------------------------------------------------
static void toString_LocalidadImpl(void *self)
{
     obj_Localidad *obj=this(self);
     printf("Codigo Postal: %d  Nombre:%s \n",
	 obj->getId(obj),
	 obj->getNombre(obj)
	 );
}

//----------------------------------------------------
int compara_CodigoDesc(const void *l1, const void * l2)
{
    obj_Localidad *loc1 =  *((obj_Localidad **)l1);
    obj_Localidad *loc2 =  *((obj_Localidad **)l2);
    
    return (loc1->getId(loc1) - loc2->getId(loc2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAsc(const void *l1, const void * l2)
{
    obj_Localidad *loc1 =  *((obj_Localidad **)l1);
    obj_Localidad *loc2 =  *((obj_Localidad **)l2);
    
    return (loc2->getId(loc2)) - (loc1->getId(loc1))*-1;
}
//--------------------------------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getIdLocalidad_Impl(void *self)
{
  return *( (int *) getValue(self,POS_ID));
}
//----------------------------------------------------
static char *getNombreLocalidad_Impl(void *self)
{
  return  (char *) getValue(self,POS_DESCRIPCION);
}
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setIdLocalidad_Impl(void *self,int val)
{ 
	setValue(self,POS_ID,&val);
}
//----------------------------------------------------
static void setNombreLocalidad_Impl(void *self,char *descripcion_Localidad)
{ 
	setValue(self,POS_DESCRIPCION,descripcion_Localidad);
}
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_Localidad(void *self)
{
  obj_Localidad *obj 	  = this(self);
  obj->ds  				  = &table_Localidad;
  obj->constructor 		  = Localidad_new;
  obj->sizeObj 			  = sizeof(obj_Localidad*);
  //incializacion de la interfaz de la entidad  
  obj->toString    		  = toString_LocalidadImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getId  	  		  = getIdLocalidad_Impl;
  obj->getNombre		  = getNombreLocalidad_Impl;  
  /// setters  
  obj->setId     		  = setIdLocalidad_Impl;
  obj->setNombre   		  = setNombreLocalidad_Impl;  
  return obj;
}
//----------------------------------------------------
//constructor de Localidad
obj_Localidad *Localidad_new()
{
  return (obj_Localidad *)init_obj(sizeof(obj_Localidad), init_Localidad);
}
//----------------------------------------------------
void actualizarLocalidad(){

	obj_Localidad *loc;
	loc =  Localidad_new();	

	printf("USTED ESTA ACTUALIZANDO UNA LOCALIDAD\n\n");

	listarLocalidades(NULL,false);

	do{
		confirmar = false;
				
		ingresarNumero("Ingrese el codigo postal:\t", &cadena);
		codigo = atoi(cadena);
		
		if(loc->findbykey(loc,codigo) != NOT_FOUND)
		{
			ingresarCadena("Ingrese el nuevo nombre de la localidad:\t", &cadena);
			loc->setNombre(loc,cadena);

			if(!loc->saveObj(loc))
				printf("Ocurrio un error al agregar Localidad:\n%s\n",getLastError());
			else
				printf("\nLOCALIDAD MODIFICADA\n");
			
			system("pause");
		}else{
			printf("\nERROR: no se pudo encontrar la localidad\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);	
	destroyObj(loc);
}

//---------------------------------------------------------------------------------
void registrarLocalidad(){

	obj_Localidad *loc;
	loc = Localidad_new();
		
	printf("USTED ESTA REGISTRANDO UNA LOCALIDAD\n\n");	
		
	ingresarNumero("Ingrese el codigo postal de la localidad:\t", &cadena);	
	codigo = atoi(cadena);
	loc->setId(loc,codigo);
	
	ingresarCadena("Ingrese el nombre:\t", &cadena);
	loc->setNombre(loc,cadena);
	
	if(!loc->saveObj(loc))
		printf("\nOcurrio un error al agregar la localidad:\n%s\n",getLastError());
	else
		printf("\nLOCALIDAD CREADA\n\n");
	
	system("pause");
	destroyObj(loc);
}

//----------------------------------------------------------------------------------------
//listado de localidades
void listarLocalidades(char* archivo, bool descendente){
	
	printf("\n\tLOCALIDADES\n");
		
	FILE *salida = stdin;
	
	obj_Localidad *loc, *aux;
	void *list,*itm;
	int i,size=0;
	
	loc = Localidad_new();
	size = loc->findAll(loc,&list,NULL); 
	
	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_Localidad*),compara_CodigoDesc);
	else
		qsort(list, size, sizeof(obj_Localidad*),compara_CodigoAsc);
	
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_Localidad **)list)[i];
		aux = (obj_Localidad*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "Codigo Postal: %d  Nombre:%s \n",
				aux->getId(aux),
				aux->getNombre(aux)
			);
		else
			((Object *)itm)->toString(itm);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de localidades exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(loc);
}

char *obtenerNombreLocalidad(int codLocalidad){
	
	obj_Localidad *loc;
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
	
	loc = Localidad_new();
	
	//listamos por el numero de documento
	sprintf(criterio, "cod_postal=%d",codLocalidad);
	size = loc->findAll(loc,&list,criterio);
	
	loc = ((obj_Localidad **)list)[size-1];
	char nombre[10];
	
	strcpy(nombre,loc->getNombre(loc));
	
	return ; nombre;
}





