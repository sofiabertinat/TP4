
#include "unity.h"
#include "proyecto.h"

#define SENSOR_OFF 0
#define SENSOR_ON 1
#define CANT_SENSORES 16
#define ALTURA_SENSOR4 110
#define OFFSET 1
#define DESNIVEL_ON 1

/*Se define variable donde se va a emular el puerto de E/S */
static uint16_t puertoAC;

/*Se define variable donde se va a emular el valor de los sensores IR de entrada analógica*/
static uint16_t lecturaAnalog_sensores[CANT_SENSORES]={0,0,0,1020,0,0,0,0,0,0,0,0,0,0,0,0};

/*Se define variable donde se va a emular el valor de la MPU9250*/
static float Gyro[3]={0, 0, 0};

void setUp(void){

	InicializarPuertoAnalogico(&puertoAC);
	InicializarSensores(&lecturaAnalog_sensores);
	InicializarGyro(&Gyro);
}

void tearDown(void){

}

//Se puede leer lectura de un sensor IR
void test_LecturaSensorIr(void){

	const uint8_t sensor = 4;
	uint16_t lectura = 400;

	lectura = LecturaSensorIR(sensor);

	TEST_ASSERT_EQUAL(lecturaAnalog_sensores[sensor-OFFSET], lectura); 
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoAlturaPasto(void){

	uint16_t valor;

	valor = CalculoAlturaPasto();

	TEST_ASSERT_EQUAL(ALTURA_SENSOR4, valor);
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoPromedioAlturaPasto(void){

	uint16_t valor = 1;
	uint16_t cantIter = 0;
	uint16_t i;
	
	cantIter = DuracionSensado(10);
	for(i=1;i<cantIter;i++){
		valor = CalculoPromedioAlturaPasto(cantIter);	
	}	


	TEST_ASSERT_EQUAL(ALTURA_SENSOR4, valor);
}


//Se puede descartar lecturas medidas
void test_DescartarMedida(void){

	uint8_t desnivel;

	Gyro[0]=0.52;

	desnivel = descartarMedida();

	TEST_ASSERT_EQUAL(DESNIVEL_ON, desnivel);
}

//Se puede leer lectura de todos los sensores IR y calcular la altura del pasto en ese instante
void test_CalculoPromedioAlturaPastoDescartarMedida(void){

	uint16_t valor = 1;
	uint16_t cantIter = 0;
	uint16_t i;
	
	cantIter = DuracionSensado(10);
	for(i=1;i<cantIter;i++){
	//	valor = CalculoPromedioAlturaPasto(cantIter);	
	}	

	TEST_ASSERT_EQUAL(0 , valor);
}







