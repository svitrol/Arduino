void setup() {
  Serial.begin(115200);
  pinMode(A0,OUTPUT);
  pinMode(A7,OUTPUT);
  pinMode(A5,OUTPUT);
  digitalWrite(A0,HIGH);
  digitalWrite(A7,LOW);
  digitalWrite(A5,LOW);
  pinMode(A6,INPUT);
  pinMode(A4,INPUT);

  pinMode(A1,OUTPUT);
  digitalWrite(A1,LOW);
}

void loop() {
  Serial.print("tlacitka1: ");
  Serial.println(analogRead(A6));
  Serial.print("tlacitka2: ");
  Serial.println(analogRead(A4));
  delay(100);

}
//skip + 880-889
//play 610-660
//stop 740-750
//skip- 820-830
//pause 950-970
//open 410-420
//power 20-40
