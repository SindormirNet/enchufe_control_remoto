byte process_server() {
  
  //El tipo de peticion que se espera es:
  // http://IP/X_
  // siendo _ alguno de los valores A,B,C (encender) o a,b,c (apagar)
  
  char param=0, c;

  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        c = client.read();
        Serial.write(c);

        if (c == 'X') {
          param = client.read();
          Serial.print("Parametro recibido ");
          Serial.write(param);
        }

        if (c == '\n' && currentLineIsBlank) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.print("Parametro recibido: ");
          client.println(param);
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    delay(1);
    client.stop();
    
  }
  return param;
}
