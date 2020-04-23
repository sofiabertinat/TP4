
 
#include "proyecto.h"

/* Función que tiene como parámetro el numero de sensor que se quiere saber su estado */
/* El numero de sensor se convierte de decimal a binario y se escribe en la entrada del multiplexor */
/* Se lee y retorna la salida analógica del multiplexor el valor del sensor seleccionado */
uint16_t LecturaSensorIR(uint8_t sensor){

	int j;
	uint8_t  valor = 0;
	int aux, resto;

	aux = sensor - OFFSET;

	for(j=0; j<LARGO_CANT_SENSORES_BIN; j++){

		resto = aux % 2;
		aux = aux / 2;

		if(resto!=0){
			valor = 1;
		}else{
			valor = 0;
		}        	
		/*Escribe en la entrada S del multiplexor */
        	gpioWrite( s[j], valor);
	}	
	return(adcRead(SENSOR_SIG));
}

//Funcion que calcula la altura del pasto 
 uint16_t CalculoAlturaPasto(void){

	 int i, j, altura;
	
	//Leo el estado de los sensores IR
	for (j=1; j<=CANT_SENSORES; j++){
					
		if(LecturaSensorIR(j) >= VALOR_UMBRAL){
			estadoSensor[j] = SENSOR_ON;
		}else{
			estadoSensor[j] = SENSOR_OFF;
		}	
	}	
			  
	altura=0;
	i = CANT_SENSORES;
	//determino la altura del pasto, dependiendo del sensor mas alto activo	
	while(i>0 && altura == 0){
		if(estadoSensor[i] == 1 ){
			altura = ALTURA_SENSOR_MIN + (i-1) * ALTURA_ENTRE_SENSORES;			
		}
		i--;
	}		
	
	return altura;
}

//  calcula la duracion del sensado de la altura del pasto 
uint16_t DuracionSensado(uint16_t metros){
	
	float duracion;
	
	duracion = metros/0.0275; // si v = 5,5 m/s --> en 5ms recorre 0,0275m
	
	return (uint16_t)duracion;
	
 }

//Funcion que calcula el promedio de la altura del pasto 
 uint16_t CalculoPromedioAlturaPasto(uint16_t cantIter){
	 
	uint16_t promedio;	
	static uint16_t suma = 0;
	static uint16_t cont = 1;
	static uint16_t altura = 0;		
		
	if(descartarMedida() == 0){				
		altura = CalculoAlturaPasto();				
	}else
		altura = 0;
	//guardo la suma de todas las aturas del pasto para poder calcular el promedio
	suma = suma + altura;		
	//calculo el promedio de la altura del pasto hasta el momento
	promedio = suma/cont;
			
	//Me fijo si ya recorri toda la distancia que se quería sensar
	if(cont == cantIter){				
		cont = 1;
		suma = 0;
								
	}else{
		cont++;										
	}
		 
	return promedio;
}

void MedidaGiroscopio(float *rad_x, float *rad_y, float *rad_z){

     //Leer el sensor y guardar en estructura de control
     //mpu9250Read();

     (*rad_x)= mpu9250GetGyroX_rads();
     (*rad_y)= mpu9250GetGyroY_rads();
     (*rad_z)= mpu9250GetGyroZ_rads();

}

uint8_t  descartarMedida( void){

	 float rad_x, rad_y, rad_z;
	 float tolmax, tolmin;

	 MedidaGiroscopio(&rad_x, &rad_y, &rad_z);
	 tolmax = 3.14/180;
	 tolmin = - 3.14/180;

	 if(rad_x>tolmax|rad_x<tolmin |rad_y>tolmax|rad_y<tolmin |rad_z>tolmax|rad_x<tolmin ){
		 return 1;
	 }else{
		 return 0;
	}
 }



