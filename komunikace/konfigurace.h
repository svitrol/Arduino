#ifndef LED
#define LED 2
#endif
#include "EEPROM.h"

const char* udaje[3];
char blaznivyNapad [3][32];
byte stavKomunikace=0;
byte posuv=0,velikostEpromky=99;
Ticker blinker;

void konfigurace_init(){
  Serial.begin(115200);
  //nactiUdaje()
  
  if(nactiUdaje()==1){
    pripojSa();
    wifiServer.begin();//začni server
  }
   pinMode(LED,OUTPUT);
 
    //neco jako timer interrupt ve vetřinách
    blinker.attach(1, changeState); 
}

void vycistiEepromku(){
  EEPROM.begin(velikostEpromky);
  // vycisti eepromku
  for (byte i = 0; i < velikostEpromky; i++) {
    EEPROM.write(i, '\0');
  }
  EEPROM.end();
}
byte nactiUdaje(){
  EEPROM.begin(velikostEpromky);
  posuv=0;
  byte bylo=0;
  for(byte poradi=0;poradi<3;poradi++){
    byte n=EEPROM.read(posuv++);
    if(n>0){
      bylo=1;
      for(byte i=0;i<n;i++){
        blaznivyNapad[poradi][i]=EEPROM.read(i+posuv);
      }
      udaje[poradi]=blaznivyNapad[poradi];
      posuv+=32;
      Serial.println(udaje[poradi]);
    }
    
  } 
  EEPROM.end();
  return bylo;
}
void prosteToPrecti(){
  EEPROM.begin(velikostEpromky);
  for(byte i=0;i<velikostEpromky;i++){
        Serial.print(i);
        Serial.print(": ");
        Serial.println(char(EEPROM.read(i)));
      }
      EEPROM.end();
}
void scanujSite(){
  
      Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  Serial.println();
}
void pripojSa(){
  Serial.println(udaje[1]);
  Serial.println(udaje[2]);
   WiFi.begin(udaje[1],udaje[2]);
  byte pocitadlo=10;
  while (WiFi.status() != WL_CONNECTED&&pocitadlo>0) {
    delay(1000);
    Serial.println("Connecting..");
    pocitadlo--;
  }
 
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP()); 
  
}
void changeState()
{
  if(Serial.available()>0)    //Checks is there any data in buffer 
  {
    byte n=Serial.available();
    char pole[n];
    for(byte i=0;i<n;i++){
      pole[i]=Serial.read();
    }
    pole[n-1]='\0';
    Serial.println(pole);  //Read serial data byte and send back to serial monitor
    switch(stavKomunikace){
        case 1:{
          posuv=0;
          vycistiEepromku();
          EEPROM.begin(velikostEpromky);
           //zapise na prvni pozici velikost pole jak bude velke zapis jmena zarizeni
           EEPROM.write(posuv++, n);
          for (byte i = 0; i < n; i++) {
            EEPROM.write(i+posuv, pole[i]);
            }
          EEPROM.end(); 
          posuv+=32; 
          stavKomunikace++;
          Serial.println("zadej jmeno wifi site");
          
          WiFi.mode(WIFI_STA);
          WiFi.disconnect();
  
          scanujSite();          
          break;     
        }
        case 2:{
          EEPROM.begin(velikostEpromky);
           // zapis jmena wifi
           
                EEPROM.write(posuv++, n);
                for (byte i = 0; i < n; i++) {
                EEPROM.write(i+posuv, pole[i]);
            }           
          EEPROM.end(); 
          posuv+=32;  
          stavKomunikace++;
          Serial.println("zadej heslo wifi site");
          break;
        }
        case 3:{
          EEPROM.begin(velikostEpromky);
           // zapis wifi hesla
           EEPROM.write(posuv++, n);
          for (byte i = 0; i < n; i++) {
            EEPROM.write(i+posuv, pole[i]);
            }
          EEPROM.end();  
          posuv=0; 
          stavKomunikace=0;
          Serial.println("konfigurace byla dokončena chces li ji zmenit zadej ahoj");
          nactiUdaje();
          pripojSa();
          break;
        }
      }
    if(strcmp(pole,"ahoj")==0){
      Serial.println("Co je moje jméno?");
      stavKomunikace=1;
      }
      if(strcmp(pole,"zobraz")==0){
      Serial.println(nactiUdaje());
      }
      if(strcmp(pole,"proste to precti")==0){
      prosteToPrecti();
      }
      if(strcmp(pole,"vycisti to")==0){
      vycistiEepromku();
      }
      if(strcmp(pole,"nacpi to tam na pevno")==0){
        const char* neco="2.4GHz_nech_me_byt";
        const char* peco="01189998819991197253";
        udaje[1]=neco;
        udaje[2]=peco;
      }
      if(strcmp(pole,"pripoj sa")==0){
        pripojSa();
        wifiServer.begin();//začni server
      }
      
   }
  digitalWrite(LED, !(digitalRead(LED)));  //Invert Current State of LED  
}
