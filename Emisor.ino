#include "DHT.h"
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial MiSerial(2, 3); //Rx 2 - Tx 3
DHT dht(4, DHT22);
void setup() {
  Serial.begin(9200);
  MiSerial.begin(9200);
  lcd.init(); lcd.backlight();
  dht.begin();
  int temperatura = 0;
}
void loop() {
  int temperatura = dht.readTemperature();
  int humedad = dht.readHumidity();
  MiSerial.write(temperatura);
  Serial.println(temperatura);
  lcd.setCursor(0, 0);
  lcd.print("Temperatura: "); lcd.print(temperatura); lcd.print("c");
  lcd.setCursor(0, 1);
  lcd.print("Humedad: "); lcd.print(humedad); lcd.print("%");
  delay(1000);
}