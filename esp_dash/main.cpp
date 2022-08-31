/*
  -----------------------
  ESPDASH - Basic Example
  -----------------------

  Skill Level: Intermediate

  In this example we will be creating a basic dashboard which consists 
  of some cards and then update them in realtime ( at 3s interval ).

  Github: https://github.com/ayushsharma82/ESP-DASH
  WiKi: https://ayushsharma82.github.io/ESP-DASH/

  Works with both ESP8266 & ESP32
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPDash.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


/* Your WiFi Credentials */
const char* ssid = "INSAN-AP"; // SSID
const char* password = "kenapa tanya?"; // Password
const int togglePin = 2 ;

/* Start Webserver */
AsyncWebServer server(80);

/* Attach ESP-DASH to AsyncWebServer */
ESPDash dashboard(&server); 

/* 
  Dashboard Cards 
  Format - (Dashboard Instance, Card Type, Card Name, Card Symbol(optional) )
*/
Card temperature(&dashboard, TEMPERATURE_CARD, "Temperature", "°C");
Card humidity(&dashboard, HUMIDITY_CARD, "Humidity", "%");
Card statusCard(&dashboard, STATUS_CARD, "Init Status", "success");
Card button(&dashboard, BUTTON_CARD, "Test Button");

/* Functions to use when button is triggered */
// void TogglePower(int pin, String default_state);
void TogglePower(int pin, String default_state) {
  // Relay allows current to pass through with low voltage at the input (and vice versa).
  if (default_state == "NO") {
    digitalWrite(pin, HIGH);
    delay(3000);
    digitalWrite(pin, LOW);
  }
  else {
    digitalWrite(pin, LOW);
    delay(3000);
    digitalWrite(pin, HIGH);
  }
}

// String serverName = "http://localhost:80/toggle";

void setup() {
  Serial.begin(115200);  
//   Serial.begin(921600);

  /* Connect WiFi */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.printf("WiFi Failed!\n");
      return;
  }
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  // WiFiClient client;
  // HTTPClient http;

  // prepare GPIO2
  pinMode(togglePin, OUTPUT);
  digitalWrite(togglePin, LOW);
  button.update(false);

  /* Attach Button Callback */
  button.attachCallback([&](bool value){
    /* Print our new button value received from dashboard */
    Serial.println("Button Triggered: "+String((value)?"true":"false"));

    /* Make sure we update our button's value and send update to dashboard */   
    button.update(value);
    dashboard.sendUpdates();

    digitalWrite(togglePin, (!value));
      
    // // Your Domain name with URL path or IP address with path
    // http.begin(client, serverName.c_str());
    
    // // Send HTTP GET request
    // int httpResponseCode = http.GET();
    // if (httpResponseCode>0) {
    //     Serial.print("HTTP Response code: ");
    //     Serial.println(httpResponseCode);
    //     String payload = http.getString();
    //     Serial.println(payload);
    // }
    // else {
    //   Serial.print("Error code: ");
    //   Serial.println(httpResponseCode);
    // }
    // // Free resources
    // http.end();

  });

  /* Start AsyncWebServer */
  server.begin();
}

void loop() {
  /* Update Card Values */
  temperature.update((int)random(0, 50));
  humidity.update((int)random(0, 100));
  statusCard.update("New message", "danger");

  /* Send Updates to our Dashboard (realtime) */
  dashboard.sendUpdates();
  // TogglePower(togglePin, "NO");
  // delay(5000);
  // TogglePower(togglePin, "NO");

  // /* TOGGLE BUTTON ON OFF after delay, basically blinking */
  // button.update(true);
  // dashboard.sendUpdates();
  // delay(5000);
  // button.update(false);
  // dashboard.sendUpdates();

  /* 
    Delay is just for demonstration purposes in this example,
    Replace this code with 'millis interval' in your final project.
  */
  delay(3000);
}