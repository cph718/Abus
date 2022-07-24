#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"


const char* PARAM_POSSPEED = "posspeedval";
const char* PARAM_POSACCEL = "posaccelval";
const char* PARAM_POSPOSITION = "pospositionval";
const char* PARAM_CONSPEED = "conspeedval";
const char* PARAM_CONACCEL = "conaccelval";

/* Global Variables */
// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "Internat";
const char* password = "p1kachu1";
//const char* ssid = "lazersafe";
//const char* password = "#We*Sell*Lasers@World!";

// replace staticIP and gateway address based on your home router settings
IPAddress staticIP(192, 168, 1, 200);
IPAddress gateway(192, 168, 1, 1);
//IPAddress staticIP(192, 168, 106, 194);
//IPAddress gateway(192, 168, 106, 253);
IPAddress subnet(255, 255, 0, 0);

AsyncWebServer server(80);

void WebServer_Init()
{
  // Initialize SPIFFS
  if(!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  WiFi.mode(WIFI_STA);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) 
  {
    Serial.println("WiFi Failed!");
    return;
  }
  
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/index.html", String(), false);
  });

    // Send phone page to client
  server.on("/phone", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(SPIFFS, "/phone.html", String(), false);
  });

  // Receive an HTTP GET request
  server.on("/posstart", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Positional Start");
    
    String speedValueS, accelValueS, posValueS;
    int speedValue, accelValue, posValue;
    speedValueS = request->getParam(PARAM_POSSPEED)->value();
    speedValue = speedValueS.toInt();
    accelValueS = request->getParam(PARAM_POSACCEL)->value();
    accelValue = accelValueS.toInt();
    posValueS = request->getParam(PARAM_POSPOSITION)->value();
    posValue = posValueS.toInt();
    
    if((speedValue >= 0) & (accelValue >= 1))
    {
      
      if(Reload_ChangeStateMachine(positionalInit))
      {
        Reload_SetNextParamaters(speedValue, accelValue, posValue);
      }
    }
    
    request->send(200, "text/plain", "ok");
  });

    // Receive an HTTP GET request
  server.on("/posstop", HTTP_GET, [] (AsyncWebServerRequest *request) 
  {
    Serial.println("Positional Stop");

    //Stop move to function
  });
  
  server.on("/constart", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Continous Start");

    String cSpeedValueS, cAccelValueS;
    int cSpeedValue, cAccelValue;
    cSpeedValueS = request->getParam(PARAM_POSSPEED)->value();
    cSpeedValue = cSpeedValueS.toInt();
    cAccelValueS = request->getParam(PARAM_POSACCEL)->value();
    cAccelValue = cAccelValueS.toInt();

    if((cSpeedValue >= 0) & (cAccelValue >= 1))
    {
      if(Reload_ChangeStateMachine(continuousInit))
      {
        Reload_SetNextParamaters(cSpeedValue, cAccelValue, 0);
      }
    }

    request->send(200, "text/plain", "ok");
  });

  server.on("/conup", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Continous Up");
    Reload_UpdateContinuousState(1);

    request->send(200, "text/plain", "ok");
  });

  server.on("/condown", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Continous Down");
    Reload_UpdateContinuousState(2);

    request->send(200, "text/plain", "ok");
  });

  server.on("/constop", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    Serial.println("Continous Stop");
    Reload_UpdateContinuousState(0);

    request->send(200, "text/plain", "ok");
  });
  
 // server.onNotFound(handleNotFound);
  server.begin();

}
