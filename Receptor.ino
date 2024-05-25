// SoftwareSerial - Version: Latest 
#
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

File logFile;
RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial MiSerial(2, 3); //Rx 2 - Tx 3
const int buzzer = 5;
const int prealerta = 9; const int alerta = 10;
int temperatura = 0;

void setup() {
  rtc.begin();
  SD.begin(4);
  Serial.begin(9200); MiSerial.begin(9200);
  lcd.init(); lcd.backlight();
  pinMode(prealerta, OUTPUT); pinMode(alerta, OUTPUT);
  if (!SD.exists("temp_cpd.txt"))//COMPROBAR LA EXISTENCIA DEL FICHERO
  {
    logFile = SD.open("temp_cpd.txt", FILE_WRITE);
    if (logFile) {//CREAR LAS CABECERAS DE LOS CAMPOS PARA EXCEL SI EL FICHERO NO EXISTE PREVIAMENTE
      logFile.println("FECHA,HORA,TEMPERATURA");
      logFile.close();
    } else {
      Serial.println("Error creando el archivo temp_cpd.txt");
    }
  }
/*====================================================================================*/
}

void FechaSerial(DateTime date) //PARA CONTROLAR LA SALIDA POR MONITOR SERIAL.
{
  Serial.print(date.year(), DEC); Serial.print('/'); Serial.print(date.month(), DEC); Serial.print('/'); Serial.print(date.day(), DEC); // IMPRIME aa/mm/dd
  Serial.print(" "); Serial.print(date.hour(), DEC); Serial.print(':'); Serial.print(date.minute(), DEC); Serial.print(':'); Serial.print(date.second(), DEC); //IMPRIME hh:mm:ss
  Serial.print(" "); Serial.print(temperatura); Serial.print("ºC");
  Serial.println();
}

void GrabarDatos(DateTime date, int temp)//DATOS Y FORMATO QUE SE GRABARAN EN LA SD
{
  logFile.print(date.year(), DEC);
  logFile.print('/');
  logFile.print(date.month(), DEC);
  logFile.print('/');
  logFile.print(date.day(), DEC);
  logFile.print(",");
  logFile.print(date.hour(), DEC);
  logFile.print(':');
  logFile.print(date.minute(), DEC);
  logFile.print(':');
  logFile.print(date.second(), DEC);
  logFile.print(",");
  logFile.println(temperatura);
}


void grabacion() { //APERTURA DEL FICHERO Y GRABACION DE LOS DATOS
  DateTime now = rtc.now();
  FechaSerial(now);
  logFile = SD.open("temp_cpd.txt", FILE_WRITE);
  if (logFile) {//COMPRUEBA LA EXISTENCIA DEL FICHERO ANTES DE GRABAR
    int temp = MiSerial.read();
    DateTime now = rtc.now();
    GrabarDatos(now, temp);
    logFile.close();
  }
  else {//ERROR DE APERTURA DE FICHERO
    Serial.println(F("Error al abrir el archivo"));
  }
}

void loop() {
  temperatura = MiSerial.read();
  lcd.setCursor(0, 0); lcd.print("CONTROL SALA CPD");
  lcd.setCursor(0, 1); lcd.print("Temperatura: "); lcd.print(temperatura); lcd.print("c");
    /*============================SECCION CONTROL DE ALARMAS===========================*/
  if (temperatura <= 21) {
    digitalWrite(prealerta, LOW); digitalWrite(alerta, LOW);
    noTone(buzzer);
  }
  else if (temperatura > 21 && temperatura < 26) {
    grabacion();
    digitalWrite(prealerta, HIGH); digitalWrite(alerta, LOW);
    noTone(buzzer);
  }
  else {
    digitalWrite(prealerta, HIGH); digitalWrite(alerta, HIGH);
    grabacion();
    tone(buzzer, 300); delay(200); tone(buzzer, 400); delay(200); tone(buzzer, 500); delay(200); noTone(buzzer);
  }
  delay(2000);//ESPERA ENTRE MEDICIONES EN ms 60000(1min) 3600000(1h) 7200000 (2horas) 14400000(4h)
}