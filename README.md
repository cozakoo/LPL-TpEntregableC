# Sistema de Registro de vacunación Covid en C

    Dada la continuidad de la pandemia, y que existen varios tipos de vacunas disponibles para ser aplicadas a la población se ha visto la necesidad de registrar en un sistema el proceso de vacunación, donde queda constancia de las personas que se vacunan, que vacuna se aplican, en qué lugar(centro de atención), que enfermero procede a la aplicación y cuando es el turno de vacunación.
    
    Del registro se pretende obtener una serie de listados con los cuales controlar de mejor manera el proceso de vacunación. A continuación se lista una serie de entidades de la que se deben tener registro de información. 
    
    Por protocolo cada centro de atención solo permitirá por día a unas 72 personas, de
    Lunes a Sábado, y el horario de atención disponible es desde las 8 de la mañana a las 16 hs, y se debe tener un espacio de 20 minutos entre paciente. 
    
    Se dispone de 3 enfermeros por centro de atención. 
    
    Turnos disponibles 8:00, 8:20... cada 20 minutos y distribuido en tres enfermeros.

| Enfermero | Lunes | Martes | Miércoles | Jueves | Viernes | Sábado |
|-----------|-------|--------|-----------|--------|---------|--------|
| Enfermero1| 8:30 8:00 
| Enfermero2|       | 8:30 8:00 
| Enfermero3|       |         |8:30 8:00 
