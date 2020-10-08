void dilatometriaOptica(){
  Serial.print("N");          Serial.print("\t"); 
  Serial.print("Texp");       Serial.print("\t"); 
  Serial.print("dt");         Serial.print("\t"); 
  Serial.print("Ti");         Serial.print("\t"); 
  Serial.print("Tx");         Serial.print("\t"); 
  Serial.print("Taxa");       Serial.println("\t\t"); 
  
  myFile.print("N");          myFile.print("\t"); 
  myFile.print("Texp");       myFile.print("\t"); 
  myFile.print("dt");         myFile.print("\t"); 
  myFile.print("Ti");         myFile.print("\t"); 
  myFile.print("Tx");         myFile.print("\t"); 
  myFile.print("Taxa");       myFile.println("\t\t"); 
  myFile.println();
    
  while(1){
    sensorValue = analogRead(sensorPin);
    if(sensorValue > 500) {
      fotoLeitura();
      delay(1000); //Evita captura dupla
    }
  }
}

void fotoLeitura(){  
  t0 =  millis();
  dt=t0-texp; 
  texp=t0-texp0;

  for(n=0;n<10;n++){
    tc.read();  // Read thermocouple data
    if (tc.hasError()==0){
      Ti += tc.getInternal();
      Tx += tc.getExternal(); 
    }
  }

  Ti = Ti/n; 
  Tx = Tx/n;
  
  taxa = (60000*(Tx-TxOld))/(dt);
  TxOld = Tx;
  
  Serial.print(loop_count);    Serial.print("\t"); 
  Serial.print(texp);          Serial.print("s\t"); 
  Serial.print(dt);            Serial.print("s\t"); 
  Serial.print(Ti,1);          Serial.print("ºC\t"); 
  Serial.print(Tx,1);          Serial.print("ºC\t"); 
  Serial.print(taxa,2);        Serial.print(" ºC/min \t"); 
  Serial.println();
  
  SDDilatometer();

  loop_count++;
  texp += texp0;
}
