#define timeSeconds 5

// Set GPIOs for LED and PIR Motion Sensor
const int motionSensor = D8;
bool aktualniStav=false;


// Checks if motion was detected, sets LED HIGH and starts a timer
ICACHE_RAM_ATTR void detectsMovement() {
  bool stav=digitalRead(motionSensor);
  if(aktualniStav!=stav){
    aktualniStav=stav;
    Serial.print("zmena:    ");
    Serial.println(aktualniStav);
  }
  //Serial.println("sem tu");
  
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(motionSensor, INPUT_PULLUP);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(motionSensor), konciPohyb, FALLING);
	Serial.println("hotovo");
}

void loop() {
}
