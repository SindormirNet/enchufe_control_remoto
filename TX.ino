
void transmit_data(byte dst, byte act) {
  byte header[14] = {0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1};
  byte dest[3][6] = { 0, 1, 0, 0, 1, 1,
                      0, 1, 1, 1, 0, 0,
                      1, 1, 0, 0, 0, 0 }; 

  byte action[2][5] = { 1, 1, 0, 0, 0,
                        0, 0, 1, 1, 0 };

  for (byte j = 0; j < 20; j++) {
    for (byte i = 0; i < 14; i++)
      tx(header[i]);
    for (byte i = 0; i < 6; i++)
      tx(dest[dst-1][i]);
    for (byte i = 0; i < 5; i++)
      tx(action[act][i]);
    delay(6);
  }
}

void tx(byte data) {
  if (data) {
    digitalWrite(RF_TX, HIGH);
    delayMicroseconds(530);
    digitalWrite(RF_TX, LOW);
    delayMicroseconds(740 - 530);
  }
  else {
    digitalWrite(RF_TX, HIGH);
    delayMicroseconds(170);
    digitalWrite(RF_TX, LOW);
    delayMicroseconds(740 - 170);
  }
}

