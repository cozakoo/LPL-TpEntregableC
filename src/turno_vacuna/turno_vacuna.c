#include <stdio.h>
#include <stdlib.h>
#include <libpq-fe.h> 
#include <string.h>
#include "../../includes/includelib.h"
#include "../localidad/localidad.h"
#include "../centro_atencion/centro_atencion.h"
#include "../paciente/paciente.h"
#include "../enfermero/enfermero.h"
#include "../tipo_vacuna/tipo_vacuna.h"
#include "../vacuna/vacuna.h"
#include "turno_vacuna.h"

#define MSJ_REINTENTAR "Desea reintentar la operacion? s/n\t"
#define MAX_CARACTERES 50
#define fgets(cadena) fgets(cadena,MAX_CARACTERES,stdin)

char cadena[MAX_CARACTERES];
int codigo;
int bandera;

bool confirmar;	//permitira controlar si el usuario quiere reintentar operacion
bool registrar; //permitira controlar si el usuario quiere seguir registrando
bool resp;
bool seguir; 
int i;


THIS(obj_TurnoVacuna)// crea definicion de funcion this para este modulo. .. Macro en config.h

//----------------------------------------------------
int compara_CodigoDescT(const void *l1, const void * l2)
{
    obj_TurnoVacuna *tur1 =  *((obj_TurnoVacuna **)l1);
    obj_TurnoVacuna *tur2 =  *((obj_TurnoVacuna **)l2);
    
    return (tur1->getId(tur1) - tur2->getId(tur2))*-1;
}
//--------------------------------------------------------------------------
int compara_CodigoAscT(const void *l1, const void * l2)
{
    obj_TurnoVacuna *tur1 =  *((obj_TurnoVacuna **)l1);
    obj_TurnoVacuna *tur2 =  *((obj_TurnoVacuna **)l2);
    
    return (tur2->getId(tur2)) - (tur1->getId(tur1))*-1;
}
//----------------------------------------------------
static void toString_TurnoVacunaImpl(void *self)
{
	//implementado con la mayor cantidad de datos
    obj_TurnoVacuna *obj=this(self);
    obj_Vacuna *vacuna = obj->getVacunaObj(obj);
    obj_Paciente *paciente = obj->getPacienteObj(obj);
    obj_Enfermero *enfermero = obj->getEnfermeroObj(obj);
    obj_CentroAtencion *centroAtencion = obj->getCentroAtencionObj(obj);
    
    printf("Codigo: %d - FNac: %s - hora: %s - Paciente: %d - Enfermero: %d - Centro: %d - Dosis: %d - Asistio: %d\n",
	
	obj->getId(obj),
	obj->getFecha(obj),
	obj->getHora(obj),
	obj->getDocPaciente(obj),
	obj->getDocEnfermero(obj),
	obj->getCodCentroAtencion(obj),
	obj->getNroDosis(obj),
	obj->getAsistio(obj)
	);
}
//----------------------------------------------------
//implementacion de getters
//----------------------------------------------------
static int getIdTurnoVacuna_Impl(void *self)
{ return *( (int *) getValue(self,POS_ID)); }
//----------------------------------------------------
static int getCodCentroAtencion_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_COD_CENTRO_A_T)); }
//----------------------------------------------------
static int getCodPaciente_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_DOC_PACIENTE_T)); }
//----------------------------------------------------
static int getCodEnfermero_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_DOC_ENFERMERO_T)); }
//----------------------------------------------------
static char *getFecha_T_Impl(void *self)
{ return  (char *) getValue(self,POS_FECHA_T); }
//----------------------------------------------------
static char *getHora_T_Impl(void *self)
{ return  (char *) getValue(self,POS_HORA_T); }
//----------------------------------------------------
static int getAsistio_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_ASISTIO_T)); }
//----------------------------------------------------
static int getNroDosis_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_NRO_DOSIS_T)); }
//----------------------------------------------------
static int getCod_Vacuna_T_Impl(void *self)
{ return *( (int *) getValue(self,POS_COD_VACUNA_T)); }
//----------------------------------------------------
//implementacion setters
//----------------------------------------------------
static void setIdTurnoVacuna_Impl(void *self,int val)
{ setValue(self,POS_ID,&val); }
//----------------------------------------------------
static void setCodCentroAtencion_T_Impl(void *self,int val)
{ setValue(self,POS_COD_CENTRO_A_T,&val); }
//----------------------------------------------------
static void setDocPaciente_T_Impl(void *self,int val)
{ setValue(self,POS_DOC_PACIENTE_T,&val); }
//----------------------------------------------------
static void setDocEnfermero_T_Impl(void *self,int val)
{ setValue(self,POS_DOC_ENFERMERO_T,&val); }
//----------------------------------------------------
static void setFecha_T_Impl(void *self,char *fecha)
{ setValue(self,POS_FECHA_T,fecha); }
//----------------------------------------------------
static void setHora_T_Impl(void *self,char *hora)
{ setValue(self,POS_HORA_T,hora); }
//----------------------------------------------------
static void setAsistio_T_Impl(void *self,int val)
{ setValue(self,POS_ASISTIO_T,&val); }
//----------------------------------------------------
static void setNroDosis_T_Impl(void *self,int val)
{ setValue(self,POS_NRO_DOSIS_T,&val); }
//----------------------------------------------------
static void setCod_Vacuna_T_Impl(void *self,int val)
{ setValue(self,POS_COD_VACUNA_T,&val); }
//----------------------------------------------------
static void destroyInternalTurnoVacuna_Impl(void *self)
{
	obj_TurnoVacuna *obj = this(self);

	// implementado
	if(obj->paciente != NULL){
		if(obj->enfermero != NULL){
			if(obj->centro_atencion != NULL){
				if(obj->vacuna != NULL){
					//Destruyen los objetos relacionados
					destroyObj(obj->paciente);
					destroyObj(obj->enfermero);
					destroyObj(obj->centro_atencion);
					destroyObj(obj->vacuna);
					
					obj->paciente = NULL;
					obj->enfermero = NULL;
					obj->centro_atencion = NULL;
					obj->vacuna = NULL;		
				}
			}
		}
	}
}
//----------------------------------------------------
//implementacion de relaciones
//----------------------------------------------------
obj_Paciente *getPacienteTurnoVacunaObj_Impl(void *self)
{
	// implementado
    obj_TurnoVacuna *obj = this(self);
    obj->paciente = Paciente_new();

    if(obj->paciente->findbykey(obj->paciente, obj->getDocPaciente(obj))!= NOT_FOUND)
    	return obj->paciente;    	
    return NULL;
}
//----------------------------------------------------
obj_Enfermero *getEnfermeroTurnoVacunaObj_Impl(void *self)
{
	// implementado
    obj_TurnoVacuna *obj = this(self);
    obj->enfermero = Enfermero_new();

    if(obj->enfermero->findbykey(obj->enfermero, obj->getDocEnfermero(obj))!= NOT_FOUND)
    	return obj->enfermero;    	
    return NULL;
}
//----------------------------------------------------
obj_CentroAtencion *getCentroAtencionTurnoVacunaObj_Impl(void *self)
{
	// implementado
    obj_TurnoVacuna *obj = this(self);
    obj->centro_atencion =  CentroAtencion_new();

    if(obj->centro_atencion->findbykey(obj->centro_atencion, obj->getCodCentroAtencion(obj))!= NOT_FOUND)
    	return obj->centro_atencion;    	
    return NULL;
}
//----------------------------------------------------
obj_Vacuna *getVacunaTurnoVacunaObj_Impl(void *self)
{		
	// implementado
    obj_TurnoVacuna *obj = this(self);
    obj->vacuna = Vacuna_new();

    if(obj->vacuna->findbykey(obj->vacuna,obj->getCodVacuna(obj))!= NOT_FOUND)
    	return obj->vacuna;    	
    return NULL;
}
//----------------------------------------------------
//implementacion constructor
//----------------------------------------------------
static void *init_TurnoVacuna(void *self)
{
  obj_TurnoVacuna *obj  = this(self);
  obj->ds                     = &table_TurnoVacuna;
  obj->constructor            = TurnoVacuna_new;
  obj->sizeObj                = sizeof(obj_TurnoVacuna*);
  //incializacion de la interfaz de la entidad
  obj->toString               = toString_TurnoVacunaImpl;
  obj->paciente				  = NULL;
  obj->enfermero		  	  = NULL;
  obj->vacuna				  = NULL;
  obj->centro_atencion           = NULL;
  // Inicializar handlers de getters y setters
  /// getters
  obj->getId = getIdTurnoVacuna_Impl;
  obj->getCodCentroAtencion   = getCodCentroAtencion_T_Impl;
  obj->getDocPaciente         = getCodPaciente_T_Impl;
  obj->getDocEnfermero  	  = getCodEnfermero_T_Impl;
  obj->getFecha               = getFecha_T_Impl;  
  obj->getHora                = getHora_T_Impl;
  obj->getAsistio			  = getAsistio_T_Impl;
  obj->getNroDosis			  = getNroDosis_T_Impl;
  obj->getCodVacuna			  = getCod_Vacuna_T_Impl;
    /// setters  
  obj->setId 				  = setIdTurnoVacuna_Impl;
  obj->setCodCentroAtencion   = setCodCentroAtencion_T_Impl;
  obj->setDocPaciente         = setDocPaciente_T_Impl;
  obj->setDocEnfermero  	  = setDocEnfermero_T_Impl;
  obj->setFecha               = setFecha_T_Impl;  
  obj->setHora                = setHora_T_Impl;
  obj->setAsistio			  = setAsistio_T_Impl;
  obj->setNroDosis			  = setNroDosis_T_Impl;
  obj->setCodVacuna			  = setCod_Vacuna_T_Impl;
  // implementar detroy internal para liberar recursos
  obj->destroyInternal        = destroyInternalTurnoVacuna_Impl;
  //---- acceso a relaciones    
  obj->getCentroAtencionObj	  = getCentroAtencionTurnoVacunaObj_Impl;
  obj->getPacienteObj		  = getPacienteTurnoVacunaObj_Impl;
  obj->getEnfermeroObj  	  = getEnfermeroTurnoVacunaObj_Impl;
  obj->getVacunaObj			  = getVacunaTurnoVacunaObj_Impl;
  return obj;
}
//----------------------------------------------------

int obtenerDosis(int dni, int codigo){

	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();

	char criterio[MAX_SQL];

	void *list, *itm;
	int i, aux=0;
	int vacunado = 0;
		
	CLEAR(criterio,MAX_SQL);
	int size = sprintf(criterio, "nro_doc_paciente=%d and cod_centro_a=%d and asistio=TRUE",dni, codigo);
	size = turno->findAll(turno,&list,criterio);

	printf("\n");

	for(i=0; i<size; ++i){
		turno = ((obj_TurnoVacuna **)list)[i];
		
		vacunado = turno->getAsistio(turno);
			
		if (vacunado)
			++aux;
	}
	++aux;
	
	return aux;	
}

//------------------------------------------------------------

int obtenerCodVacuna(int dni, int codigo){

	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();

	char criterio[MAX_SQL];

	void *list, *itm;
	int i, aux=0;
	int vacunado = 0;
		
	CLEAR(criterio,MAX_SQL);
	int size = sprintf(criterio, "nro_doc_paciente=%d and cod_centro_a=%d",dni, codigo);
	size = turno->findAll(turno,&list,criterio);

	turno = ((obj_TurnoVacuna **)list)[size-1];	
	return turno->getCodVacuna(turno);
}

//------------------------------------
 int verificarTurnosPrevios(int dni, int codigo){
	
	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();

	char criterio[MAX_SQL];
	void *list;
	int size;
	int i;
	
			
	int turnosAsistidos=0;
	int estado;
	int codigoVacuna;
	char fechaActual[MAXFECHA];
	strcpy(fechaActual, getFecha());
		
	CLEAR(criterio,MAX_SQL);
	//OBTENEMOS TODOS LOS TURNOS DEL PACIENTES EN OTROS CENTROS DE ATENCION
	sprintf(criterio, "nro_doc_paciente=%d and cod_centro_a<>%d",dni,codigo);
	size = turno->findAll(turno,&list,criterio); // turnos totales del paciente que ha asistido
		
	if (size != 0){ 
	// TIENE TURNOS EN OTROS CENTROS


		
		printf("\nTURNOS QUE HA TENIDO EL PACIENTE EN OTROS CENTROS DE ATENCION\n");
		
		for(i=0; i<size; ++i){
			turno = ((obj_TurnoVacuna **)list)[i];
			turno->toString(turno);
		
			if ((strcmp(turno->getFecha(turno), fechaActual) > 0))
				estado =  3;
			else{
				if (turno->getAsistio(turno) == 1){
					turnosAsistidos++;
					codigoVacuna = turno->getCodVacuna(turno);
				}
			}
		}
		
		if (estado == 3)
			return estado;
	}
	
	CLEAR(criterio,MAX_SQL);
	//OBTENEMOS TODOS LOS TURNOS DEL PACIENTES EN EL CENTRO CENTROS DE ATENCION
	sprintf(criterio, "nro_doc_paciente=%d and cod_centro_a=%d",dni,codigo);
	size = turno->findAll(turno,&list,criterio); // turnos totales del paciente que ha asistido
		
	if (size != 0){ 
	// TIENE TURNOS EN ESTE CENTRO DE ATENCION	
	
		printf("\n\nTURNOS QUE HA TENIDO EL PACIENTE EN ESTE TURNO DE ATENCION\n");
		
		for(i=0; i<size; ++i){
			turno = ((obj_TurnoVacuna **)list)[i];
			turno->toString(turno);
		
			if ((strcmp(turno->getFecha(turno), fechaActual) > 0))
				estado =  1;
			else {
				if (turno->getAsistio(turno) == 1){
					turnosAsistidos++;
					codigoVacuna = turno->getCodVacuna(turno);
				}
			}
		}
		if (estado == 3)
			return estado;
	}
	if (turnosAsistidos == 0)
		return 4;		// si no tiene registro lo tratamos como paciente nuevo
	else{
		obj_Vacuna *vac;
		vac = Vacuna_new();	
	
		CLEAR(criterio,MAX_SQL);
		sprintf(criterio, "codigo=%d",codigoVacuna);
		size = vac->findAll(vac,&list,criterio);
		
		vac =  ((obj_Vacuna **)list)[size-1];
		
		int codTipoVacuna = vac->getCodTipoVacuna(vac);
	
		destroyObjList(list,size); // liberar listado, cada instacia creada en el listado
		
		obj_TipoVacuna *tvac;
		tvac = TipoVacuna_new();
		
		CLEAR(criterio,MAX_SQL);		
		
		sprintf(criterio, "codigo=%d",codTipoVacuna);
		size = tvac->findAll(tvac,&list,criterio);
					
		tvac = ((obj_TipoVacuna **)list)[size-1];
				
		int cantDosisTvac = tvac->getCantidadDosis(tvac);
				
		//si la cantidad de turnos que ha asistido el paciente en todos los centros de atencion
		//es igual a la cantidad maxima de dosis se rechaza
		
		if (cantDosisTvac == turnosAsistidos)
			return 2;
		else 
			return 0;
	}
}

static int menuEnfermeroCentro(){
	
	int opcion = 0;
	
	printf("\n\nNO HAY ENFERMERO REGISTRADO EN EL CENTRO DE ATENCION\n\n");
	printf("QUE OPERACION DESEA REALIZAR\n");
	printf("1- INGRESAR ENFERMERO DEL LISTADO\n");
	printf("2- REGISTRAR ENFERMERO \n");
	printf("3- SALIR\n\n"); 
	printf("INDIQUE LA OPERACION A REALIZAR\n");
	scanf("%d",&opcion);

	return opcion;
}

void incrementarFecha(char *fechaTurno){ //{dia,mes,anio}

	char fecha[MAXFECHA];
	strcpy(fecha, fechaTurno);
	int fechaAux[2];
	particionarFecha(fecha,fechaAux);
		
	if (fechaAux[2] == 31){
		fechaAux[2] = 1;	
		
		if(fechaAux[1] == 12){
			fechaAux[1] = 1;
			fechaAux[0] = fechaAux[0] + 1;
		}else
			fechaAux[1] = fechaAux[1] + 1;
	}else{		
		if((fechaAux[2] == 30) && ((fechaAux[1]==4) || (fechaAux[1]==6) || (fechaAux[1]==9) || (fechaAux[1] == 11))){
			fechaAux[2] = 1;
			fechaAux[1] = fechaAux[1] + 1;
		}else
			fechaAux[2] = fechaAux[2] + 1 ;
	}
	
	char anioA[MAXFECHA];
	char mesA[MAXFECHA];
	char diaA[MAXFECHA];
	
	//casteamos de entero a cadena	
	sprintf(anioA, "%d", fechaAux[0]);
	sprintf(mesA, "%d", fechaAux[1]);
	sprintf(diaA, "%d", fechaAux[2]);
	
	//concatenamos
	strcpy(fecha, anioA);
	strcat(fecha, "-");
	strcat(fecha, mesA);
	strcat(fecha, "-");
	strcat(fecha, diaA);
	
	strcpy(fechaTurno,fecha);
}

void convertirFecha(char *fecha, int *fechaTurno){
	
	int dia = fechaTurno[0];
	int mes = fechaTurno[1];
	int anio = fechaTurno[2];
	
	char diaA[MAXFECHA];
	char mesA[MAXFECHA];
	char anioA[MAXFECHA];

	//casteamos de entero a cadena	
	sprintf(diaA, "%d", dia);
	sprintf(mesA, "%d", mes);
	sprintf(anioA, "%d", anio);

	//concatenamos
	strcpy(fecha, diaA);
	strcat(fecha, "-");
	strcat(fecha, mesA);
	strcat(fecha, "-");
	strcat(fecha, anioA);	
}

void actualizarTurno(){
	
	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();
	
	void *list,*itm;
	int i,size=0;
	char criterio[MAX_SQL];
	printf("USTED ESTA ACTUALIZANDO UN TURNO\n\n");
	
	sprintf(criterio, "asistio=FALSE");
	size = turno->findAll(turno,&list,criterio);	

	for(i=0;i<size;++i){
		turno = ((obj_TurnoVacuna **)list)[i];
		turno ->toString(turno);
	}
	do {
		confirmar = false;
		
		ingresarNumero("Ingrese el Id del turno a modificar:\t", &cadena);
		codigo = atoi(cadena);
		
		if(turno->findbykey(turno,codigo) != NOT_FOUND)
		{
			int estado;
			do{
				ingresarFecha("Ingrese nueva fecha de turno(aaaa-mm-dd):\t", &cadena);
				bandera= esFechaMayor(&cadena);
				estado = esDomingo(&cadena);
				
			}while((bandera==0) && (estado == 0));	
			
			if(!turno->saveObj(turno))
				printf("\nOcurrio un error al actualizar el turno:\n%s\n",getLastError());
			else
				printf("\nTURNO ACTUALIZADO\n");
				
			system("pause");
		}else{
			printf("\nERROR: no se pudo encontrar el ID\n\n");
			confirmar = continuar(MSJ_REINTENTAR);
		}
	}while(confirmar);
		
	
	
}

int verificarEnfermero(int documento, char* fecha, int codCentro){

	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();
	
	char criterio[MAX_SQL];
	void *list, *itm;
	int i, size;
	
	sprintf(criterio, "fecha='%s' and nro_doc_enfermero=%d and cod_centro_a=%d",fecha,documento,codCentro);
	size = turno->findAll(turno,&list,criterio);	
	
	if (size == 24)
		return 0;
	else 
		return 1;
}

int esDomingo(char *fechaTurno){
	
	if (strcmp(getDiaFecha(fechaTurno),"Domingo") == 0)
		return 0;
	else
		return 1;
}

int verificarTurno(int *turnoFecha, int codCentro){ //FALTA EL CODIGO
	
	int dia = turnoFecha[0];
	int mes = turnoFecha[1];
	int anio = turnoFecha[2];
	
	char fechaAux[MAXFECHA];
	char diaAux[MAXFECHA];
	char mesAux[MAXFECHA];
	char anioAux[MAXFECHA];
	
	//casteamos de entero a cadena	
	sprintf(diaAux, "%d", dia);
	sprintf(mesAux, "%d", mes);
	sprintf(anioAux, "%d", anio);

	//concatenamos
	strcpy(fechaAux, diaAux);
	strcat(fechaAux, "-");
	strcat(fechaAux, mesAux);
	strcat(fechaAux, "-");
	strcat(fechaAux, anioAux);

	if (strcmp(getDiaFecha(fechaAux),"Domingo") == 0){
		return 0;
	}else{		
		obj_TurnoVacuna *turno;
		turno = TurnoVacuna_new();
	
		char criterio[MAX_SQL];
		void *list;
		int size;

		sprintf(criterio,"fecha='%s' and cod_centro_a=%d",fechaAux, codCentro);
		size = (turno->findAll(turno,&list,criterio));			
		
		int cantTurno = size + 1;
		
		if (cantTurno == 72){
			printf("LOS TURNOS SE ENCUENTRAN LLENOS");
			system("pause");
			return 0;
		}
		else{
			return 1;
		}	
	}
}

int verificarDisponibilidad(int codigo, char *fechaTurno){
	
	obj_TurnoVacuna *turnoVac;
	turnoVac = TurnoVacuna_new();
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
	int turnosTotal;
	
	CLEAR(criterio,MAX_SQL);
	sprintf(criterio, "cod_centro_a=%d and fecha= '%s' ",codigo,fechaTurno);
	size = turnoVac->findAll(turnoVac,&list,criterio);
	
	return size;
}


int asignarCentro(int codLocalidad, char *fTurno){
	
	obj_CentroAtencion *cen;	
	cen = CentroAtencion_new();

	char fechaAux[MAXFECHA];
	strcpy(fechaAux, fTurno);
	
	char criterio[MAX_SQL];
	void *list;
	int i=0,size=0;
	int turnoTotal=72;
	int codCentro= 0;

	//listamos por el codigo postal
	
	sprintf(criterio, "cod_postal=%d",codLocalidad);
	size = cen->findAll(cen,&list,criterio);
	

	//si hay solo un centro de atencion
	if(size == 1){
		cen = ((obj_CentroAtencion **)list)[size-1];
		turnoTotal = verificarDisponibilidad(cen->getId(cen),fechaAux);
		
		if (turnoTotal < 72)
			codCentro = cen->getId(cen);		
	}
	else{
		int aux = 72; 
		for(i=0; i < size;++i){
			
			cen = ((obj_CentroAtencion **)list)[i];
			turnoTotal = verificarDisponibilidad(cen->getId(cen),fechaAux);
			
			//nos quedamos con el menor
			if (turnoTotal < 72){	
				if ((turnoTotal >= 0) && ((turnoTotal < aux))){
					aux = turnoTotal;	
					codCentro = cen->getId(cen);
				}
			}
		}
	}
	return codCentro;
}

int verificarDisponibilidadEnfermero(int codCentro, char *fechaTurno, int codEnfermero){
	
	obj_TurnoVacuna *turnoVac;
	turnoVac = TurnoVacuna_new();
	
	char criterio[MAX_SQL];
	void *list;
	int i,size=0;
	int turnosTotal;

	CLEAR(criterio,MAX_SQL);
	
	sprintf(criterio, "cod_centro_a=%d and fecha= '%s' and nro_doc_enfermero=%d",codCentro,fechaTurno, codEnfermero);
	
	size = turnoVac->findAll(turnoVac,&list,criterio);
	
	return size;
}



void registrarTurno(){
		
	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();
	
	bool esPacienteNuevo = false; //para comprobar si el paciente es nuevo
	bool esEnfermeroNuevo = false; //para comprobar si el paciente es nuevo

	printf("USTED ESTA REGISTRANDO UN TURNO\n\n");
	
	// PACIENTE
	obj_Paciente *paciente;
	paciente = Paciente_new();
	
	ingresarNumero("Ingrese el DNI del paciente:\t",&cadena);
	int codigoDNI = atoi(cadena);
	
	int codLocalidad;
	//si no existe se registra, caso contrario se verifica el estado del paciente
	if (paciente->findbykey(paciente,codigoDNI) == NOT_FOUND){
		
		printf("EL PACIENTE NO ESTA REGISTRADO EN EL SISTEMA\n");
		resp = continuar("Desea registrar al paciente? s/n:\t");
		
		if (resp){
			esPacienteNuevo = true;
			turno->setDocPaciente(turno,codigoDNI);
			registrarPaciente();		
			codLocalidad = obtenerCodLocalidad(codigoDNI);
		}
		else
			exit;
	}else{
		esPacienteNuevo = false;
		codLocalidad = paciente->getCodPostal(paciente);	
	}
	
	obj_Localidad *loc;
	loc = Localidad_new();
	
	char fechaTurno[MAXFECHA];
	strcpy(fechaTurno,getFecha()); //obtenemos la fecha actual	
	
	do{
		incrementarFecha(fechaTurno);
		bandera = esDomingo(fechaTurno);
	}while(bandera == 0);
	
	char criterio[MAX_SQL];
	
	obj_CentroAtencion *centro;
	centro = CentroAtencion_new();	
	int codCentro;
	
	bandera = 1;
	int estado;
	do{	
		codCentro = asignarCentro(codLocalidad,fechaTurno);
		
		if (codCentro == 0){
			do{
				incrementarFecha(fechaTurno);
				estado = esDomingo(fechaTurno);
			}while(estado == 0);
			bandera = 0;
		}
	}while(bandera == 0);

	turno->setDocPaciente(turno,codigoDNI);
	turno->setCodCentroAtencion(turno,codCentro);
	turno->setFecha(turno,fechaTurno);
	turno->setAsistio(turno,false);
	
	//obtenemos e nombre de la localidad
	char nombreLocalidad[10];
	strcpy(nombreLocalidad,(char *)obtenerNombreLocalidad(codLocalidad));
	
	char nombreCentro[10];
	strcpy(nombreCentro,(char *)obtenerNombreCentro(codCentro));
	
	printf("NOMBRE LOCALIDAD:\t\t%s", nombreLocalidad);
	printf("NOMBRE DEL CENTRO:\t\t%s\n", nombreCentro);
	
	/* hasta aca tenemos la fecha verificada,
		el paciente, centro, nombre, localidad */
	
	int logico;
	int codVacuna=0; //variable que seatermos al turno
	int cantDosisAplicar=0;
	bool salir;
		
	/* [FALTA] enfermero, hora, vacuna */
	if (!esPacienteNuevo){
		
		logico = verificarTurnosPrevios(codigoDNI,codCentro);
		switch(logico){
			case 0:
				salir = false;
				codVacuna = obtenerCodVacuna(codigoDNI,codCentro);
				cantDosisAplicar = obtenerDosis(codigoDNI,codCentro);
				system("pause");
				break;
			case 1: 
				printf("\nEl PACIENTE TIENE TURNOS PENDIENTES DENTRO DE ESTE CENTRO DE ATENCION\n\n");
				system("pause");
				salir = true;
				break;
			case 2:
				printf("\nERROR: EL PACIENTE YA SE HA VACUNADO CON TODAS LAS DOSIS\n");
				system("pause");
				salir=true;
				break;
			case 3:
				printf("\nERROR: EL PACIENTE TIENE TURNOS PENDIENTES EN OTROS CENTROS DE ATENCION");
				system("pause");
				salir = true;
				break;
			case 4:
				esPacienteNuevo = true;
				salir = false; 
				system("pause");
				break;
		}							
	}
	
	if (esPacienteNuevo){
		//es nuevo el paciente
		salir = false;
		
		listarVacunas(NULL, false);

		bool reintentar;
		
		obj_Vacuna *vac; 
		vac = Vacuna_new();

		do {
			reintentar = false;
			ingresarNumero("\nIngrese el ID de la vacuna que se quiere aplicar:\t",&cadena);
			codVacuna = atoi(cadena);
	
			if(vac->findbykey(vac,codVacuna) != NOT_FOUND){
				cantDosisAplicar= 1;
			}
			else{
				printf("\nERROR: ID incorreto\n");
				reintentar = continuar(MSJ_REINTENTAR);
			}
		}while(reintentar);	
		destroyObj(vac);
	}
	
	if (!salir){ //se sigue la ejecucion
		
		turno->setCodVacuna(turno,codVacuna);
		turno->setNroDosis(turno,cantDosisAplicar);
		
		// LISTADO DE LOS ENFERMEROS ATENDIENDO EN EL CENTRO
		obj_Enfermero *enf;
		enf = Enfermero_new();
			
		int codEnfermero=0;
		int sizeEnf;
		void *listEnf;
	
		CLEAR(criterio,MAX_SQL);
		sprintf(criterio,"nro_documento in (select nro_doc_enfermero from turno_vacuna where cod_centro_a=%d)", codCentro);	
		sizeEnf = enf->findAll(enf,&listEnf,criterio);
		// si no hay uno se debe cargar de lista de enfermeros disponibles(OTROS).
		esEnfermeroNuevo=false;
		
		if(sizeEnf==0){
			
			do {
				bandera = menuEnfermeroCentro();	
				switch(bandera){
					case 1:
						listarEnfermeroLocalidad(codLocalidad);
						ingresarNumero("Ingrese el numero de docuento del enfermero:\t\t",&cadena);
						codigo = atoi(cadena);
						
						if(enf->findbykey(enf,codigo) != NOT_FOUND){
							codEnfermero = codigo;
							esEnfermeroNuevo=true;
							bandera = 3;
							break;								
						}else
							printf("\nERROR: no se pudo encontrar la localidad\n\n");
					case 2:
						registrarEnfermero();
						esEnfermeroNuevo=true;
						bandera = 3;
						break;
					case 3:
						exit;
						break;
					default:
						printf("Opcion incorrecta\n\n");
						system("pause");
						break;
				}
			}while(bandera!=3);
			
			ingresarNumero("Ingrese el numero de docuento del enfermero creado:\t\t",&cadena);
			codEnfermero = atoi(cadena);
			
		}else{
			int cantTurno;
			int aux = 71;

			for(i=0; i < sizeEnf; ++i){
	
				enf = ((obj_Enfermero **)listEnf)[i];
		
				cantTurno = verificarDisponibilidadEnfermero(codCentro,fechaTurno, enf->getNro_Documento(enf));
				
				//nos quedamos con el menor
				if (cantTurno < 24){	
					if ((cantTurno >= 0) && ((cantTurno < aux))){
						aux = cantTurno;	
						codEnfermero = enf->getNro_Documento(enf);
					}
				}
			}
		}
		
		turno->setDocEnfermero(turno,codEnfermero);
		destroyObjList(listEnf,sizeEnf);
		destroyObj(enf);				
		
		//obtenerInfoEnfermero(codEnfermero);
		
    	char hora[MAXFECHA];
		//Horario permitido.
    	char *horario[8][3]={
							{"08:00:00","08:20:00","08:40:00"},
							{"10:00:00","10:20:00","10:40:00"},
							{"11:00:00","11:20:00","11:40:00"},
							{"12:00:00","12:20:00","12:40:00"},
							{"13:00:00","13:20:00","13:40:00"},
							{"14:00:00","14:20:00","14:40:00"},
							{"15:00:00","15:20:00","15:40:00"}
        					};
     					
		int horario_disponible=false;
		void *list;
		int size,j;
		
		printf("\nASIGNANDO HORARIO: ");
				
		for(i=0;i<8;i++){

			for(j=0;j<3;++j){
				// consultar horario
				CLEAR(criterio,MAX_SQL);
				
				if (esEnfermeroNuevo == true)
					sprintf(criterio,"fecha='%s' and hora='%s' and cod_centro_a=%d",fechaTurno, horario[i][j],codCentro);
				else 
					sprintf(criterio,"fecha='%s' and hora='%s' and cod_centro_a=%d and nro_doc_enfermero=%d",fechaTurno, horario[i][j],codCentro,codEnfermero);
				
				size = turno->findAll(turno,&list,criterio);

				horario_disponible= (size==0);

				if(horario_disponible==true) 
					break;
			}
			if(horario_disponible==true) 
				break;
	        printf("\n");
		}
		destroyObjList(list,size);

		char fechaPrueba[MAXFECHA];
		strcpy(fechaPrueba,horario[i][j]);
		printf("\n\nHORARIO DE TURNO: %s\n\n",fechaPrueba);

		turno->setHora(turno,fechaPrueba);
						
		//GUARDAMOS
		if(!turno->saveObj(turno))
			printf("\nOcurrio un error al agregar el turno %s\n",getLastError());
		else
			printf("\nTURNO CREADO\n");
		
		system("pause");
	}else
		exit;
	
	destroyObj(centro);
	destroyObj(loc);
	destroyObj(paciente);
	destroyObj(turno); 
}

		
//constructor de TurnoVacuna
obj_TurnoVacuna *TurnoVacuna_new()
{
  return (obj_TurnoVacuna *)init_obj(sizeof(obj_TurnoVacuna), init_TurnoVacuna);
}
//-----------------------------------------------------------------------------------
int compararFecha(char *fecha1, char *fecha2){

	
	char fecha1Aux[MAXFECHA];
	strcpy(fecha1Aux, fecha1);
	int fechaMinima[2];
	particionarFecha(fecha1Aux, fechaMinima);
	
	char fecha2Aux[MAXFECHA];
	strcpy(fecha1Aux, fecha2);
	int fechaMax[2];
	particionarFecha(fecha1Aux, fechaMax);
	
	int estado;
	
	//devuelve 0 si fecha 1 es menor a la fecha 2

	//SI EL ANIO INGRESADO ES MAYOR QUE EL ANIO ACTUAL 
	if (fechaMinima[0] > fechaMax[0])
		estado = 0;
	else{
		//SI ESTAMOS EN EL MISMO ANIO PERO EL MES ES MAYOR RETORNA
		if ((fechaMinima[0] == fechaMax[0]) && (fechaMinima[1] > fechaMax[1]))
			estado = 0;
		else{
			if ((fechaMinima[0] == fechaMax[0]) && (fechaMinima[1] == fechaMax[1]) && (fechaMinima[2] < fechaMax[2]))
				estado = 0;
			else
				estado = 1;
		}
	}
	return estado;
}

void listarEnfermerosVacuna(char* archivo, bool descendente)
{

	char fechaMin[MAXFECHA];
	char fechaMax[MAXFECHA];

	do{
		ingresarFecha("\nIngrese fecha minima:\t",fechaMin);
		ingresarFecha("\nIngrese fecha maxima:\t",fechaMax);
		
		//devuelve 0 si fecha 1 es menor a la fecha 2
		bandera =  compararFecha(fechaMin,fechaMax);
				
		if (bandera == 0){
			printf("ERROR: La fecha ingresada es MAYOR o IGUAL que la fecha actual");
			system("pause");
		}		
	}while(bandera == 0);
	
	obj_TurnoVacuna *turno;
	turno = TurnoVacuna_new();

    char criterio[MAX_SQL];
    void *list;
	int size;

    sprintf(criterio, "fecha < '%s' and fecha > '%s' and asistio=TRUE", fechaMax, fechaMin);
    size = turno->findAll(turno,&list,criterio);
	
	if (size == 0){
		printf("NO HAY TURNOS EN EL RANGO DE ESTAS FECHAS\n");
	}else{	    
		int codEnfermero, size1, i;
		void *listEnf;
		char criterioEnf[MAX_SQL];
		
		obj_Enfermero *enf;
    	enf = Enfermero_new();
	    printf("\n\tENFERMEROS QUE HAN APLICADO VACUNAS\n");
		
		for(i=0; i<size; ++i){
			turno = ((obj_TurnoVacuna **)list)[i]; 
	    	codEnfermero = turno->getDocEnfermero(turno);
			CLEAR(criterioEnf,MAX_SQL);
	    	sprintf(criterioEnf, "nro_documento=%d",codEnfermero);
    		
			size1 = enf->findAll(enf,&listEnf,criterioEnf);
			
			enf = ((obj_Enfermero **)listEnf)[size1-1];
	    	enf->toString(enf);	
		}
	}
}

void listarTurnos(char* archivo, bool descendente){
	
	printf("\n\t\t\t\tTURNOS\n");
	
	FILE *salida = stdin;
	
	obj_TurnoVacuna *turno, *aux;
	void *list,*itm;
	int i,size=0;
	
	turno = TurnoVacuna_new();
	aux = TurnoVacuna_new();
	size = turno->findAll(turno,&list,NULL);
	
	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }
    
	if (descendente)
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoDescT);
	else
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoAscT);
		
	// IMPRIME
	for(i=0;i<size;++i){
		turno = ((obj_TurnoVacuna **)list)[i];
		aux = turno;
		
		if(archivo != NULL)
			fprintf(salida, "ID: %d - FNac: %s - hora: %s - Paciente: %d - Enfermero: %d - Centro: %d - Dosis: %d - Asistio: %d\n",	
				aux->getId(aux),
				aux->getFecha(aux),
				aux->getHora(aux),
				aux->getDocPaciente(aux),
				aux->getDocEnfermero(aux),
				aux->getCodCentroAtencion(aux),
				aux->getNroDosis(aux),
				aux->getAsistio(aux)
			);
		else
			turno->toString(turno);
	}
	
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de pacientes exportado\n\n");
	}
	printf("\n");
	system("pause");
}


listadoTurnosCentroAtencion(char* archivo, bool descendente)
{
	
	FILE *salida = stdin;

    char fechaMin[MAXFECHA];
    char fechaMax[MAXFECHA];

    do{
        ingresarFecha("\nIngrese fecha minima:\t",fechaMin);
        ingresarFecha("\nIngrese fecha maxima:\t",fechaMax);

        //devuelve 0 si fecha 1 es menor a la fecha 2
        bandera =  compararFecha(fechaMin,fechaMax);

        if (bandera == 0){
            printf("ERROR: La fecha ingresada es MAYOR o IGUAL que la fecha actual");
            system("pause");
        }
    }while(bandera == 0);

    obj_TurnoVacuna *turno;
    turno = TurnoVacuna_new();

    char criterio[MAX_SQL];
    void *list;
    int size;

    sprintf(criterio, "fecha < '%s' and fecha > '%s' and asistio=TRUE", fechaMax, fechaMin);
    size = turno->findAll(turno,&list,criterio);

	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }


    if (size == 0){
        printf("NO HAY TURNOS EN EL RANGO DE ESTAS FECHAS\n");
    }else{
    	
        int codCentro, size1, i;
        void *listCen;
        char criterioCen[MAX_SQL];

        obj_CentroAtencion *cen;
        cen = CentroAtencion_new();
        
        printf("\n\tCENTROS DE ATENCIÓN QUE HAN TENIDO TURNOS\n");

        for(i=0; i<size; ++i){
            turno = ((obj_TurnoVacuna **)list)[i];
            codCentro = turno->getCodCentroAtencion(turno);
            
			CLEAR(criterioCen,MAX_SQL);
            sprintf(criterioCen, "codigo=%d",codCentro);

            size1 = cen->findAll(cen,&listCen,criterioCen);

            cen = ((obj_CentroAtencion **)listCen)[size1-1];
            
            if(archivo != NULL)
            	fprintf(salida,"ID: %d - NOMBRE: %s",
            	cen->getId,
            	cen->getNombre);
			else
				cen->toString(cen);
        }
    }
    
	if (archivo != NULL){
		fclose(salida); 
		printf("\nListado de pacientes exportado\n\n");
	}
}

void listarPacientesPorVacuna(char* archivo, bool descendente){

	obj_TurnoVacuna *tv;
	
    obj_Paciente *pac;
    pac = Paciente_new();
	tv = TurnoVacuna_new();
	
	FILE *salida = stdin;
		
    char criterio[MAX_SQL];
    void *list, *itm;
    int i, size;

	if(archivo != NULL) {
		printf("Exportando a archivo...\n");
        salida = fopen(archivo, "w+");
    }

	size= (tv->findAll(tv, &list,"asistio=TRUE and cod_vacuna in(select vac.codigo from vacunas vac inner join  tipo_vacuna tvac on tvac.codigo=vac.cod_tipo_vacuna where tvac.nombre='Sputnik V')"));


	if (descendente)
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoDescT);
	else
		qsort(list, size, sizeof(obj_Paciente*),compara_CodigoAscT);

	
	
    for(i=0; i<size; ++i){
    	
    	tv = ((obj_TurnoVacuna **)list)[i];
    	pac = tv->getPacienteObj(tv);
    	
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




