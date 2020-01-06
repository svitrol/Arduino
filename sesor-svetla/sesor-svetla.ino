// Měření intenzity světla BH1750

// připojení potřebných knihoven
#include <Wire.h>
#include <BH1750.h>
// inicializace senzoru BH1750 z knihovny
BH1750 luxSenzor;

void setup() {
  // komunikace po sériové lince rychlostí 9600 baud
  Serial.begin(115200);
  // zapnutí komunikace se senzorem BH1750
  luxSenzor.begin();
  // pauza před zahájením měření
  delay(200);
}

void loop() {
  // vytvoření proměnné pro uložení naměřených údajů
  // a načtení aktuální intenzity světla
  uint16_t lux = luxSenzor.readLightLevel();
  // vytištění výsledku po sériové lince
  Serial.println(lux); 
  // pauza po dobu 1 vteřiny před dalším měřením
  delay(200);
}
