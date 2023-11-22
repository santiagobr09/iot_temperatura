#pragma once

AsyncWebServer server1(80);
bool wm_nonblocking = false; 
WiFiManager wm; 
WiFiManagerParameter custom_field; 

String getParam(String name){
  
  String value;
  if(wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

void inicializar_conexion(){

    WiFi.mode(WIFI_STA);

    if(wm_nonblocking) wm.setConfigPortalBlocking(false);
  int customFieldLength = 40;
  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str); // custom html input
  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);
  std::vector<const char *> menu = {"wifi","info","param","sep","restart","exit"};
  wm.setMenu(menu);
  // set dark theme
  wm.setClass("invert");
  wm.setConfigPortalTimeout(300); // auto close configportal after n seconds
  // wm.setCaptivePortalEnable(false); // disable captive portal redirection
  // wm.setAPClientCheck(true); // avoid timeout if client connected to softap
  // wifi scan settings
  // wm.setRemoveDuplicateAPs(false); // do not remove duplicate ap names (true)
  // wm.setMinimumSignalQuality(20);  // set min RSSI (percentage) to show in scans, null = 8%
  // wm.setShowInfoErase(false);      // do not show erase button on info page
  // wm.setScanDispPerc(true);       // show RSSI as percentage not graph icons
  // wm.setBreakAfterConfig(true);   // always exit configportal even if wifi save fails
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap

  //SSID Y CONTRASENA DEL PORTAL CAUTIVO
  res = wm.autoConnect("ESP32_CONFIG","123456789"); 
  if(!res) {
    Serial.println("cONEXION FALLIDA");
    // ESP.restart();
  } 
  else {
    //if you get here you have connected to the WiFi    
    Serial.println("CONECTADO!!! ");
    server1.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
    });

  AsyncElegantOTA.begin(&server1);    // Start ElegantOTA   subir bin
  server1.begin();
  Serial.println("INICIO SERVIDOR HTTP ");
  }


}