
void receive_data() {
  static unsigned long t_ini_pulso;
  static unsigned long t_fin_pulso;
  static unsigned long t_pulse;
  static byte estado_pulso = 0;
  static byte pulso_leido = 0;
  static byte n = 0;
  static byte m = 0;
  boolean header_ok = 0;

  if (digitalRead(RF_RX) && estado_pulso == 0) {
    //Inicio de pulso
    t_ini_pulso =  micros();
    estado_pulso = 1;
  }
  if (!digitalRead(RF_RX) && estado_pulso == 1) {
    //Fin del pulso
    t_fin_pulso =  micros();
    estado_pulso = 0;
    pulso_leido = 1;
  }

  if (pulso_leido == 1) {
    pulso_leido = 0;

    //Comprobacion del tamaño del pulso para saber si es un cero o un uno
    t_pulse = t_fin_pulso - t_ini_pulso;

    if ((t_pulse > (PULSE_0 - MARGEN)) && (t_pulse < (PULSE_0 + MARGEN)))  dato_in[dato_in_ptr++] = 0;
    if ((t_pulse > (PULSE_1 - MARGEN)) && (t_pulse < (PULSE_1 + MARGEN)))  dato_in[dato_in_ptr++] = 1;


    if (dato_in_ptr < 15) {
      if (dato_in[dato_in_ptr - 1] != header[dato_in_ptr - 1]) {
        delay(5); // NPI de porque hay que poner este delay aqui
        header_ok = 0;
        dato_in_ptr = 0;
        n = 0;
        m = 0;
      }
    }
    else if (dato_in_ptr < 21) {
      dest[n++] = dato_in[dato_in_ptr - 1];
    }
    else if (dato_in_ptr < 25) {
      action[m++] = dato_in[dato_in_ptr - 1];
    }
    else if (dato_in_ptr >= 25) header_ok = 1;

    if (header_ok) {
      //print_bits(dato_in, dest, action);
      analyze_data(dest, action);
      dato_in_ptr = 0;
    }
  }
}


void analyze_data(byte *dest, byte *action ) {
  // Esto no funciona bien del todo. Averiguar que esta pasando...
  Serial.print("Dest: ");
  if (dest[0] == 0 && dest[5] == 1) Serial.print("1");
  if (dest[0] == 0 && dest[5] == 0) Serial.print("2");
  if (dest[0] == 1 && dest[5] == 0) Serial.print("3");

  Serial.print(" Action: ");
  if (action[0] == 0 && action[3] == 1) Serial.println("ON");
  if (action[0] == 1 && action[3] == 0) Serial.println("OFF");

}



void print_bits(byte *dato_in, byte *dest, byte *action ) {
  Serial.print("Header found ");
  for (byte i = 0; i < 14; i++)
    Serial.print(dato_in[i]);
  Serial.println(".");

  Serial.print("Dest found ");
  for (byte i = 0; i < 6; i++)
    Serial.print(dest[i]);
  Serial.println(".");

  Serial.print("Action found ");
  for (byte i = 0; i < 4; i++)
    Serial.print(action[i]);
  Serial.println(".");
}




