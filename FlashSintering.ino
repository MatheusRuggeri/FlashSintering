#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Nanoshield_ADC.h>
#include "Nanoshield_Termopar.h"
#define MAXPOT 40

// --------------------  User defined values -----------------

#define nchannels         4         // Num de canais para ser lido; n = 0, 1, 2, 3 ou 4
#define max_nloops        0         // Num de loops

// ---------------------- Global variables -------------------

File myFile;
char file_name[12];
static unsigned long tTaxa100, tTaxa100old;   // Auxiliar para calcular a taxa de aquecimento (ms)
float TmOld = 0, Tm = 0;         // Temperatura para calcular a taxa de aquecimento
float TxOld = 0;         // Temperatura para calcular a taxa de aquecimento
float taxa;                   // Taxa de aquecimento em ºC/min
Gain_t gain[4];               // ADC gain
static unsigned int loop_count=0;  // loop counter
static unsigned long texp;    // Total experiment ellapsed time (ms)
static unsigned long texp0;   // Experiment start time (ms)
unsigned long dt;             // one loop cycle elapsed time
float Ti = 0, Tx = 0;         // Temperature value sum
double v[4];                   // Voltage value sum
unsigned long tempo;             // Sampling start time
int i = 0, n = 1, pwm = 128;
float potencia;
unsigned long t0;                         // Sampling start time
int sensorPin = A1;
int sensorValue = 0;


// ----------------- Thermocouple ----------------------------

// Termopar Nanoshield on CS pin D8, type K thermocouple
Nanoshield_Termopar tc(8, TC_TYPE_K, TC_AVG_OFF);

// ---------------- ADC - 4 x 16-bit -------------------------

Nanoshield_ADC adc;

// -----------------------------------------------------------

void setup(){
  Serial.begin(9600);
  Serial.println("Iniciando...");
  SDStart();
  
    
  tc.begin();
  adc.begin();

  // Set ADC sample rate. Possible values are: 8, 16, 32, 64, 128, 250, 475, 860 - If not one of the above, the closest lower value will be selected.
  adc.setSampleRate(860);

  pinMode(6,OUTPUT); 
  pinMode(sensorPin,INPUT); 
  digitalWrite(6,LOW);
  tempo = millis();
  
  texp0 = t0;
  texp = 0; 
  dt=0;
  gain[0] = GAIN_ONE;
  gain[1] = GAIN_TWOTHIRDS;
  gain[2] = GAIN_SIXTEEN;
  gain[3] = GAIN_SIXTEEN;
  
  //dilatometriaOptica();
  
  printHeaderJM();
}

// -----------------------------------------------------------

void loop(){ 
  //if(loop_count>max_nloops && max_nloops !=0){printLimite(); reset();}
  
  t0 =  millis();
  dt=t0-texp; 
  texp=t0-texp0;

  for(i=0;i<4;i++) v[i]=0;

  for(n=0;n<10;n++){
    tc.read();  // Read thermocouple data
    if (tc.hasError()==0){
      Ti += tc.getInternal();
      Tx += tc.getExternal(); 
    }
    for (i=0;i<nchannels;i++){   // Read voltages in Volts
      adc.setGain(gain[i]);
      v[i] += adc.readVoltage(i); 
    }
  }

  if(n>0){Ti = Ti/n; Tx = Tx/n;}
  else {Ti = Tx = 0;}
  for(i=0;i<nchannels;i++){v[i]=v[i]/n;}
  v[0] = -v[0];
  v[1] = v[1]*101000/1000; //TEÓRICO
  //v[1] = v[1]*101292/872;  //REAL
  potencia = v[0]*v[1];

  if(v[0] >= 0.1){digitalWrite(6,HIGH); pwm = 0;}
  /**if(potencia >= MAXPOT){
    pwm += 10;  
    if(potencia >= 1 * MAXPOT){pwm += 100;} //queda rápida 
    if(potencia >= 1.5 * MAXPOT){pwm += 30;} //queda rápida 
    if(potencia >= 2 * MAXPOT){pwm += 40;} //queda rápida 
    if(potencia >= 2.5 * MAXPOT){pwm += 50;} //queda rápida 
    if(potencia >= 3 * MAXPOT){pwm += 60;} //queda rápida 
    if(potencia >= 3.5 * MAXPOT){pwm += 70;} //queda rápida 
    if(potencia >= 4 * MAXPOT){pwm += 80;} //queda rápida 
    if(potencia >= 5 * MAXPOT){pwm += 100;} //queda rápida 
    if(pwm > 255) pwm=255;
    analogWrite(6,pwm);
  }
  else{
    pwm -= 10;
    if(pwm < 0) pwm=0;
    analogWrite(6,pwm);
  }*/
  
  Serial.print(loop_count);    Serial.print("\t"); 
  Serial.print(texp);          Serial.print("s\t"); 
  Serial.print(dt);            Serial.print("s\t"); 
  Serial.print(Ti,1);          Serial.print("ºC\t"); 
  Serial.print(Tx,1);          Serial.print("ºC\t"); 
  Serial.print(v[0],6);        Serial.print(" A\t"); 
  Serial.print(v[1],6);        Serial.print(" V\t"); 
  Serial.print(taxa,2);        Serial.print(" ºC/min \t"); 
  Serial.print(potencia,4);    Serial.print(" W \t"); 
  Serial.print(pwm);
  Serial.println();
  
  if(loop_count%10 == 0) {
    TmOld = Tm; 
    Tm = Tx; 
    tTaxa100old = tTaxa100;
    tTaxa100 = texp;
    taxa = (60000*(Tm-TmOld))/(tTaxa100 - tTaxa100old);
  }
    
  SDSave();

  loop_count++;
  texp += texp0;
}
