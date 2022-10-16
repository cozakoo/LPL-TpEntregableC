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
#include "tipo_vacuna.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo,cantidad;

bool confirmar;
bool registrar;
int bandera;

THIS(obj_TipoVacuna)// crea definicion de funcion this para este modulo. .. Macro en config.h
//----------------------------------------------------
int compara_CodigoDescTV(const void *l1, const void * l2)
{
    obj_TipoVacuna *loc1 =  *((obj_TipoVacuna **)l1);
    obj_TipoVacuna *loc2 =  *((obj_TipoVacuna **)l2);
    
    return (loc1->getId(loc1) - loc2->getId(loc2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscTV(const void *l1, const void * l2)
{
   obj_TipoVacuna *loc1 =  *((obj_TipoVacuna **)l1);
   obj_TipoVacuna *loc2 =  *((obj_TipoVacuna **)l2);
    
    return (loc2->getId(loc2)) - (loc1->getId(loc1))*-1;
}
//----------------------------------------------------

static void toString_TipoVacunaImpl(void *self)
{
     obj_TipoVacuna *obj=this(self);
     printf("TipoVacuna_id: %d  TipoVacuna:%s \n",
	 obj->getId(obj),
	 obj->getNombre(obj)
	 );
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getIdTipoVacuna_Impl(void *self)
{ return *( (int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static char *getNombreTipoVacuna_Impl(void *self)
{ return  (char *) getValue(self,POS_NOMBRE_TV); }
//----------------------------------------------------
static int getCantidadDosis_Impl(void *self)
{ return *( (int *) getValue(self,POS_CANT_DOSIS_TV)); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setIdTipoVacuna_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setNombreTipoVacuna_Impl(void *self,char *nombre_TipoVacuna)
{ setValue(self,POS_NOMBRE_TV,nombre_TipoVacuna); }
//----------------------------------------------------
static void setCantidadDosis_Impl(void *self,int val)
{ setValue(self,POS_CANT_DOSIS_TV,&val); }
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_TipoVacuna(void *self)
{
  obj_TipoVacuna *obj 	  = this(self);
  obj->ds  				  = &table_TipoVacuna;
  obj->constructor 		  = TipoVacuna_new;
  obj->sizeObj 			  = sizeof(obj_TipoVacuna*);
  //incializacion de la interfaz de la entidad  
  obj->toString    		  = toString_TipoVacunaImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getId    		  = getIdTipoVacuna_Impl;
  obj->getNombre		  = getNombreTipoVacuna_Impl;  
  obj->getCantidadDosis   = getCantidadDosis_Impl;
  /// setters  
  obj->setId    		  = setIdTipoVacuna_Impl;
  obj->setNombre   		  = setNombreTipoVacuna_Impl;  
  obj->setCantidadDosis   = setCantidadDosis_Impl;
  return obj;
}
//----------------------------------------------------
//constructor de TipoVacuna
obj_TipoVacuna *TipoVacuna_new()
{
  return (obj_TipoVacuna *)init_obj(sizeof(obj_TipoVacuna), init_TipoVacuna);
}
//----------------------------------------------------
void actualizarTipoVacuna(){
		
	obj_TipoVacuna *t_vacuna;
	t_vacuna = TipoVacuna_new();

	printf("USTED ESTA ACTUALIZANDO UN TIPO DE VACUNA\n\n");	
	
	listarTipoVacunas(NULL,false);
	
	do{
		confirmar = false;
		
		ingresarNumero("Ingrese el codigo del tipo de vacuna:\t", &cadena);
		codigo = atoi(cadena);
				
		if(t_vacuna->findbykey(t_vacuna,codigo) != NOT_FOUND)
		{	
			ingresarCadena("Ingrese el nuevo nombre de la vacuna:\t", &cadena);
			t_vacuna->setNombre(t_vacuna,cadena);
			
			ingresarNumero("Ingrese la nueva cantidad de dosis:\t\t",&cadena);
			int dosis = atoi(cadena);
			t_vacuna->setCantidadDosis(t_vacuna,dosis);
	
			if(!t_vacuna->saveObj(t_vacuna))
				printf("Ocurrio un error al agregar el tipo de vacuna:\n%s\n",getLastError());
			else
				printf("\nTIPO DE VACUNA MODIFICADA\n");
			
			system("pause");
		}else{
			printf("\nERROR: no se pudo encontrar el tipo de vacuna\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	
	destroyObj(t_vacuna);	
}
//---------------------------------------------------------------------------
void registrarTipoVacuna(){
	
	obj_TipoVacuna *t_vacuna;
	t_vacuna = TipoVacuna_new();	
	
	printf("USTED ESTA REGISTRANDO UN TIPO DE VACUNA\n\n");	
	
	ingresarCadena("Ingrese el nombre:\t\t",&cadena);
	t_vacuna->setNombre(t_vacuna,cadena);
	
	ingresarNumero("Ingrese la cantidad de dosis:\t", &cadena);
	int dosis = atoi(cadena);
	
	t_vacuna->setCantidadDosis(t_vacuna,dosis); //seteamos la dosis
	
	if(!t_vacuna->saveObj(t_vacuna))
		printf("\nOcurrio un error al agregar el tipo de vacuna:\n%s\n",getLastError());
	else
		printf("\nTIPO DE VACUNA CREADA\n\n");
	
	system("pause");
	destroyObj(t_vacuna);
}
//-----------------------------------------------------------------------------------
//listar tipos de vacuna
void listarTipoVacunas(char* archivo, bool descendente){

	printf("\n\t\tTIPOS DE VACUNAS\n");

	FILE *salida = stdin;
	
	obj_TipoVacuna *tv, *aux;
	void *list,*itm;
	int i,size=0;
	tv = TipoVacuna_new();
	
	size = tv->findAll(tv,&list,NULL); 

	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_TipoVacuna*),compara_CodigoDescTV);
	else
		qsort(list, size, sizeof(obj_TipoVacuna*),compara_CodigoAscTV);
		
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_TipoVacuna **)list)[i];
		aux = (obj_TipoVacuna*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "Id centro: %d\t Nombre: %s\t Numero de Dosis: %d\n",
				aux->getId(aux),
				aux->getNombre(aux),
				aux->getCantidadDosis(aux)
			);
		else
			((obj_TipoVacuna *)itm)->toString(itm);
	}
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de tipos de vacunas exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(tv);
}
