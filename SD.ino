void SDStart(){
  int i = 0;
  if (!SD.begin(4)) Serial.println("Erro, SD não pode ser inicializado");

  for(i=0;i<1000;i++){ 
    sprintf(file_name,"FILE%03d.txt",i);   
    if (!SD.exists(file_name)) break;
  }
  if(i>=1000) Serial.println("Erro, nome válido não disponível");
  else{
    if(i==0) sprintf(file_name,"FILE000.txt");
    else{ 
      sprintf(file_name,"FILE%03d.txt",i);
      SD.remove(file_name);
      if (SD.exists(file_name)) sprintf("Erro ao tentar apagar o arquivo FILE%03d.txt",i); 
    } 
    myFile = SD.open(file_name, FILE_WRITE); myFile.close();
  }
  Serial.println(file_name); 
  SDHeader();
}

void SDHeader(){
    myFile.print("N");          myFile.print("\t"); 
    myFile.print("Texp");       myFile.print("\t"); 
    myFile.print("dt");         myFile.print("\t"); 
    myFile.print("Ti");         myFile.print("\t"); 
    myFile.print("Tx");         myFile.print("\t"); 
    myFile.print("i");          myFile.print("\t\t");
    myFile.print("V");          myFile.print("\t\t"); 
    myFile.print("Taxa");       myFile.println("\t\t"); 
    myFile.print("Pote");       myFile.println("\t\t"); 
    myFile.println();
}

void SDSave(){
  myFile = SD.open(file_name, FILE_WRITE);
  if (myFile){
    myFile.print(loop_count);    myFile.print("\t"); 
    myFile.print(texp);          myFile.print("\t"); 
    myFile.print(dt);            myFile.print("\t"); 
    myFile.print(Ti,1);          myFile.print("\t"); 
    myFile.print(Tx,1);          myFile.print("\t"); 
    myFile.print(v[0],6);        myFile.print("\t"); 
    myFile.print(v[1],6);        myFile.print("\t"); 
    myFile.print(taxa,2);        myFile.print("\t"); 
    myFile.print(potencia,4);    myFile.print("\t"); 
    myFile.print(pwm);
    myFile.println();
    myFile.close(); 
  }
  else if(loop_count==0){Serial.println("Error SD: data will not be saved");}
}

void SDDilatometer(){
  myFile = SD.open(file_name, FILE_WRITE);
  if (myFile){
    myFile.print(loop_count);    myFile.print("\t"); 
    myFile.print(texp);          myFile.print("\t"); 
    myFile.print(dt);            myFile.print("\t"); 
    myFile.print(Ti,1);          myFile.print("\t"); 
    myFile.print(Tx,1);          myFile.print("\t"); 
    myFile.print(taxa,2);        myFile.print("\t"); 
    myFile.println();
    myFile.close(); 
  }
  else if(loop_count==0){Serial.println("Error SD: data will not be saved");}
}
