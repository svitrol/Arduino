#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#ifndef STASSID
#define STASSID "2.4GHz_nech_me_byt"
#define STAPSK  "01189998819991197253"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;


IPAddress server_addr(88,86,120,229);  // IP of the MySQL *server* here
char MYSQL_user[] = "svitaceka";              // MySQL user login username
char MYSQL_password[] = "ef5002711";        // MySQL user login password

// Sample query
char INSERT_DATA[] = "INSERT INTO prihlaseni.hello_sensor (zprava, vlhkost, teplota) VALUES ('%s',%d,%s)";
char query[128];
char temperature[10];


WiFiClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("Connecting...");
  if (conn.connect(server_addr, 3310, MYSQL_user, MYSQL_password)) {
    delay(1000);
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    dtostrf(50.125, 1, 1, temperature);
    sprintf(query, INSERT_DATA, "test sensor", 24, temperature);
    // Execute the query
    cur_mem->execute(query);
    // Note: since there are no results, we do not need to read any data
    // Deleting the cursor also frees up memory used
    delete cur_mem;
    Serial.println("Data recorded.");
  }
  else
    Serial.println("Connection failed.");
  conn.close();
}

void loop() {
  // put your main code here, to run repeatedly:

}
