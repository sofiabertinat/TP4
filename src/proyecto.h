
//#include "sapi.h"
//#include "appMpu9250.h"
//#include "leds.h"

#include <stdint.h>

#define TIEMPO_LECTURA 5 // ms
#define CANT_SENSORES 16
#define LARGO_CANT_SENSORES_BIN 4
#define VALOR_UMBRAL 1010
//se establece que el sensor 1 esta a 50mm del suelo, y el resto distan 20mm entre sí
#define ALTURA_SENSOR_MIN 50
#define ALTURA_ENTRE_SENSORES 20
#define TIEMPO_LECTURA     5 // ms

//#define S0		     GPIO0
//#define S1		     GPIO1
//#define S2		     GPIO2
//#define S3		     GPIO3
//#define SENSOR_SIG   CH1

#define OFFSET 1
#define SENSOR_ON 1
#define SENSOR_OFF 0 

static uint8_t  estadoSensor[CANT_SENSORES] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//static gpioMap_t s[4]={S0,S1,S2,S3};


static uint16_t * puertoEntradaAC; // Variable definida para probar los test unitarios

uint16_t (*valor_sensores)[CANT_SENSORES]; // Variable definida para probar los test unitarios

float (*lecturaGyro)[3];// Variable definida para probar los test unitarios


uint16_t LecturaSensorIR(uint8_t sensor); 

uint16_t CalculoAlturaPasto(void);

uint16_t CalculoPromedioAlturaPasto(uint16_t cantIter);

uint16_t DuracionSensado(uint16_t metros);

void MedidaGiroscopio(float *rad_x, float *rad_y, float *rad_z);

uint8_t  descartarMedida( void);


void InicializarPuertoAnalogico(uint16_t * puerto); // Funcion implementada para probar los test unitarios

int gpio_mux(int cont, uint8_t  valor_bin); // Funcion implementada para probar los test unitarios

void InicializarSensores(uint16_t (*valor)[CANT_SENSORES]); // Funcion implementada para probar los test unitarios

void InicializarGyro(float (*lectura)[3]); // Funcion implementada para probar los test unitarios



