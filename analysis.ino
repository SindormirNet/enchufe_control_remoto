
void analyze_data(byte *dest, byte *action ) {
  byte pin=0;
  byte estado=2;
  
  if      (dest[0] == 0 && dest[1] == 1 && dest[2] == 0 && dest[3] == 0 && dest[4] == 1 && dest[5] == 1 ) pin=LED_1;
  else if (dest[0] == 0 && dest[1] == 1 && dest[2] == 1 && dest[3] == 1 && dest[4] == 0 && dest[5] == 0 ) pin=LED_2;
  else if (dest[0] == 1 && dest[1] == 1 && dest[2] == 0 && dest[3] == 0 && dest[4] == 0 && dest[5] == 0 ) pin=LED_3;

  if      (action[0] == 0 && action[1] == 0 && action[2] == 1 && action[3] == 1) estado=HIGH;
  else if (action[0] == 1 && action[1] == 1 && action[2] == 0 && action[3] == 0) estado=LOW;
  
  if (pin != 0 && estado != 2) {
    #ifdef RXDEBUG
      Serial.print(pin); 
      Serial.print(" ");
      Serial.print(estado);
    #endif
    digitalWrite(pin, estado); 
  }
}

void print_bits(byte *dato_in, byte *dest, byte *action) {
  Serial.print("[H] ");
  for (byte i = 0; i < 14; i++)
    Serial.print(dato_in[i]);

  Serial.print(" [D] ");
  for (byte i = 0; i < 6; i++)
    Serial.print(dest[i]);

  Serial.print(" [A] ");
  for (byte i = 0; i < 5; i++)
    Serial.print(action[i]);
  Serial.println(" ");
}




