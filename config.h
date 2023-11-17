#pragma once

const char* mqtt_server     = "mqtt.mantis.ec";
const int mqtt_port     = 1883;
const char* mqtt_user     = "mantis_iot";
const char* mqtt_pass     = "kae2eZetohbahzeu6Uodah1ujai9ohSa2urec2aezaezaeSh8Sooph4ahf6Ieque";
//const char* host_id = WiFi.macAddress().c_str();

const char* t_status = "mantis/status";
const char* t_temperatura = "/mantis/temperatura/";
const char* t_humedad = "/mantis/humedad/";

const char* m_online = "online";

const char* t_light_state = "/ha/luminaria1/status";
const char* t_light_command = "/ha/luminaria1/switch";

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