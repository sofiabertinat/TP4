
#include "unity.h"
#include "proyecto.h"
#include "mock_sapi.h"

#define SENSOR_OFF 0
#define SENSOR_ON 1
#define CANT_SENSORES 16
#define ALTURA_SENSOR4 110
#define OFFSET 1
#define DESNIVEL_ON 1

/*Se define variable donde se va a emular el valor de los sensores IR de entrada analógica*/
static uint16_t lecturaAnalog_sensores[CANT_SENSORES]={0,0,0,1020,0,0,0,0,0,0,0,0,0,0,0,0};

/*Se define variable donde se va a emular el valor de la MPU9250*/
static float Gyro[3]={0, 0, 0};

void setUp(void){

}

void tearDown(void){

}

//Se puede leer lectura de un sensor IR
void test_LecturaSensorIr(void){

	const uint8_t sensor = 4;
	uint16_t lectura = 400;

	gpioWrite_ExpectAndReturn(s[0], 1, 1);
	gpioWrite_ExpectAndReturn(s[1], 1, 1);	
	gpioWrite_ExpectAndReturn(s[2], 0, 1);
	gpioWrite_ExpectAndReturn(s[3], 0, 1);
	adcRead_ExpectAndReturn(CH1, 1020);
	lectura = LecturaSensorIR(sensor);

	TEST_ASSERT_EQUAL(lecturaAnalog_sensores[sensor-OFFSET], lectura); 
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoAlturaPasto(void){

	uint16_t valor, i;

	for(i=0;i<CANT_SENSORES;i++){
		gpioWrite_IgnoreAndReturn(1);
		adcRead_ExpectAndReturn(CH1, lecturaAnalog_sensores[i]);
	}
	gpioWrite_IgnoreAndReturn(1);

	valor = CalculoAlturaPasto();

	TEST_ASSERT_EQUAL(ALTURA_SENSOR4, valor);
}


//Se puede descartar lecturas medidas
void test_DescartarMedida(void){

	uint8_t desnivel;

	Gyro[0]=0.52;
	
	mpu9250GetGyroX_rads_IgnoreAndReturn(Gyro[0]);
	mpu9250GetGyroY_rads_IgnoreAndReturn(Gyro[1]);
	mpu9250GetGyroZ_rads_IgnoreAndReturn(Gyro[2]);
	desnivel = descartarMedida();

	TEST_ASSERT_EQUAL(DESNIVEL_ON, desnivel);
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoPromedioAlturaPastoDescartarMedida(void){

	uint16_t valor = 1;
	uint16_t cantIter = 4;
	uint16_t i;

	//cantIter = DuracionSensado(10);
	for(i=1;i<cantIter+1;i++){
		mpu9250GetGyroX_rads_IgnoreAndReturn(Gyro[0]);
		mpu9250GetGyroY_rads_IgnoreAndReturn(Gyro[1]);
		mpu9250GetGyroZ_rads_IgnoreAndReturn(Gyro[2]);
		valor = CalculoPromedioAlturaPasto(cantIter);	
	}	

	TEST_ASSERT_EQUAL(0 , valor);
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoPromedioAlturaPasto(void){

	uint16_t valor = 1;
	uint16_t cantIter = 4;
	uint16_t i;
	
	Gyro[0]=0;
	//cantIter = DuracionSensado(1);
	for(i=1;i<cantIter+1;i++){
		mpu9250GetGyroX_rads_IgnoreAndReturn(Gyro[0]);
		mpu9250GetGyroY_rads_IgnoreAndReturn(Gyro[1]);
		mpu9250GetGyroZ_rads_IgnoreAndReturn(Gyro[2]);
		for(i=0;i<CANT_SENSORES;i++){
			gpioWrite_IgnoreAndReturn(1);
			adcRead_ExpectAndReturn(CH1, lecturaAnalog_sensores[i]);
		}
		valor = CalculoPromedioAlturaPasto(cantIter);	
	}	


	TEST_ASSERT_EQUAL(ALTURA_SENSOR4, valor);
}











