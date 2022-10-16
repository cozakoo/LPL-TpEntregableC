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
#include "../localidad/localidad.h"
#include "centro_atencion.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo;
int bandera;

bool confirmar;	//permitira controlar si el usuario quiere reintentar operacion
bool registrar; //permitira controlar si el usuario quiere seguir registrando

THIS(obj_CentroAtencion)// crea definicion de funcion this para este modulo. .. Macro en config.h
//----------------------------------------------------
int compara_CodigoDescC(const void *l1, const void * l2)
{
    obj_CentroAtencion *loc1 =  *((obj_CentroAtencion **)l1);
    obj_CentroAtencion *loc2 =  *((obj_CentroAtencion **)l2);
    
    return (loc1->getId(loc1) - loc2->getId(loc2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscC(const void *l1, const void * l2)
{
    obj_CentroAtencion *loc1 =  *((obj_CentroAtencion **)l1);
    obj_CentroAtencion *loc2 =  *((obj_CentroAtencion **)l2);
    
    return (loc2->getId(loc2)) - (loc1->getId(loc1))*-1;
}
//--------------------------------------------------------------------------
static void toString_CentroAtencionImpl(void *self)
{
	obj_CentroAtencion *obj=this(self);     
    obj_Localidad *loc;
    loc = obj->getLocalidadObj(obj);
    printf("id: %d\tCons.:%s \tDom.: %s\tTel.: %s - Loc:%s\n",
	    obj->getId(obj),
	    obj->getNombre(obj),
	    obj->getDomicilio(obj),
	    obj->getTelefono(obj),
	    loc->getNombre(loc)
	    );
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getIdCentroAtencion_Impl(void *self)
{ return *( (int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static char *getNombreCentroAtencion_Impl(void *self)
{ return  (char *) getValue(self,POS_NOMBRE_CENTRO); }
//----------------------------------------------------
static char *getDomicilioCentroAtencion_Impl(void *self)
{ return  (char *) getValue(self,POS_DOMICILIO_CENTRO); }
//----------------------------------------------------
static char *getTelefonoCentroAtencion_Impl(void *self)
{ return  (char *) getValue(self,POS_TELEFONO_CENTRO); }
//----------------------------------------------------
static int getCodPostalCentroAtencion_Impl(void *self)
{ return *( (int *) getValue(self,POS_CODPOSTAL_CENTRO)); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setIdCentroAtencion_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setNombreCentroAtencion_Impl(void *self,char *nombre)
{ setValue(self,POS_NOMBRE_CENTRO,nombre); }
//----------------------------------------------------
static void setDomicilioCentroAtencion_Impl(void *self,char *domicilio)
{ setValue(self,POS_DOMICILIO_CENTRO,domicilio); }
//----------------------------------------------------
static void setTelefonoCentroAtencion_Impl(void *self,char *telefono)
{ setValue(self,POS_TELEFONO_CENTRO,telefono); }
//----------------------------------------------------
static void setCodPostalCentroAtencion_Impl(void *self,int val)
{ setValue(self,POS_CODPOSTAL_CENTRO,&val); }
//----------------------------------------------------
static void destroyInternalCentroAtencion_Impl(void *self)
{
    obj_CentroAtencion *obj = this(self);    
    if(obj->localidad!=NULL)
      destroyObj(obj->localidad);
      
    obj -> localidad = NULL;
}
//----------------------------------------------------
//implementacion de relaciones
//----------------------------------------------------
obj_Localidad *getLocalidadCentroAtencionObj_Impl(void *self)
{
    obj_CentroAtencion *obj = this(self);    
    obj->localidad = Localidad_new();
    if(obj->localidad->findbykey(obj->localidad,obj->getCodPostal(obj))!=NOT_FOUND)
      {
          return obj->localidad;
      }
    return obj->localidad = NULL;
}
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_CentroAtencion(void *self)
{
  obj_CentroAtencion *obj  = this(self);  
  //incializacion de la interfaz de la entidad  
  obj->ds               	= &table_CentroAtencion;
  obj->constructor      	= CentroAtencion_new;
  obj->sizeObj          	= sizeof(obj_CentroAtencion*);
  obj->localidad 	    	= NULL;  
  obj->toString         	= toString_CentroAtencionImpl;    
  // Inicializar handlers de getters y setters
  /// getters
  obj->getId  				= getIdCentroAtencion_Impl;
  obj->getNombre  		   	= getNombreCentroAtencion_Impl;
  obj->getDomicilio     	= getDomicilioCentroAtencion_Impl;
  obj->getTelefono      	= getTelefonoCentroAtencion_Impl;
  obj->getCodPostal			= getCodPostalCentroAtencion_Impl;
  /// setters  
  obj->setId 				= setIdCentroAtencion_Impl;
  obj->setDenominacion  	= setNombreCentroAtencion_Impl;
  obj->setDomicilio     	= setDomicilioCentroAtencion_Impl;
  obj->setTelefono      	= setTelefonoCentroAtencion_Impl;
  obj->setCodPostal			= setCodPostalCentroAtencion_Impl;
  // implementar detroy internal para liberar recursos  
  obj->destroyInternal 		= destroyInternalCentroAtencion_Impl;
  //---- acceso a relaciones  
  obj->getLocalidadObj 		= getLocalidadCentroAtencionObj_Impl;    
  return obj;
}
//----------------------------------------------------
//constructor de CentroAtencion
obj_CentroAtencion *CentroAtencion_new()
{
  return (obj_CentroAtencion *)init_obj(sizeof(obj_CentroAtencion), init_CentroAtencion);
}
//----------------------------------------------------
void actualizarCentroAtencion(){
	
	obj_CentroAtencion *centroAtencion;
	centroAtencion = CentroAtencion_new();

	printf("USTED ESTA ACTUALIZANDO UN CENTRO DE ATENCION\n\n");		
	listarCentrosAtencion(NULL,false);

	do{
		confirmar = false;

		ingresarNumero("Ingrese el codigo del Centro que desea actualizar:\t\t\t",&cadena);
		codigo = atoi(cadena);

		if(centroAtencion->findbykey(centroAtencion,codigo) != NOT_FOUND)
		{
			ingresarCadena("Ingrese el nombre (nuevo):\t",&cadena);
			centroAtencion->setDenominacion(centroAtencion,cadena);

			ingresarCadena("Ingrese el nuevo domicilio (nuevo) del Centro de Atencion:\t",&cadena);
			centroAtencion->setDomicilio(centroAtencion,cadena);
			
			ingresarNumero("Ingrese el numero (nuevo) de telefono:\t",&cadena);
			centroAtencion->setTelefono(centroAtencion,cadena);
			
			obj_Localidad *localidad;
			localidad = Localidad_new();
			
			listarLocalidades(NULL,false);
			
			bool reintentar; 
			
			do {
				reintentar = false;
				
				ingresarNumero("Ingrese el codigo postal:\t\t\t",&cadena);	
				codigo = atoi(cadena);
		
				if(localidad->findbykey(localidad,codigo) != NOT_FOUND){
					
					centroAtencion->setCodPostal(centroAtencion,codigo);
		  
					if(!centroAtencion->saveObj(centroAtencion))
						printf("\nOcurrio un error al actualizar el Centro de Atencion:\n%s\n",getLastError());
					else
						printf("\nCENTRO DE ATENCION ACTUALIZADO\n");
					
					system("pause");
				}else{
					printf("\nERROR: no se pudo encontrar la localidad\n\n");
					reintentar = continuar(MSJ_REINTENTAR);
				}
			}while(reintentar);
			destroyObj(localidad);
		}else{
			printf("\nERROR: no se pudo encontrar el Centro de Atencion\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(centroAtencion);
}
//-------------------------------------------------------------------------------
void registrarCentroAtencion(){
	
	obj_CentroAtencion *centroAtencion;
	centroAtencion = CentroAtencion_new();
		
	printf("USTED ESTA REGISTRANDO UN CENTRO DE ATENCION\n\n");	
	
	ingresarCadena("Ingrese el nombre:\t",&cadena);
	centroAtencion->setDenominacion(centroAtencion,cadena);
	
	ingresarCadena("Ingrese el domicilio:\t",&cadena);
	centroAtencion->setDomicilio(centroAtencion,cadena);
	
	ingresarNumero("Ingrese el telefono:\t",&cadena);
	centroAtencion->setTelefono(centroAtencion,cadena);
	
	obj_Localidad *localidad;
	localidad = Localidad_new();

	listarLocalidades(NULL,false);
	
	do{
		confirmar = false;
		
		ingresarNumero("Ingrese el codigo postal:\t\t\t",&cadena);
		codigo = atoi(cadena);
	
		if(localidad->findbykey(localidad,codigo) != NOT_FOUND)
		{
			centroAtencion->setCodPostal(centroAtencion,codigo);		
		
			if(!centroAtencion->saveObj(centroAtencion))
				printf("\nOcurrio un error al agregar el Centro de Atencion:\n%s\n",getLastError());
			else
				printf("\nCENTRO DE ATENCION CREADO\n");
			
			system("pause");
		}else{
			printf("\nERROR: no se pudo encontrar la localidad\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(localidad);
	destroyObj(centroAtencion);
}
//------------------------------------------------------------------------------------------
void listarCentrosPorCriterio(int numero){
	
	obj_CentroAtencion *cen;
	cen = CentroAtencion_new();
	
	char criterio[MAX_SQL];
	void *list, *itm;
	int i, size;

	sprintf(criterio, "cod_postal=%d",numero);
	size = cen->findAll(cen,&list,criterio);	
	
	for(i=0; i<size; ++i){
		itm = ((Object **)list)[i];
		((Object *)itm)->toString(itm);
	}	
}

//listado centros de atención
void listarCentrosAtencion(char* archivo, bool descendente){
	
	printf("\n\t\t\t\tCENTROS DE ATENCION\n");
	
	FILE *salida = stdin;
	
	obj_CentroAtencion *cen, *aux;
	void *list,*itm;
	int i,size=0;
	
	cen = CentroAtencion_new();
	size = cen->findAll(cen,&list,NULL); 
	
	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_CentroAtencion*),compara_CodigoDescC);
	else
		qsort(list, size, sizeof(obj_CentroAtencion*),compara_CodigoAscC);
		
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_CentroAtencion **)list)[i];
		aux = (obj_CentroAtencion*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "Id centro: %d  Nombre centro:%s Domicilio :%s\n",
				aux->getId(aux),
				aux->getNombre(aux),
				aux->getDomicilio(aux)
			);
		else
			((Object *)itm)->toString(itm);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de centros de atención exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(cen);
}




char *obtenerNombreCentro(int codCentro){

	obj_CentroAtencion *cen;
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
	
	cen = CentroAtencion_new();
	
	//listamos por el numero de documento
	sprintf(criterio, "codigo=%d",codCentro);
	size = cen->findAll(cen,&list,criterio);
	
	cen = ((obj_CentroAtencion **)list)[size-1];
	char nombre[10];
	
	strcpy(nombre,cen->getNombre(cen));
	
	return ; nombre;
}
