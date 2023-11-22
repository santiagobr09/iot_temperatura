/////////////////////////////////
//
//
//      SENSOR DE TEMPERATURA Y HUMEDAD
//      TALLER UTN
//      2023-11-22
//
////////////////////////////////

#include <EEPROM.h>
#include <ESP32Time.h>
#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "DHT.h"
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>
#include <TM1637Display.h>

// archivos de configuracion
#include "config.h"
#include "ESP32_Utils_WiFi.hpp"
#include "ESP32_Utils_MQTT.hpp"


#define uS_TO_S_FACTOR 1000000ULL  //factor de conversion de micro segundos a segundos
#define TIME_TO_SLEEP  110        //tiempo en segundos que el ESP entra en modo sleep
#define TRIGGER_PIN 12


// pin y configuracion para sensor de temperatura
#define DHTPIN 4
#define DHTTYPE DHT22
// pines para display 7 segmentos alimentado a 3.3V
#define CLK 22
#define DIO 23
#define VCC_D 13

TM1637Display display(CLK, DIO);

TaskHandle_t Task1;
ESP32Time rtc;


DHT dht(DHTPIN, DHTTYPE);

RTC_DATA_ATTR int bootCount = 0; //portal cautivo

//RTC_DATA_ATTR int bootCount = 0;
int ledState = LOW;             

unsigned long previousMillis = 0;        
const long interval = 60000;   // tiempo de adquisicion de lectura del sensor

int reseteo=0;
String messageTemp;
bool bandera=false;
String host_mac;



void DHT22_envio(float H,float T){

  char D_humedad[18];
  dtostrf(H, 1, 5, D_humedad);
  char D_temp[18];
  dtostrf(T, 1, 5, D_temp);

  String trama_json = WiFi.macAddress() + "," + String(T,2) + "," + String(H,2);
  char enviar[50];
  trama_json.toCharArray(enviar,trama_json.length()+1);
  client.publish(t_temperatura, enviar);   // envio de datos MQTT

}


void loop2(void *parameter){

  for(;;){
    digitalWrite(ledPin,HIGH);
    delay(500);
    digitalWrite(ledPin,LOW);
    delay(500);
 
   vTaskDelay(10);
  }
  
  
  
}

void setup() {
  xTaskCreatePinnedToCore(
    loop2,
    "Task_1",
    1000,
    NULL,
    1,
    &Task1,
    0);

  delay(2000);
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("Inicio...");

  digitalWrite(VCC_D,HIGH);
  display.setBrightness(7);
  display.clear();
  Serial.println("PANTALLA...");
  display.showNumberDec(8888);
  delay(2000);
  dht.begin();
  
  //configuracion tiempo de hibernacion
  // esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

  /// ESP32 en modo AP para realizar la primera conexion
  inicializar_conexion();

  EEPROM.begin(EEPROM_SIZE);

  pinMode(ledPin, OUTPUT);
  pinMode(VCC_D,OUTPUT);
  pinMode(TRIGGER_PIN, INPUT_PULLUP);

  delay(2000);
    
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  client.connected();
  

}

void checkButton(){
  // verificar si el boton de reset se presiona
  if ( digitalRead(TRIGGER_PIN) == LOW) {
      delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Boton Presionado");
      delay(3000); 
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Borrando configuracion y reiniciando...");
        wm.resetSettings();
        ESP.restart();
      }
      // Inicia en modo portal cautivo
    }
  }
  
}

void loop() {
  checkButton();
  if(wm_nonblocking) wm.process(); 
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop(); 

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {  // intervalos de acciones sin delay
    
    previousMillis = currentMillis;

    ////// monitoreo temperatura
     float h = dht.readHumidity();
     float t = dht.readTemperature(); 
     DHT22_envio(h,t);
     Serial.print(F("Humedad: "));
     Serial.print(h);
     Serial.print(F("% Temperatura: "));
     Serial.print(t);
     Serial.println(F("°C "));
     int temperatura;
     temperatura = t * 10; 

    display.showNumberDecEx(temperatura,0b01000000,false,3,0);
    display.setSegments(Celsius, 1, 3);

 
    }
    //esp_deep_sleep_start(); activar modo hibernacion
}
