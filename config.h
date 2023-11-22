#pragma once

const char* mqtt_server     = "37.187.95.240";
const int mqtt_port     = 1883;

const char* t_status = "/utn/status";
const char* t_temperatura = "/utn/temperatura/";

const char* m_online = "online";

const char* LIGHT_ON = "ON";
const char* LIGHT_OFF = "OFF";

boolean light_state = false;

const int ledPin =  2; //led interno ESP


// Create the °C Symbol
//const uint8_t Celsius[] = {
  //SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
  //SEG_A | SEG_D | SEG_E | SEG_F   // C
//};
// Create the °C Symbol
const uint8_t Celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G
};