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
    Serial.print("Attempting MQTT connection...");
    if (client.connect(WiFi.macAddress().c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("connected");
      client.subscribe(t_temperatura);
      //client.subscribe(topicEventos);
      //client.subscribe(topicConexion);
      delay(20);
      client.publish(t_status, m_online);
      //client.publish(topicEventos, "ESP inicio...");
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(10000);
      ESP.restart();
    }
  }
}