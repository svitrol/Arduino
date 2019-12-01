void setup() {
 pinMode(A0,INPUT);
 pinMode(D2,OUTPUT);
 Serial.begin(115200);
}

void loop() {
  int neco=analogRead(A0);
  Serial.println(neco);
  analogWrite(D2,neco);
  // put your main code here, to run repeatedly:

}
