#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "2.4GHz_nech_me_byt";
const char* password =  "01189998819991197253";

const uint16_t port = 8888;
const char * host = "192.168.1.102";

const long utcOffsetInSeconds = 3600;

const char* daysOfTheWeek[7] = {"Neděle", "Pondělí", "Úterý", "Středa", "Čtvrtek", "Pátek", "Sobota"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiClient client;
void setup() {
    Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());
   if (!client.connect(host, port)) {

        Serial.println("Connection to host failed");

        delay(1000);
        return;
    }

    Serial.println("Connected to server successful!");
    client.print("t:v\n");
    timeClient.begin();
  
}



void loop() {
  client.print("t:v\n");
  if(client.available()){
    int n=client.available();
    for(int i=0;i<n;i++){
      char znacka=client.read();
      Serial.print(znacka);
    }
    Serial.println();
    
  }
    timeClient.update();

  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());
  //Serial.println(timeClient.getFormattedTime());

  delay(1000);

}
/*void pocitadlo_hodin() {
	if (++sekundy >= 60) {
		sekundy = 0;
		if (++minuty >= 60) {
			minuty = 0;
			if (++hodiny >= 24) {
				hodiny = 0;
			}
			if (WiFi.status() == WL_CONNECTED) {
				timeClient.update();
				hodiny = timeClient.getHours();
				minuty = timeClient.getMinutes();
				sekundy = timeClient.getSeconds();
				Serial.printf("#cas je updatnuty: %d:%d:%d",hodiny,minuty,sekundy);
			}

		}
	}
}*/