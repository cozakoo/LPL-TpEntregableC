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
#include "paciente.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo;
int bandera;

bool confirmar;	//permitira controlar si el usuario quiere reintentar operacion
bool registrar; //permitira controlar si el usuario quiere seguir registrando

//----------------------------------------------------
int compara_CodigoDescP(const void *l1, const void * l2){
    obj_Paciente *pac1 =  *((obj_Paciente **)l1);
    obj_Paciente *pac2 =  *((obj_Paciente **)l2);
    
    return (pac1->getDni(pac1) - pac2->getDni(pac2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscP(const void *l1, const void * l2)
{
    obj_Paciente *pac1 =  *((obj_Paciente **)l1);
    obj_Paciente *pac2 =  *((obj_Paciente **)l2);
    
    return (pac2->getDni(pac2)) - (pac1->getDni(pac1))*-1;
}
//--------------------------------------------------------------------------

THIS(obj_Paciente)// crea definicion de funcion this para este modulo. .. Macro en config.h
//----------------------------------------------------
static void toString_PacienteImpl(void *self)
{
    obj_Paciente *obj=this(self);     
    obj_Localidad *loc = obj->getLocalidadObj(obj);
    
	printf("DNI: %d  - ApyNom:%s, %s - FNac:%s \tLocalidad:%s\n",
	obj->getDni(obj),
	obj->getApellido(obj),
	obj->getNombres(obj),
	obj->getFechaNac(obj),	 
	loc->getNombre(loc)
	);
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getDniPaciente_Impl(void *self)
{ return *((int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static char *getNombresPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_NOMBRES_PAC); }
//----------------------------------------------------
static char *getApellidoPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_APELLIDO_PAC); }
//----------------------------------------------------
static char *getFechaNacPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_FNAC_PAC); }
//----------------------------------------------------
static char *getDomicilioPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_DOMICILIO_PAC); }
//----------------------------------------------------
static char *getTelefonoPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_TELEFONO_PAC); }
//----------------------------------------------------
static int getCodPostalPaciente_Impl(void *self)
{ return *( (int *) getValue(self,POS_COD_POSTAL_PAC)); }
//----------------------------------------------------
static int getDeRiesgoPaciente_Impl(void *self)
{ return *( (int *) getValue(self,POS_DE_RIESGO_PAC)); }
//----------------------------------------------------
static int getVacunadoPaciente_Impl(void *self)
{ return *( (int *) getValue(self,POS_VACUNADO_PAC)); }
//----------------------------------------------------
static char *getObservacionesPaciente_Impl(void *self)
{ return  (char *) getValue(self,POS_OBS_PAC); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setDniPaciente_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setNombresPaciente_Impl(void *self,char *nombres_Paciente)
{ setValue(self,POS_NOMBRES_PAC,nombres_Paciente); }
//----------------------------------------------------
static void setApellidoPaciente_Impl(void *self,char *apellido_Paciente)
{ setValue(self,POS_APELLIDO_PAC,apellido_Paciente); }
//----------------------------------------------------
static void setFechaNacPaciente_Impl(void *self,char *f_nac_Paciente)
{ setValue(self,POS_FNAC_PAC,f_nac_Paciente); }
//----------------------------------------------------
static void setDomicilioPaciente_Impl(void *self,char *domicilio_Paciente)
{ setValue(self,POS_DOMICILIO_PAC,domicilio_Paciente); }
//----------------------------------------------------
static void setTelefonoPaciente_Impl(void *self,char *telefono_Paciente)
{ setValue(self,POS_TELEFONO_PAC,telefono_Paciente); }
//----------------------------------------------------
static void setCodPostalPaciente_Impl(void *self,int val)
{ setValue(self,POS_COD_POSTAL_PAC,&val); }
//----------------------------------------------------
static void setDeRiesgoPaciente_Impl(void *self,int val)
{ setValue(self,POS_DE_RIESGO_PAC,&val); }
//----------------------------------------------------
static void setVacunadoPaciente_Impl(void *self,int val)
{ setValue(self,POS_VACUNADO_PAC,&val); }
//----------------------------------------------------
static void setObservacionesPaciente_Impl(void *self,char *obs)
{ setValue(self,POS_OBS_PAC,obs); }
//----------------------------------------------------
static void destroyInternalPac_Impl(void *self)
{
	//implementado
    obj_Paciente *obj = this(self);
    if(obj->localidad != NULL)
    {
        destroyObj(obj->localidad);
        obj->localidad = NULL;
    }
}
//----------------------------------------------------
//implementacion de relaciones
//----------------------------------------------------
obj_Localidad *getLocalidadPacienteObj_Impl(void *self)
{
	//implementado
    obj_Paciente *obj = this(self);
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
static void *init_Paciente(void *self)
{
  obj_Paciente *obj     = this(self);  
  obj->ds  			    = &table_Paciente;
  obj->constructor 	    = Paciente_new;
  obj->sizeObj 		    = sizeof(obj_Paciente*);
  obj->localidad 	    = NULL;  
  //incializacion de la interfaz de la entidad
  obj->toString    		= toString_PacienteImpl;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getDni  	  		= getDniPaciente_Impl;  
  obj->getNombres 		= getNombresPaciente_Impl;
  obj->getApellido 		= getApellidoPaciente_Impl;
  obj->getFechaNac  	= getFechaNacPaciente_Impl;
  obj->getDomicilio 	= getDomicilioPaciente_Impl;
  obj->getTelefono 		= getTelefonoPaciente_Impl;    
  obj->getCodPostal 	= getCodPostalPaciente_Impl;  
  obj->getDeRiesgo 		= getDeRiesgoPaciente_Impl;  
  obj->getVacunado   	= getVacunadoPaciente_Impl;  
  obj->getObservaciones	= getObservacionesPaciente_Impl;  
  /// setters  
  obj->setDni     		= setDniPaciente_Impl;    
  obj->setNombres 		= setNombresPaciente_Impl;
  obj->setApellido 		= setApellidoPaciente_Impl;
  obj->setFechaNac 		= setFechaNacPaciente_Impl;
  obj->setDomicilio 	= setDomicilioPaciente_Impl;
  obj->setTelefono 		= setTelefonoPaciente_Impl;  
  obj->setCodPostal     = setCodPostalPaciente_Impl;
  obj->setDeRiesgo      = setDeRiesgoPaciente_Impl;
  obj->setVacunado      = setVacunadoPaciente_Impl;
  obj->setObservaciones = setObservacionesPaciente_Impl;
  // implementar detroy internal para liberar recursos  
  obj->destroyInternal 	= destroyInternalPac_Impl;
  //---- acceso a relaciones  
  obj->getLocalidadObj 	= getLocalidadPacienteObj_Impl;  
  return obj;
}
//----------------------------------------------------
//constructor de Paciente
obj_Paciente *Paciente_new()
{
  return (obj_Paciente *)init_obj(sizeof(obj_Paciente), init_Paciente);
}
//----------------------------------------------------
//listar pacientes
void listarPacientes(char* archivo, bool descendente){
	
	printf("\n\t\t\t\tPACIENTES\n");
	
	FILE *salida = stdin;
	
	obj_Paciente *pac, *aux;
	void *list,*itm;
	int i,size=0;
	
	pac = Paciente_new();
	size = pac->findAll(pac,&list,NULL);
	
	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoDescP);
	else
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoAscP);
		
	// IMPRIME
	for(i=0;i<size;++i){
		itm = ((obj_Paciente **)list)[i];
		aux = (obj_Paciente*)itm; 
			
		if(archivo != NULL)
			fprintf(salida, "DNI:%d  Nombre:%s Apellido:%s \n",
				aux->getDni(aux),
				aux->getNombres(aux),
				aux->getApellido(aux)
			);
		else
			((Object *)itm)->toString(itm);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de pacientes exportado\n\n");
	}
	printf("\n");
	destroyObjList(list,size); 
	destroyObj(pac);
}
//+++++++++++++++++++++++++++++++++++++++++++++
void actualizarPaciente()
{
	obj_Paciente *paciente;
	paciente = Paciente_new();
	
	printf("USTED ESTA ACTUALIZANDO UN PACIENTE\n\n");	
	listarPacientes(NULL,false);
	
	do{
		confirmar = false;

		ingresarNumero("Ingrese el Numero de Documento del paciente:\t\t",&cadena);
		codigo = atoi(cadena);

		if(paciente->findbykey(paciente,codigo) != NOT_FOUND)
		{
			ingresarCadena("Ingrese el/los apellido/s del paciente:\t\t\t",&cadena);
			paciente->setApellido(paciente,cadena);
			
			ingresarCadena("Ingrese los nombres del paciente:\t\t\t",&cadena);
			paciente->setNombres(paciente,cadena);
							
			ingresarFecha("Ingrese fecha de nacimiento del paciente (aaaa-mm-dd):\t",&cadena);
			paciente->setFechaNac(paciente,cadena);
			
			ingresarCadena("Ingrese el domicilio del paciente:\t\t\t",&cadena);
			paciente->setDomicilio(paciente,cadena);
							
			obj_Localidad *localidad;
			localidad = Localidad_new();
			
			listarLocalidades(NULL,false);
			
			bool reintentar; 
			
			do{
				reintentar = false;
				
				ingresarNumero("Ingrese el nuevo codigo postal del paciente:\t\t\t", &cadena);
				codigo = atoi(cadena);
			
				if(localidad->findbykey(localidad,codigo) != NOT_FOUND)
				{
					paciente->setCodPostal(paciente,codigo);
					
					ingresarNumero("Ingrese el numero de telefono:\t\t\t\t",&cadena);
					paciente->setTelefono(paciente,cadena);
					
					bool resp = continuar("¿El paciente sigue siendo de riesgo? s/n:\t\t");
					paciente->setDeRiesgo(paciente,resp);
					
					resp = continuar("¿El paciente esta vacunado?s/n:\t\t\t\t");
					paciente->setVacunado(paciente,resp);
				
					ingresarCadena("Ingrese observaciones:\t\t\t\t\t", &cadena);
					paciente->setObservaciones(paciente,cadena);
				  
					if(!paciente->saveObj(paciente))
						printf("\nOcurrio un error al agregar al paciente:\n%s\n",getLastError());
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
			printf("\nERROR: paciente no encontrado\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
	destroyObj(paciente);
}

//-----------------------------------------------------------------------------------


//----------------------------------------------------------------------------
void registrarPaciente(){
	
	obj_Paciente *paciente;
	paciente = Paciente_new();
		
	printf("USTED ESTA REGISTRANDO UN PACIENTE\n\n");
	
	ingresarNumero("Ingrese el Numero de Documento:\t\t",&cadena);
	codigo = atoi(cadena);
	paciente->setDni(paciente,codigo); //seteamos el id 

	ingresarCadena("Ingrese el/los apellido/s del paciente:\t", &cadena);
	paciente->setApellido(paciente,cadena);

	ingresarCadena("Ingrese el/los nombres del paciente:\t",&cadena);
	paciente->setNombres(paciente,cadena);

	do{
		ingresarFecha("Ingrese fecha de nacimiento (aaaa-mm-dd):\t", &cadena);
		bandera= esFechaMenor(&cadena);
	}while(bandera==0);
	
	paciente->setFechaNac(paciente,cadena);

	ingresarCadena("Ingrese el domicilio del paciente:\t",&cadena);
	paciente->setDomicilio(paciente,cadena);

	obj_Localidad *localidad;
	localidad = Localidad_new();
	
	listarLocalidades(NULL,false);

	do{
		confirmar = false;

		ingresarNumero("Ingrese codigo postal del paciente:\t\t",&cadena);
		codigo = atoi(cadena);

		if(localidad->findbykey(localidad,codigo) != NOT_FOUND)
		{
			paciente->setCodPostal(paciente,codigo);
	
			ingresarNumero("Ingrese el numero de telefono:\t\t",&cadena);
			paciente->setTelefono(paciente,cadena);
	
			bool resp = continuar("¿Es de riesgo la persona?s/n:\t\t");
			paciente->setDeRiesgo(paciente,resp);
	
			paciente->setVacunado(paciente,false);
	
			ingresarCadena("Ingrese observaciones:\t",&cadena);			
			paciente->setObservaciones(paciente,cadena);
	
			if(!paciente->saveObj(paciente))
				printf("\nOcurrio un error al agregar al paciente:\n%s\n",getLastError());
			else
				printf("\nPACIENTE CREADO\n");
				
			system("pause");
			
		}else{
			printf("\nERROR: no se pudo encontrar la localidad\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);

	destroyObj(paciente);
	destroyObj(localidad);
}


int obtenerCodLocalidad(int dni){
	
	obj_Paciente *pac;
	pac = Paciente_new();
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
		
	//listamos por el numero de documento
	sprintf(criterio, "nro_documento=%d",dni);
	size = pac->findAll(pac,&list,criterio);
	
	pac = ((obj_Paciente **)list)[size-1];
	return pac->getCodPostal(pac);		
}
/*
void listarPacientePorLocalidad(char* archivo, bool descendente){


    
    
    obj_Paciente *pac, *aux;
    pac = Paciente_new();
    aux = Paciente_new();

	
    int localidades[3] = {9100, 9103, 9120};
    
	char criterio[MAX_SQL];
    void *list;
    int i, size;


    
    
	if (descendente)
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoDescP);
	else
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoAscP);
	
	//obtenemos todas las localidades
	
	int j;

	for(i=0; i < 3; ++i){
		
		sprintf(criterio, "cod_postal=%d",localidades[i]);	
		size = pac->findAll(pac,&list,criterio);
		
		for (j = 0; j < size; ++j){
			
			pac = ((obj_Paciente **)list)[j]; 

			
			if(archivo != NULL)
				fprintf(salida, "DNI:%d  Nombre:%s Apellido:%s \n",

			else
				pac->toString(pac);
		}
	}
	

	
	printf("\n");
	system("pause");
	destroyObjList(list,size); 
	destroyObj(pac);

}
*/



listarPacientesPorLocalidad(char* archivo, bool descendente){

    obj_Paciente *pac;
    pac = Paciente_new();

	FILE *salida = stdin;
		
    char criterio[MAX_SQL];
    void *list, *itm;
    int i, size;

	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }

    sprintf(criterio, "cod_postal=9100 or cod_postal=9103 or cod_postal=9120");
	size = pac->findAll(pac,&list,criterio); 

	if (descendente)
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoDescP);
	else
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoAscP);

	
	
    for(i=0; i<size; ++i){
    	
    	pac = ((obj_Paciente **)list)[i];
    	
    	if(archivo != NULL)
			fprintf(salida, "DNI:%d  Nombre:%s Apellido:%s", 
				pac->getDni(pac),
				pac->getNombres(pac),
				pac->getApellido(pac));
		else      
        	pac->toString(pac);   
    }
    
    if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de pacientes exportado\n\n");
	}
}



