
 
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
        	gpio_mux(j, valor); //gpioWrite( s[j-OFFSET], valor);
	}

	return((*puertoEntradaAC)); //return(adcRead(SENSOR_SIG));
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
	uint8_t desnivel;
	static uint16_t suma = 0;
	static uint16_t cont = 0;
	static uint16_t altura = 0;		
	
	desnivel = descartarMedida();
	if(desnivel==0){				
		altura = CalculoAlturaPasto();				
	}
	//guardo la suma de todas las aturas del pasto para poder calcular el promedio
	suma = suma + altura;		
	//calculo el promedio de la altura del pasto hasta el momento
	promedio = suma/cont;
			
	//Me fijo si ya recorri toda la distancia que se quería sensar
	if(cont == cantIter){				
		cont = 0;
		suma = 0;							
	}else{
		cont++;										
	}
		 
	return promedio;
}

void MedidaGiroscopio(float *rad_x, float *rad_y, float *rad_z){

     //Leer el sensor y guardar en estructura de control
     //mpu9250Read();

     (*rad_x)= (*lecturaGyro)[0];// = mpu9250GetGyroX_rads();
     (*rad_y)= (*lecturaGyro)[1];// = mpu9250GetGyroY_rads();
     (*rad_z)= (*lecturaGyro)[2];// = mpu9250GetGyroZ_rads();

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

// Funcion implementada para probar los test unitarios
void InicializarSensores(uint16_t  (*valor)[CANT_SENSORES]){
	
	valor_sensores = valor;		
}

// Funcion implementada para probar los test unitarios
void InicializarGyro(float (*lectura)[3]){
	
	lecturaGyro = lectura ;		
}

// Funcion implementada para probar los test unitarios
void InicializarPuertoAnalogico(uint16_t * puerto){
	
	puertoEntradaAC = puerto;		
}

// Multiplexor Computing -> Funcion implementada para probar los test unitarios
int gpio_mux(int cont, uint8_t valor_bin){

	static int control[LARGO_CANT_SENSORES_BIN];
	uint16_t valor; 

	control[cont]= valor_bin;

	if(cont == LARGO_CANT_SENSORES_BIN-OFFSET){
		if(control[3]==1){
			if(control[2]==1){				
				if(control[1]==1){
					if(control[0]==1){
						valor = (*valor_sensores)[15];
					}else{
						valor = (*valor_sensores)[14];
					}
				}else{
					if(control[0]==1){
						valor = (*valor_sensores)[13];
					}else{
						valor = (*valor_sensores)[12];
					}
				}		
               		}else{
				if(control[1]==1){
					if(control[0]==1){
						valor = (*valor_sensores)[11];
					}else{
						valor = (*valor_sensores)[10];
					}
				}else{
					if(control[0]==1){
						valor = (*valor_sensores)[9];
					}else{
						valor = (*valor_sensores)[8];
					}
				}	
			}			
		}else{
			if(control[2]==1){				
				if(control[1]==1){
					if(control[0]==1){
						valor = (*valor_sensores)[7];
					}else{
						valor = (*valor_sensores)[6];
					}
				}else{
					if(control[0]==1){
						valor = (*valor_sensores)[5];
					}else{
						valor = (*valor_sensores)[4];
					}
				}		
               		}else{
				if(control[1]==1){
					if(control[0]==1){
						valor = (*valor_sensores)[3];
					}else{
						valor = (*valor_sensores)[2];
					}
				}else{
					if(control[0]==1){
						valor = (*valor_sensores)[1];
					}else{
						valor = (*valor_sensores)[0];
					}
				}	
			}
		}   		              						
	}

	*puertoEntradaAC = valor;
}

