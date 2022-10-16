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
#include "enfermero.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo;
int bandera;

bool confirmar;	//permitira controlar si el usuario quiere reintentar operacion
bool registrar; //permitira controlar si el usuario quiere seguir registrando

THIS(obj_Enfermero)// crea definicion de funcion this para este modulo. .. Macro en config.h
//----------------------------------------------------
int compara_CodigoDescE(const void *l1, const void * l2)
{
    obj_Enfermero *loc1 =  *((obj_Enfermero **)l1);
    obj_Enfermero *loc2 =  *((obj_Enfermero **)l2);
    
    return (loc1->getNro_Documento(loc1) - loc2->getNro_Documento(loc2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscE(const void *l1, const void * l2)
{
    obj_Enfermero *loc1 =  *((obj_Enfermero **)l1);
    obj_Enfermero *loc2 =  *((obj_Enfermero **)l2);
    
    return (loc2->getNro_Documento(loc2)) - (loc1->getNro_Documento(loc1))*-1;
}
//--------------------------------------------------------------------------
static void toString_EnfermeroImpl(void *self)
{
     obj_Enfermero *obj=this(self);     
     obj_Localidad *loc = obj->getLocalidadObj(obj);
     printf("Enfermero MAT: %s - DNI: %d  - ApyNom:%s, %s\n",	 
	 obj->getMatricula(obj),
	 obj->getNro_Documento(obj),
	 obj->getApellido(obj),
	 obj->getNombres(obj)// de que localidad es? // agregar...	 
	 );
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getDniEnfermero_Impl(void *self)
{ return *( (int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static char *getNombresEnfermero_Impl(void *self)
{ return  (char *) getValue(self,POS_NOMBRES_PROF); }
//----------------------------------------------------
static char *getApellidoEnfermero_Impl(void *self)
{ return  (char *) getValue(self,POS_APELLIDO_PROF); }
//----------------------------------------------------
static char *getDomicilioEnfermero_Impl(void *self)
{ return  (char *) getValue(self,POS_DOMICILIO_PROF); }
//----------------------------------------------------
static char *getTelefonoEnfermero_Impl(void *self)
{ return  (char *) getValue(self,POS_TELEFONO_PROF); }
//----------------------------------------------------
static char *getMatriculaEnfermero_Impl(void *self)
{ return  (char *) getValue(self,POS_MATRICULA_PROF); }
//----------------------------------------------------
static int getCodPostalEnfermero_Impl(void *self)
{ return *( (int *) getValue(self,POS_COD_LOC_PROF)); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setDniEnfermero_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setApellidoEnfermero_Impl(void *self,char *apellido_Enfermero)
{ setValue(self,POS_APELLIDO_PROF,apellido_Enfermero); }
//----------------------------------------------------
static void setNombresEnfermero_Impl(void *self,char *nombres_Enfermero)
{ setValue(self,POS_NOMBRES_PROF,nombres_Enfermero); }
//----------------------------------------------------
static void setDomicilioEnfermero_Impl(void *self,char *domicilio_Enfermero)
{ setValue(self,POS_DOMICILIO_PROF,domicilio_Enfermero); }
//----------------------------------------------------
static void setTelefonoEnfermero_Impl(void *self,char *telefono_Enfermero)
{ setValue(self,POS_TELEFONO_PROF,telefono_Enfermero); }
//----------------------------------------------------
static void setMatriculaEnfermero_Impl(void *self,char *mat_Enfermero)
{ setValue(self,POS_MATRICULA_PROF,mat_Enfermero); }
//----------------------------------------------------
static void setCodPostalEnfermero_Impl(void *self,int val)
{ setValue(self,POS_COD_LOC_PROF,&val); }
//----------------------------------------------------
static void destroyInternalProf_Impl(void *self)
{
	//implementado
    obj_Enfermero *obj = this(self);
    if(obj->localidad != NULL)
    {
        destroyObj(obj->localidad);
        obj->localidad = NULL;
    }
}
//----------------------------------------------------
//implementacion de relaciones
//----------------------------------------------------
obj_Localidad *getLocalidadProfObj_Impl(void *self)
{
	//implementado
    obj_Enfermero *obj = this(self);
    obj->localidad = Localidad_new();
    if(obj->localidad->findbykey(obj->localidad, obj->getCodPostal(obj))!= NOT_FOUND)
    {
        return obj->localidad;
    }
    return NULL;
}
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_Enfermero(void *self)
{
  obj_Enfermero *obj    = this(self);
  obj->ds  			      = &table_Enfermero;
  obj->constructor 	      = Enfermero_new;
  obj->sizeObj 		      = sizeof(obj_Enfermero*);
  //incializacion de la interfaz de la entidad
  obj->toString    		  = toString_EnfermeroImpl;
  obj->localidad 	      = NULL;
  // Inicializar handlers de getters y setters
  /// getters    
  obj->getNro_Documento   = getDniEnfermero_Impl;
  obj->getApellido        = getApellidoEnfermero_Impl;
  obj->getNombres         = getNombresEnfermero_Impl;
  obj->getDomicilio       = getDomicilioEnfermero_Impl;
  obj->getTelefono        = getTelefonoEnfermero_Impl;
  obj->getMatricula       = getMatriculaEnfermero_Impl;
  obj->getCodPostal    	  = getCodPostalEnfermero_Impl;  
  /// setters    
  obj->setNro_Documento   = setDniEnfermero_Impl;
  obj->setApellido        = setApellidoEnfermero_Impl;
  obj->setNombres         = setNombresEnfermero_Impl;
  obj->setDomicilio       = setDomicilioEnfermero_Impl;
  obj->setTelefono        = setTelefonoEnfermero_Impl;
  obj->setMatricula       = setMatriculaEnfermero_Impl;
  obj->setCodPostal       = setCodPostalEnfermero_Impl;  
  // implementar detroy internal para liberar recursos  
  obj->destroyInternal 	  = destroyInternalProf_Impl;
  //---- acceso a relaciones  
  obj->getLocalidadObj 	  = getLocalidadProfObj_Impl;
  return obj;
}
//----------------------------------------------------
//constructor de Enfermero
obj_Enfermero *Enfermero_new()
{
  return (obj_Enfermero *)init_obj(sizeof(obj_Enfermero), init_Enfermero);
}
//----------------------------------------------------
void actualizarEnfermero()
{		
	obj_Enfermero *enfermero;
	enfermero = Enfermero_new();

	printf("USTED ESTA ACTUALIZANDO UN ENFERMERO\n\n");
	listarEnfermeros(NULL,false);
	
	do{
		confirmar = false; 

		ingresarNumero("Ingrese el Numero de Documento del enfermero/a:\t", &cadena);
		codigo = atoi(cadena);
		
		if(enfermero->findbykey(enfermero,codigo) != NOT_FOUND)
		{
			ingresarCadena("Ingrese el/los apellido/s del enfermero/a:\t\t\t",&cadena);
			enfermero->setApellido(enfermero,cadena);
			
			ingresarCadena("Ingrese el/los nombres del enfermero/a:\t\t\t",&cadena);
			enfermero->setNombres(enfermero,cadena);

			ingresarNumero("Ingrese el numero de telefono:\t\t\t\t",&cadena);
			enfermero->setTelefono(enfermero,cadena);
			
			ingresarCadena("Ingrese el domicilio del enfermero/a:\t\t\t",&cadena);
			enfermero->setDomicilio(enfermero,cadena);				
	
			obj_Localidad *localidad;
			localidad = Localidad_new();
			
			listarLocalidades(NULL,false);
			
			bool reintentar;
					
			do{
				reintentar = false;
			
				ingresarNumero("Ingrese el codigo postal del enfermero/a:\t\t\t",&cadena);
				codigo = atoi(cadena);
			
				if(localidad->findbykey(localidad,codigo) != NOT_FOUND)
				{
					enfermero->setCodPostal(enfermero,codigo);
					
					ingresarNumero("Ingrese el numero de matricula:\t",&cadena);
					enfermero->setMatricula(enfermero,cadena);
											
					if(!enfermero->saveObj(enfermero))
						printf("\nOcurrio un error al agregar al enfermero:\n%s\n",getLastError());
					else
						printf("\nPACIENTE ACTUALIZADO\n");
					
					system("pause");
					destroyObj(localidad);
				}else{
					printf("\nERROR: no se pudo encontrar la localidad\n\n");
					reintentar = continuar(MSJ_REINTENTAR);
				}
			}while(reintentar);
		}else{
			printf("\nERROR: enfermero no encontrado\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(enfermero);
}
//----------------------------------------------------
void registrarEnfermero(){
	
	obj_Enfermero *enfermero;
	enfermero = Enfermero_new();
	
	printf("USTED ESTA REGISTRANDO UN/A ENFERMERO/A\n\n");	
	
	ingresarNumero("Ingrese el Numero de Documento del enfermero/a:\t\t",&cadena);
	codigo = atoi(cadena);		
	enfermero->setNro_Documento(enfermero,codigo); //seteamos el id 
	
	ingresarCadena("Ingrese el/los apellido/s del enfermero/a:\t",&cadena);
	enfermero->setApellido(enfermero,cadena);

	ingresarCadena("Ingrese el/los nombres del enfermero/a:\t",&cadena);
	enfermero->setNombres(enfermero,cadena);
	
	ingresarNumero("Ingrese el numero de telefono:\t\t",&cadena);
	enfermero->setTelefono(enfermero,cadena);
	
	ingresarCadena("Ingrese el domicilio del enfermero/enfermera:\t",&cadena);
	enfermero->setDomicilio(enfermero,cadena);

	obj_Localidad *localidad;
	localidad = Localidad_new();

	listarLocalidades(NULL,false);
	
	do{
		confirmar = false;
		
		ingresarNumero("Ingrese el codigo postal del enfermero:\t\t",&cadena);		
		codigo = atoi(cadena);

		if(localidad->findbykey(localidad,codigo) != NOT_FOUND)
		{
			enfermero->setCodPostal(enfermero,codigo);
		
			ingresarNumero("Ingrese el numero de matricula:\t",&cadena);
			enfermero->setMatricula(enfermero,cadena);

			if(!enfermero->saveObj(enfermero))
				printf("\nOcurrio un error al agregar al enfermero/a:\n%s\n",getLastError());
			else
				printf("\nENFERMERO CREADO\n");	
			
			system("pause;");
			
		}else{
			printf("\nERROR: no se pudo encontrar la Localidad\n\n");
			confirmar = continuar(MSJ_REINTENTAR);			
		}
	}while(confirmar);

	destroyObj(localidad);
	destroyObj(enfermero);	
}
//-----------------------------------------------------------------------------------
//listar enfermeros
void listarEnfermeros(char* archivo, bool descendente){

	printf("\n\t\tENFERMEROS\n");

	FILE *salida = stdin;
	
	obj_Enfermero *enf, *aux;
	void *list,*itm;
	int i,size=0;
	
	enf = Enfermero_new(); 
	size = enf->findAll(enf,&list,NULL); 

	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_Enfermero*),compara_CodigoDescE);
	else
		qsort(list, size, sizeof(obj_Enfermero*),compara_CodigoAscE);
		
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_Enfermero **)list)[i];
		aux = (obj_Enfermero*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "DNI enfermero: %d  Nombre:%s Apellido:%s\n",
				aux->getNro_Documento(aux),
				aux->getNombres(aux),
				aux->getApellido(aux)
			);
		else
			((Object *)itm)->toString(itm);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de turnos exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(enf);
}

void listarEnfermeroLocalidad(int codLocalidad){
	
	obj_Enfermero *enf;
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
	
	enf = Enfermero_new();
	
	sprintf(criterio, "cod_postal=%d",codLocalidad);
	size = enf->findAll(enf,&list,criterio);
	
	printf("\n\n");
	for (i=0; i<size; ++i){
		enf = ((obj_Localidad **)list)[i];	
		enf->toString(enf);
	}
	printf("\n\n");
}

void obtenerInfoEnfermero(int dni){
	
	obj_Enfermero *enf;
	enf = Enfermero_new();
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
		
	//listamos por el numero de documento
	sprintf(criterio, "nro_documento=%d",dni);
	size = enf->findAll(enf,&list,criterio);
	
	enf = ((obj_Enfermero **)list)[size-1];
	enf->toString(enf);		
}





