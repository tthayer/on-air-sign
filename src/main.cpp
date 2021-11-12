#include <M5Atom.h>
#include <ESPAsyncWebServer.h>
#include "config.h"

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Valid endpoints are <href=/on-air or /off-air");
}

int on_air_status = 0;

void setup() {
    M5.begin(true, false, true);
    M5.dis.drawpix(0, 0xf00000);
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.setHostname("on-air-esp32");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(WiFi.localIP());
  
    server.on("/on-air", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Unmuted");
        on_air_status = 1;
    });

    server.on("/off-air", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Muted");
        on_air_status = 2;
    });

    server.onNotFound(notFound);
    server.begin();
}

void loop() {
    M5.update();
    switch (on_air_status) {
      case 0: { M5.dis.drawpix(0, 0x0000FF); break; } // default set to blue
      case 1: { M5.dis.drawpix(0, 0x00FF00); break; } // on-air set to green
      case 2: { M5.dis.drawpix(0, 0xFF0000); break; } // off-air set to red
    }
}