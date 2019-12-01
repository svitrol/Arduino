#include "ESP8266WiFi.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.P

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop()
{
  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.println(WiFi.SSID(i));
  }
  Serial.println("konec cyklu");

  delay(5000);
}
