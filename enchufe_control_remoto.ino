#include <SPI.h>
#include <Ethernet.h>

#define RF_RX 5
#define RF_TX 9
#define LED_1 A0
#define LED_2 A2
#define LED_3 A5
#define DEBUG A3

#define PULSE_0 170   //Valores obtenidos del uC
#define PULSE_1 530
#define MARGEN_0 70
#define MARGEN_1 70

#define RXDEBUG
#undef RXDEBUG

// TODO:
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

// 13,12,11,10,9, 8, 7, 6, 5, 4, 3, 2 ,1, 0
const byte header[] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
byte dest[6];
byte action[4];

byte mac[] = { 0xCA, 0xFE, 0xCA, 0xFE, 0x00, 0x01 };
IPAddress ip(192, 168, 1, 2);

EthernetServer server(80);

void setup() {
  rx.header = 0x0115;

  Serial.begin(115200);
  Serial.println("V0.36");

  Ethernet.begin(mac, ip);
  server.begin();

  pinMode(RF_RX, INPUT);
  pinMode(RF_TX, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(DEBUG, OUTPUT);
}

void loop() {
  byte dato;

  for (unsigned int i = 0; i < 65535; i++) receive_data_funciona();

  dato = process_server();

  switch (dato) {
    case 'A':
      digitalWrite(LED_1, HIGH);
      transmit_data(1, 1);
      Serial.println("1 Encendido");
      break;
    case 'a':
      digitalWrite(LED_1, LOW);
      transmit_data(1, 0);
      Serial.println("1 Apagado");
      break;
    case 'B':
      digitalWrite(LED_2, HIGH);
      transmit_data(2, 1);
      Serial.println("2 Encendido");
      break;
    case 'b':
      digitalWrite(LED_2, LOW);
      transmit_data(2, 0);
      Serial.println("2 Apagado");
      break;
    case 'C':
      digitalWrite(LED_3, HIGH);
      transmit_data(3, 1);
      Serial.println("3 Encendido");
      break;
    case 'c':
      digitalWrite(LED_3, LOW);
      transmit_data(3, 0);
      Serial.println("3 Apagado");
      break;
  }

}
