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

#include "config.h"
#include "ESP32_Utils_WiFi.hpp"
#include "ESP32_Utils_MQTT.hpp"

#define DHTPIN 4
#define DHTTYPE DHT22
#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  600        /* Time ESP32 will go to sleep (in seconds) */ 
#define TRIGGER_PIN 12
#define EEPROM_SIZE 12


#define CLK 22
#define DIO 23
#define VCC_D 13

TM1637Display display(CLK, DIO);

TaskHandle_t Task1;
ESP32Time rtc;


//ESP.restart();
DHT dht(DHTPIN, DHTTYPE);

RTC_DATA_ATTR int bootCount = 0; //portal cautivo

//RTC_DATA_ATTR int bootCount = 0;
int ledState = LOW;             

unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 60000;   // tiempo de adquisicion de lectura del sensor

int reseteo=0;
String messageTemp;
bool bandera=false;
String host_mac;



void DHT222_humedad(float H,float T){

  char D_humedad[18];
  dtostrf(H, 1, 5, D_humedad);
  char D_temp[18];
  dtostrf(T, 1, 5, D_temp);

  String trama_json = WiFi.macAddress() + "," + String(T,2) + "," + String(H,2);
  char enviar[50];
  trama_json.toCharArray(enviar,trama_json.length()+1);
  client.publish(t_temperatura, enviar);
  //String trama_json = "{ \"temperatura\": " + String(T,2) + ", \"humedad\": " + String(H,0) + "}";
  //String trama_json = WiFi.macAddress() + "," + String(T,2) + "," + String(H,2);
  //char enviar[50];
  //trama_json.toCharArray(enviar,trama_json.length()+1);

  //client.publish("/ha/sensor1/", "{ \"temperatura\": 21, \"humedad\": 75}");
  //client.publish(t_temperatura, enviar);
}


void loop2(void *parameter){
  delay(10000);
  reseteo = EEPROM.read(0);
  //EEPROM.write(0, 1);
  //EEPROM.write(0, 1);
  //EEPROM.commit();
  for(;;){
    //Serial.println(reseteo);
  
    //Serial.println("loop2");
   
 
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
  //pinMode(LED_BUILTIN, OUTPUT);

}

void checkButton(){
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW) {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      delay(3000); // reset delay hold
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      // start portal w delay 
    }
  }
  
}

void loop() {
  checkButton();
  if(wm_nonblocking) wm.process(); // avoid delays() in loop when non-blocking and other long running code  
  
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop(); 

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {  // intervalos de acciones sin delay
    
    previousMillis = currentMillis;

    //////encender led interno
    /*
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);
    
    */
    ////// monitoreo temperatura
     float h = dht.readHumidity();
     float t = dht.readTemperature(); 
     DHT222_humedad(h,t);
     Serial.print(F("Humedad: "));
     Serial.print(h);
     Serial.print(F("% Temperatura: "));
     Serial.print(t);
     Serial.println(F("°C "));
     int temperatura;
     temperatura = t * 10; 
     //Display the temperature in Celsius
     //display.showNumberDec(temperatura, false, 3, 0);
     //display.setSegments(Celsius, 1, 3);

    display.showNumberDecEx(temperatura,0b01000000,false,3,0);
    display.setSegments(Celsius, 1, 3);

    // Display the temperature in Celsius
     //display.showNumberDec(temperatura, false, 3, 0);
     //display.setSegments(Celsius, 1, 3);

   
    //Serial.print(host_mac[0]);
    //Serial.println(host_mac[1]);
    
    //display.showNumberHexEx(0xf1af);
    //for(int k=0; k <= 4; k++) {
		//display.showNumberDecEx(t,(0x80 >> k), false,0);
		//delay(2000);
	  //}
	
    
    }
}
