#include <ESP8266WiFi.h>

#define LED 2  //On board LED

//=======================================================================
void ICACHE_RAM_ATTR onTimerISR(){
    digitalWrite(LED,!(digitalRead(LED)));  //Toggle LED Pin
    //timer1_write(5000000);//12us
}
//=======================================================================
//                               Setup
//=======================================================================
void setup()
{
    Serial.begin(115200);
    Serial.println("");

    pinMode(LED,OUTPUT);
    timer1_attachInterrupt(onTimerISR);
    timer1_enable(TIM_DIV16, TIM_EDGE, TIM_LOOP);
    timer1_write(5000000); //1 000 000 us
}
//=======================================================================
//                MAIN LOOP
//=======================================================================
void loop()
{
}
//=======================================================================
