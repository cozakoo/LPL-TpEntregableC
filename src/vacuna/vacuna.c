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
#include "../tipo_vacuna/tipo_vacuna.h"
#include "vacuna.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo,cantidad;

bool confirmar;
bool registrar;
int bandera;
THIS(obj_Vacuna)// crea definicion de funcion this para este modulo. .. Macro en config.h

//----------------------------------------------------
int compara_CodigoDescV(const void *l1, const void * l2)
{
    obj_Vacuna *vac1 =  *((obj_Vacuna **)l1);
    obj_Vacuna *vac2 =  *((obj_Vacuna **)l2);
    
    return (vac1->getVacunaId(vac1) - vac2->getVacunaId(vac2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscV(const void *l1, const void * l2)
{
    obj_Vacuna *vac1 =  *((obj_Vacuna **)l1);
    obj_Vacuna *vac2 =  *((obj_Vacuna **)l2);
    
    return (vac2->getVacunaId(vac2)) - (vac1->getVacunaId(vac1))*-1;
}
//----------------------------------------------------
static void toString_VacunaImpl(void *self)
{
	obj_Vacuna *obj=this(self);
    obj_TipoVacuna *tv = obj->getTipoVacunaObj(obj) ;
    printf("id:%d - L:%s - FV:%s - CantDosis:%f - Marca: %s\n", obj->getVacunaId(obj),obj->getLote(obj),obj->getFechaVenc(obj),
    obj->getCantidad(obj),
    tv->getNombre(tv));
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getVacunaId_Impl(void *self)
{ return *( (int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static char *getLoteVacuna_Impl(void *self)
{ return  (char *) getValue(self,POS_LOTE_V); }
//----------------------------------------------------
static char *getFechaVencVacuna_Impl(void *self)
{ return  (char *) getValue(self,POS_FVENC_V); }
//----------------------------------------------------
static int getCodTipoVacuna_Impl(void *self)
{ return *( (int *) getValue(self,POS_COD_TIPO_V)); }
//----------------------------------------------------
static float getCantidadVacuna_Impl(void *self)
{ return *( (float *) getValue(self,POS_CANT_V)); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setVacunaId_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setLoteVacuna_Impl(void *self,char *lote_Vacuna)
{ setValue(self,POS_LOTE_V,lote_Vacuna); }
//----------------------------------------------------
static void setFechaVencVacuna_Impl(void *self,char *fechaVenc_Vacuna)
{ setValue(self,POS_FVENC_V,fechaVenc_Vacuna); }
//----------------------------------------------------
static void setCodTipoVacuna_Impl(void *self,int val)
{ setValue(self,POS_COD_TIPO_V,&val); }
//----------------------------------------------------
static void setCantidadVacuna_Impl(void *self,float val)
{ setValue(self,POS_CANT_V,&val); }
//----------------------------------------------------
//implementacion de relaciones
//----------------------------------------------------
obj_TipoVacuna *getTipoVacunaObj_Impl(void *self)
{    
    obj_Vacuna *obj = this(self);
    obj->tipo_vacuna = TipoVacuna_new();

    if(obj->tipo_vacuna->findbykey(obj->tipo_vacuna, obj->getCodTipoVacuna(obj))!= NOT_FOUND)
    	return obj->tipo_vacuna;
    	
    return NULL;
}
//----------------------------------------------------
static void destroyInternalVac_Impl(void *self)
{
    obj_Vacuna *obj = this(self);
    
	if(obj->tipo_vacuna!=NULL){
        destroyObj(obj->tipo_vacuna);
        obj->tipo_vacuna = NULL;
    }
}
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_Vacuna(void *self)
{
  obj_Vacuna *obj 	  = this(self);
  obj->ds  				  = &table_Vacuna;
  obj->constructor 		  = Vacuna_new;
  obj->sizeObj 			  = sizeof(obj_Vacuna*);
  obj->tipo_vacuna		  = NULL;
  //incializacion de la interfaz de la entidad  
  obj->toString    		  = toString_VacunaImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getVacunaId  	  = getVacunaId_Impl;
  obj->getLote  	  	  = getLoteVacuna_Impl;
  obj->getFechaVenc		  = getFechaVencVacuna_Impl;
  obj->getCodTipoVacuna   = getCodTipoVacuna_Impl;
  obj->getCantidad 		  = getCantidadVacuna_Impl;
  /// setters  
  obj->setVacunaId     	  = setVacunaId_Impl;
  obj->setLote			  = setLoteVacuna_Impl;
  obj->setFechaVenc		  = setFechaVencVacuna_Impl;
  obj->setCodTipoVacuna   = setCodTipoVacuna_Impl;
  obj->setCantidad		  = setCantidadVacuna_Impl;
   // implementar detroy internal para liberar recursos  
  obj->destroyInternal 	  = destroyInternalVac_Impl;
  //---- acceso a relaciones  
  obj->getTipoVacunaObj   = getTipoVacunaObj_Impl;  
  return obj;
}
//----------------------------------------------------
//constructor de Vacuna
obj_Vacuna *Vacuna_new()
{ return (obj_Vacuna *)init_obj(sizeof(obj_Vacuna), init_Vacuna); }
//----------------------------------------------------
void actualizarVacuna(){

	obj_Vacuna *vacuna;
	vacuna =  Vacuna_new();	
	
	printf("USTED ESTA ACTUALIZANDO UNA VACUNA\n\n");	
	listarVacunas(NULL,false);
	
	do{
		confirmar = false;	
		
		ingresarNumero("Ingrese el numero de codigo de la vacuna:\t",&cadena);
		codigo = atoi(cadena);
		
		if(vacuna->findbykey(vacuna,codigo) != NOT_FOUND)
		{
			ingresarCadena("Ingrese el nuevo lote:\t\t\t\t",&cadena);
			vacuna->setLote(vacuna,cadena);
	
			do {
				ingresarFecha("Ingrese el fecha de vencimiento (aaaa-mm-dd):\t",&cadena);
				bandera= esFechaMayor(&cadena);
			}while(bandera==0);

			vacuna->setFechaVenc(vacuna,cadena);
			
			ingresarNumero("Ingrese la nueva dosis:\t\t\t\t",&cadena);
			cantidad = atoi(cadena);
			
			vacuna->setCantidad(vacuna,cantidad);

			if(!vacuna->saveObj(vacuna))
				printf("Ocurrio un error al actualizar la vacuna:\n%s\n",getLastError());					
			else
				printf("\nVACUNA MODIFICADA\n");

			system("pause");
			
		}else{
			printf("\nERROR: no se pudo encontrar la vacuna\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(vacuna);
}
//-------------------------------------------------------------------------------
void registrarVacuna(){
	
	obj_Vacuna *vacuna;
	vacuna = Vacuna_new();	

	printf("USTED ESTA REGISTRANDO UNA VACUNA\n\n");	

	ingresarCadena("Ingrese el lote de la vacuna:\t",&cadena);
	vacuna->setLote(vacuna,cadena);
	
	do{
		ingresarFecha("Ingrese la fecha de vencimiento de la vacuna (aaaa-mm-dd):\t",&cadena);
		bandera= esFechaMayor(&cadena);
	}while(bandera==0);
	
	vacuna->setFechaVenc(vacuna,cadena);	
	
	obj_TipoVacuna *t_vacuna;
	t_vacuna = TipoVacuna_new();	
	
	printf("\n\tTIPOS DE VACUNAS\n");
	listarTipoVacunas(NULL,false);
	
	do{		
		confirmar = false;

		ingresarNumero("\nIngrese el codigo del tipo de vacuna:\t\t",&cadena);
		codigo = atoi(cadena);

		if(t_vacuna->findbykey(t_vacuna,codigo) != NOT_FOUND)
		{
			vacuna->setCodTipoVacuna(vacuna,codigo);			
			
			ingresarNumero("Ingrese la cantidad de dosis:\t\t",&cadena);			
			cantidad = atoi(cadena);
			vacuna->setCantidad(vacuna,cantidad);
			 
			if(!vacuna->saveObj(vacuna))
				printf("\nOcurrio un error al agregar la vacuna:\n%s\n",getLastError());
			else
				printf("\nTIPO DE VACUNA CREADA\n");
			
			system("pause");
		}else{
			printf("\nERROR: no se pudo encontrar el tipo de vacuna\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(vacuna);
	destroyObj(t_vacuna);
}
//-----------------------------------------------------------------------------------
//listar vacunas
void listarVacunas(char* archivo, bool descendente){
	
	printf("\n\t\t\t\tVACUNAS\n");
		
	FILE *salida = stdin;
	
	obj_Vacuna *vac, *aux;
	void *list,*itm;
	int i,size=0;
	
	vac = Vacuna_new();
	size = vac->findAll(vac,&list,NULL); 
	
	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_Vacuna*),compara_CodigoDescV);
	else
		qsort(list, size, sizeof(obj_Vacuna*),compara_CodigoAscV);
	
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_Vacuna **)list)[i];
		aux = (obj_Vacuna*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "Id Vacuna: %d  Lote:%s \n",
				aux->getVacunaId(aux),
				aux->getLote(aux)
			);
		else
			((Object *)itm)->toString(itm);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de vacunas exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(vac);
}
