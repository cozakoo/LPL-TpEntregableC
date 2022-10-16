
create table localidad(cod_postal int primary key, nombre varchar(60));
create table centro_atencion(codigo serial primary key, nombre varchar(60), domicilio varchar(120),telefono varchar(20),cod_postal int);
alter table centro_atencion add constraint fk_centro_at_localidad foreign key(cod_postal) references localidad(cod_postal);
create table enfermeros(nro_documento int primary key, nombres varchar(80), apellido varchar(90), domicilio varchar(120),telefono varchar(20), matricula varchar(15), cod_postal int);
alter table enfermeros add constraint fk_enfermero_localidad foreign key(cod_postal) references localidad(cod_postal);
create table pacientes(nro_documento int primary key, nombres varchar(80), apellido varchar(90), fecha_nac Date, domicilio varchar(120), telefono varchar(20),cod_postal int, de_riesgo bit, vacunado bit, observaciones varchar(250));
alter table pacientes add constraint fk_paciente_localidad foreign key(cod_postal) references localidad(cod_postal);

alter table pacientes  ALTER COLUMN de_riesgo TYPE boolean using false;
alter table pacientes  ALTER COLUMN vacunado TYPE boolean using false;

create table tipo_vacuna(codigo serial primary key, nombre varchar(90), cant_dosis int);
create table vacunas(codigo serial primary key, lote varchar(10),fecha_venc date, cod_tipo_vacuna int, cantidad float);
alter table vacunas add constraint fk_vacuna_tipo_vac foreign key(cod_tipo_vacuna) references tipo_vacuna(codigo);
create table turno_vacuna(codigo serial primary key, fecha date, hora time,nro_doc_paciente int, nro_doc_enfermero int, asistio bit, cod_centro_a int, nro_dosis int,  cod_vacuna int);
alter table turno_vacuna add constraint fk_turno_vacuna_paciente foreign key(nro_doc_paciente) references pacientes(nro_documento);
alter table turno_vacuna add constraint fk_turno_vacuna_enfermero foreign key(nro_doc_enfermero) references enfermeros(nro_documento);
alter table turno_vacuna add constraint fk_turno_vacuna_centro_at foreign key(cod_centro_a) references centro_atencion(codigo);
alter table turno_vacuna add constraint fk_turno_vacuna_vacuna foreign key(cod_vacuna) references vacunas(codigo);
alter table turno_vacuna ALTER COLUMN asistio TYPE boolean using false;
/* carga de informacion */

insert into localidad(cod_postal,nombre) values (9100,'Trelew'),(9103,'Rawson'),(9120, 'Puerto Madryn');
insert into tipo_vacuna(nombre, cant_dosis) values('pfizer-BiONTech',2),('Moderna',2),('Sputnik V',2);
