#pragma once

//#include "config.h"

WiFiClient espClient; //Inicializacion del cliente Wifi
PubSubClient client(espClient); //nombre del cliente mqtt
unsigned long lastMsg = 0; //Variable para almacenar mensajes recibidos
#define MSG_BUFFER_SIZE  (50) //tamaño maximo del buffer para recepcion de mensajes
char msg[MSG_BUFFER_SIZE]; //Asigna un tamaño de mensaje
int value = 0; //variable "para comparaciones logicas


void callback(char* topic, byte* payload, unsigned int length) {
  int i = 0;
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    msg[i] = payload[i];
  }
  
  msg[i] = '\0';
  Serial.println();
  
 //////// MANEJO DE DATOS RECIBIDOS SEGUN EL TOPICO PARA REALIZAR ACCIONES
  
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Esperando conexion MQTT...");
    if (client.connect(WiFi.macAddress().c_str())) {
      Serial.println("conectado...");
      client.subscribe(t_temperatura); // suscribe a topico de temperatura
      delay(20);
      client.publish(t_status, m_online);
      
    } else {
      Serial.print("error, rc=");
      Serial.print(client.state());
      Serial.println(" intentado despues de 5 segundos");
      delay(10000);
      ESP.restart();
    }
  }
}