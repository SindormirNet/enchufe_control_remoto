/*
           xxxxxxxxxxxxxx DDDDDD AAAAA
1 ON    -> 00000100010101 010011 00110
1 OFF   -> 00000100010101 010011 11000

2 ON    -> 00000100010101 011100 00110
2 OFF   -> 00000100010101 011100 11000

3 ON    -> 00000100010101 110000 00110
3 OFF   -> 00000100010101 110000 11000

Destinos:
    010011 -> 1
    011100 -> 2
    110000 -> 3

xxxxxxxx -> Cabecera
AAAA -> Acción: 0011 -> ON; 1100 -> OFF
DDDDDDD -> Dirección

http://www.alibaba.com/product-detail/Remote-control-socket_767648913.html

*/

#define RXDEBUG
#undef RXDEBUG

#define RF_RX 2
#define RF_TX 3
#define LED_1 10
#define LED_2 11
#define LED_3 12
#define DEBUG 13

// Modificarlo para que haga unso de las interrupciones externas con conteo de tiempo
// Hacer varias capturas (10* intervalo de tiempo) y promediar, para eliminar el ruido
// Convertir a bits las capturas para ahorrar RAM
// Crear una estructura con la informacion de la trama

//Desde un pulso al siguiente hay 760us

typedef struct {             
  int header;                
  byte destination;
  byte action;
} trama_RF;

trama_RF rx;

#define PULSE_0 170   //Valores obtenidos del uC
#define PULSE_1 530
#define MARGEN_0 70
#define MARGEN_1 70
                    // 13,12,11,10,9, 8, 7, 6, 5, 4, 3, 2 ,1, 0
const byte header[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
byte dest[6];
byte action[4];

void setup() {  
  rx.header = 0x0115;

  Serial.begin(115200);
  Serial.println("V0.36");
  pinMode(RF_RX, INPUT);
  pinMode(RF_TX, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(DEBUG, OUTPUT);
}

void loop() {
  receive_data_funciona();
  receive_data_work();
  
  if (Serial.available()){
    if (Serial.read() == 'A') {
      transmit_data(3, 1);
      Serial.println("Encendido");
    }
    else {
      transmit_data(3, 0);
      Serial.println("Apagado");
    }
  } 
}
