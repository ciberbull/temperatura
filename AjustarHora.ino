/* ################## Test RTC DS3231 #########
    PUESTA EN HORA, EJECUTAR EL MONITOR SERIAL
      Y ENVIAR EL CARACTER 'A' MAYUSCULA
* #############################################*/

// valores a introducir para configurar el RTC
// ===========================
int ano = 2019; // 0-2099
int mes = 10; // 0-12
int dia = 28; // 0-31
int hora= 13; // 0-23
int min = 13; // 0-59
int seg = 00; // 0-59
// ===========================

#include <Wire.h> // Comunicacion I2C 
#include "RTClib.h" // libreria ADAFRUIT para DS3231 

RTC_DS1307 RTC; // creamos el objeto RTC

String dia_semana[]={"Domingo","Lunes","Martes","Miercoles","Jueves","Viernes","Sabado"};
String newdate = "";


void setup() {

 Serial.begin(9600);
// Wire.begin(); // Inicia Wire sÃ³lo si no se hace dentro de la librerÃ­a 
// supone que se usa Wire para comunicar con otros dispositivos, no sÃ³lo con el DS3231

 if (!RTC.begin()) {
 
 Serial.println("No se encuentra RTC");
 while (1);

 }else{

 Serial.println("########################################"); 
 Serial.println("Puesta en hora del modulo RTC DS3231");
 Serial.print("Fecha actual: ");
 print_time(); // imprime hora actual del RTC
 Serial.println("########################################"); 
 Serial.println(""); 
 Serial.println("Pulsa ENTER para actualizar a"); 
 newdate = "Fecha Nueva-> "
 + String(dia) + "/"
 + String(mes) + "/"
 + String(ano) + " Hora-> "
 + String(hora)+ ":"
 + String(min) + ":"
 + String(seg);
 Serial.println(newdate);
 
 Serial.println("======================================"); 
 }
}

void loop () {

 if(Serial.available()) {
 int inputByte = Serial.read(); // lee byte entrante
 
 if (inputByte==65) { // caracter A
 RTC.adjust(DateTime(ano, mes, dia, hora, min, seg));
 Serial.print("Nueva Fecha actualizada: ");
 print_time(); // imprime hora actual del RTC 
 }
 }
}

void print_time() {
 
 DateTime ahora = RTC.now(); // captura valores del tiempo
 Serial.print(ahora.day(), DEC);
 Serial.print('/');
 Serial.print(ahora.month(), DEC);
 Serial.print('/');
 Serial.print(ahora.year(), DEC);

 Serial.print(" (");
 Serial.print(dia_semana[ahora.dayOfTheWeek()]);
 Serial.print(") ");

 if (ahora.hour() <10) Serial.print (0); 
 Serial.print(ahora.hour(), DEC);
 Serial.print(':');
 if (ahora.minute() <10) Serial.print (0); 
 Serial.print(ahora.minute(), DEC);
 Serial.print(':');
 if (ahora.second() <10) Serial.print (0); 
 Serial.print(ahora.second(), DEC);
 Serial.println();
 
}