
void receive_data_work() {

  static unsigned long dato_in = 0;
  static byte dato_in_idx = 0;

  static unsigned long t_ini_pulso;
  static unsigned long t_fin_pulso;
  unsigned long t_pulse;
  static byte estado_pulso = 0;
  static byte pulso_leido = 0;
  boolean header_ok = 0;

  if (digitalRead(RF_RX)  && estado_pulso == 0) { //Inicio de pulso
    t_ini_pulso =  micros();
    pulso_leido = 0;
    estado_pulso = 1;
  }

  if (!digitalRead(RF_RX) && estado_pulso == 1) { //Fin del pulso
    t_fin_pulso =  micros();
    pulso_leido = 1;
    estado_pulso = 0;
  }


  if (pulso_leido) {
    pulso_leido = 0;

    t_pulse = t_fin_pulso - t_ini_pulso;

    if      ((t_pulse > (PULSE_0 - MARGEN_0)) && (t_pulse < (PULSE_0 + MARGEN_0)))  dato_in = (dato_in << 1) & 0xFFFFFFFE; //Guardamos un cero
    else if ((t_pulse > (PULSE_1 - MARGEN_1)) && (t_pulse < (PULSE_1 + MARGEN_1)))  dato_in = (dato_in << 1) | 0x00000001; //Guardamos un uno
    else {
      dato_in = 0;
      dato_in_idx = 0;
      return;
    }


    //Debug. Muestra si hemos recibido un uno o un cero
    /*if (dato_in[dato_in_idx]) {
      digitalWrite(DEBUG, HIGH);
      delayMicroseconds(100);
      digitalWrite(DEBUG, LOW);
    }
    else {
      digitalWrite(DEBUG, HIGH);
      delayMicroseconds(25);
      digitalWrite(DEBUG, LOW);
    }*/


    dato_in_idx++;

    Serial.print(dato_in_idx);
    Serial.print(" ");

    /*delayMicroseconds(700 - t_pulse - (micros() - t_fin_pulso)); // Muestra cuando deberia comenzar el siguiente pulso
    digitalWrite(DEBUG, HIGH);
    delayMicroseconds(10);
    digitalWrite(DEBUG, LOW);*/


    if (dato_in_idx < 16) { //Estamos en la cabecera
      //                                            (rx.header >> (14-i)) & 0x01);
      if (((dato_in >> (dato_in_idx)) & 0x01) != ((rx.header >> (14 - dato_in_idx)) & 0x01)) {
#ifdef RXDEBUG
        Serial.print("Comparando ");
        Serial.print((dato_in >> (dato_in_idx - 1)) & 0x01);
        Serial.print(" ");
        Serial.print((dato_in >> (dato_in_idx)) & 0x01);
        Serial.print(" ");
        Serial.print((dato_in >> (dato_in_idx + 1)) & 0x01);
        Serial.print(" ");
        Serial.print(" con ");
        Serial.println((rx.header >> (14 - dato_in_idx)) & 0x01);
#endif
        delay(5); // NPI de porque hay que poner este delay aqui
        header_ok = 0;
        dato_in_idx = 0;
      }
    }
    else if (dato_in_idx < 21) { //Estamos en el destino
#ifdef RXDEBUG
      Serial.print('d');
#endif
      dest[dato_in_idx - 15] = (dato_in >> (dato_in_idx)) & 0x01;
    }
    else if (dato_in_idx < 25) { //Estamos en la accion
#ifdef RXDEBUG
      Serial.print('a');
#endif
      action[dato_in_idx - 21] = (dato_in >> (dato_in_idx)) & 0x01;
    }

    if (dato_in_idx >= 25) { //Hemos leido el paquete completo
#ifdef RXDEBUG
      Serial.print (' ');
#endif
      analyze_data(dest, action);

#ifdef RXDEBUG
      //print_bits(dato_in, dest, action);
#endif
      dato_in = 0;
      dato_in_idx = 0;
      delay(1);
    }

    delayMicroseconds(700 - t_pulse - (micros() - t_fin_pulso)); // Esto es opcional ¿Puede que quite un poco de ruido?
  }
}


