/*
                                   xxxxxxxxxxxxxxxDD DDDDAAAA xxxxxxxx
1 ON    -> [00] {25} 04 55 33 00 : 00000100 01010101 00110011 00000000
1 OFF   -> [00] {25} 04 55 3c 00 : 00000100 01010101 00111100 00000000

2 ON    -> [00] {25} 04 55 c3 00 : 00000100 01010101 11000011 00000000
2 OFF   -> [00] {25} 04 55 cc 00 : 00000100 01010101 11001100 00000000

3 ON    -> [00] {25} 04 57 03 00 : 00000100 01010111 00000011 00000000
3 OFF   -> [00] {25} 04 57 0c 00 : 00000100 01010111 00001100 00000000


Destinos:
    010011 -> 1
    011100 -> 2
    110000 -> 3

xxxxxxxx -> Cabecera o final
AAAA -> Acción: 0011 -> ON; 1100 -> OFF
DDDDDDD -> Dirección

*/

#define RF_RX 2
#define LED_DEBUG 13
#define RF_TX 3

//Modificarlo para que haga unso de las interrupciones externaws con conteo de tiempo
// ramiro sugiere hacer varias capturas (10* intervalo de tiempo) y promediar, para eliminar el ruido

//Pulso corto es de 200uS
//Pulso largo es de 590uS
#define PULSE_0 108
#define PULSE_1 470
#define MARGEN 100

const byte header[14] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
byte dest[6];
byte action[4];

byte dato_in[256];
byte dato_in_ptr = 0;

void setup() {
  Serial.begin(9600);
  pinMode(RF_RX, INPUT);
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(RF_TX, OUTPUT);

}

void loop() {
  receive_data();
}


