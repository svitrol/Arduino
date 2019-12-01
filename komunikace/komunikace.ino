#include "ESP8266WiFi.h"
#include "EEPROM.h"
#include <Ticker.h>
#include "DHTesp.h"
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>


DHTesp dht;

#define LED 2

WiFiServer wifiServer(8888);

const char* udaje[3];
char blaznivyNapad [3][32];
byte stavKomunikace = 0;
byte posuv = 0, velikostEpromky = 99;
volatile byte pocitadlo=0;

WiFiClient nakladac;
MySQL_Connection conn((Client *)&nakladac);
IPAddress server_addr(88,86,120,229);  // IP of the MySQL *server* here
int port=3310;
char MYSQL_user[] = "svitaceka";              // MySQL user login username
char MYSQL_password[] = "ef5002711";        // MySQL user login password

// Sample query
const char INSERT_DATA[] = "INSERT INTO prihlaseni.hello_sensor (zprava, vlhkost, teplota) VALUES ('%s',%1.2f,%1.2f)";



Ticker blinker;

#define Maximu_clientu 10
#define mixim_delka_lajny 50

WiFiClient *clients[Maximu_clientu] = { NULL };
char inputs[Maximu_clientu][mixim_delka_lajny] = { 0 };
unsigned int cas;

void setup() {
  konfigurace_init();
  dht.setup(D2, DHTesp::DHT11);
  blinker.attach(0.5, konfigurace);
  cas=millis();
    digitalWrite(LED, HIGH);
}



void loop() {
  praceSklienty();
  if(millis()-cas>=300000){
    uloz();
    cas=millis();
  }
  
}
//**********************************************************************************************************************************
//ulozeni do db
//**********************************************************************************************************************************
void uloz(){
    Serial.println("Connecting...");
    if (conn.connect(server_addr, port, MYSQL_user, MYSQL_password)) {
      delay(1000);
      char query[128];
      // zprava co odešlu
      MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
      // nactu hodnoty 
      float v=dht.getHumidity();
      float t=dht.getTemperature();
      sprintf(query, INSERT_DATA, "test sensor", v, t);
      Serial.println(query);
      // zapise do db
      cur_mem->execute(query);
      delete cur_mem;
      Serial.println("Data recorded.");
      pocitadlo=0;
    }
    else
      Serial.println("Connection failed.");
    conn.close();  
}
//**********************************************************************************************************************************
//prikazy od clientely
//**********************************************************************************************************************************
void praceSklienty(){
  WiFiClient client = wifiServer.available();
  if (client) {
    Serial.println("new client");
    // Find the first unused space
    for (int i = 0 ; i < Maximu_clientu ; ++i) {
      if (NULL == clients[i]) {
        clients[i] = new WiFiClient(client);
        break;
      }
    }
  }
  for (byte i = 0 ; i < Maximu_clientu ; ++i) {
    if (NULL != clients[i]) {
      if (clients[i]->connected()) {
        if (clients[i]->available() > 0) {
          byte n = clients[i]->available();
          for (byte ij = 0; ij < n; ij++) {
            inputs[i][ij] = clients[i]->read();
            Serial.printf("%d/%d: znak %c a jeho cislo %d \n", n, ij, inputs[i][ij], int(inputs[i][ij]));
          }
          if (inputs[i][n - 2] == 13 || inputs[i][n - 2] == 10) inputs[i][n - 2] = '\0';
          if (inputs[i][n - 1] == 13 || inputs[i][n - 1] == 10) inputs[i][n - 1] = '\0';
          Serial.println(inputs[i]);  //čte přicházející pakety
          zpracujPozadavek(inputs[i],i);
        }
      }
    }
  }
}
void zpracujPozadavek(char pole[mixim_delka_lajny],byte kteryKokot) {
  char array[10];
  float cislo = 5;
  if (strcmp(pole, "teplota") == 0) {
    cislo = dht.getTemperature();
    Serial.print("{\"toplotka\": ");
    Serial.println(cislo);
    sprintf(array, "%f \n", cislo);
    clients[kteryKokot]->write(array);
    Serial.println(array);
  }
  if (strcmp(pole, "vlhkost") == 0) {
    cislo = dht.getHumidity();
    Serial.print("{\"vlhkost\": ");
    Serial.println(cislo);
    sprintf(array, "%f \n", cislo);
    clients[kteryKokot]->write(array);
    Serial.println(array);
  }
}
//**********************************************************************************************************************************
//prvni kofigurace pro pripojeni
//**********************************************************************************************************************************
void konfigurace_init() {
  Serial.begin(115200);
  //nactiUdaje()

  if (nactiUdaje() == 1) {
    pripojSa();
    wifiServer.begin();//začni server
  }
  pinMode(LED, OUTPUT);

  //neco jako timer interrupt ve vetřinách
  blinker.attach(1, konfigurace);
}
void vycistiEepromku() {
  EEPROM.begin(velikostEpromky);
  // vycisti eepromku
  for (byte i = 0; i < velikostEpromky; i++) {
    EEPROM.write(i, '\0');
  }
  EEPROM.end();
}
byte nactiUdaje() {
  EEPROM.begin(velikostEpromky);
  posuv = 0;
  byte bylo = 0;
  for (byte poradi = 0; poradi < 3; poradi++) {
    byte n = EEPROM.read(posuv++);
    if (n > 0) {
      bylo = 1;
      for (byte i = 0; i < n; i++) {
        blaznivyNapad[poradi][i] = EEPROM.read(i + posuv);
      }
      udaje[poradi] = blaznivyNapad[poradi];
      posuv += 32;
      Serial.println(udaje[poradi]);
    }

  }
  EEPROM.end();
  return bylo;
}
void prosteToPrecti() {
  EEPROM.begin(velikostEpromky);
  for (byte i = 0; i < velikostEpromky; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(char(EEPROM.read(i)));
  }
  EEPROM.end();
}
void scanujSite() {

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
void pripojSa() {
  //  Serial.println(udaje[1]);
  //  Serial.println(udaje[2]);
  WiFi.begin(udaje[1], udaje[2]);
  byte pocitadlo = 10;
  while (WiFi.status() != WL_CONNECTED && pocitadlo > 0) {
    delay(1000);
    Serial.println("Connecting..");
    pocitadlo--;
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());

}
void konfigurace()
{
  if (Serial.available() > 0) //Checks is there any data in buffer
  {
    byte n = Serial.available();
    char pole[n];
    for (byte i = 0; i < n; i++) {
      pole[i] = Serial.read();
    }
    pole[n - 1] = '\0';
    Serial.println(pole);  //Read serial data byte and send back to serial monitor
    switch (stavKomunikace) {
      case 1: {
          posuv = 0;
          vycistiEepromku();
          EEPROM.begin(velikostEpromky);
          //zapise na prvni pozici velikost pole jak bude velke zapis jmena zarizeni
          EEPROM.write(posuv++, n);
          for (byte i = 0; i < n; i++) {
            EEPROM.write(i + posuv, pole[i]);
          }
          EEPROM.end();
          posuv += 32;
          stavKomunikace++;
          Serial.println("zadej jmeno wifi site");

          WiFi.mode(WIFI_STA);
          WiFi.disconnect();

          scanujSite();
          break;
        }
      case 2: {
          EEPROM.begin(velikostEpromky);
          // zapis jmena wifi

          EEPROM.write(posuv++, n);
          for (byte i = 0; i < n; i++) {
            EEPROM.write(i + posuv, pole[i]);
          }
          EEPROM.end();
          posuv += 32;
          stavKomunikace++;
          Serial.println("zadej heslo wifi site");
          break;
        }
      case 3: {
          EEPROM.begin(velikostEpromky);
          // zapis wifi hesla
          EEPROM.write(posuv++, n);
          for (byte i = 0; i < n; i++) {
            EEPROM.write(i + posuv, pole[i]);
          }
          EEPROM.end();
          posuv = 0;
          stavKomunikace = 0;
          Serial.println("konfigurace byla dokončena chces li ji zmenit zadej ahoj");
          nactiUdaje();
          pripojSa();
          break;
        }
    }
    if (strcmp(pole, "ahoj") == 0) {
      Serial.println("Co je moje jméno?");
      stavKomunikace = 1;
    }
    if (strcmp(pole, "zobraz") == 0) {
      Serial.println(nactiUdaje());
    }
    if (strcmp(pole, "proste to precti") == 0) {
      prosteToPrecti();
    }
    if (strcmp(pole, "vycisti to") == 0) {
      vycistiEepromku();
    }
    if (strcmp(pole, "nacpi to tam na pevno") == 0) {
      const char* neco = "2.4GHz_nech_me_byt";
      const char* peco = "01189998819991197253";
      udaje[1] = neco;
      udaje[2] = peco;
    }
    if (strcmp(pole, "pripoj sa") == 0) {
      pripojSa();
      wifiServer.begin();//začni server
    }

  }
  //Invert Current State of LED
}
