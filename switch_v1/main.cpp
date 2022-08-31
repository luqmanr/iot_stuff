// Relay control using the ESP8266 WiFi chip
// https://medium.com/trabe/wireless-switch-with-nodemcu-esp8266-and-a-relay-ba606a935b19

// Import required libraries
#include <ESP8266WiFi.h>
#include <aREST.h>
#include <aREST_UI.h>

// Create aREST instance
aREST_UI rest = aREST_UI();

// WiFi parameters
const char* ssid = "INSAN-AP";
const char* password = "kenapa tanya?";

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

int ledControl(String command);

void setup(void)
{  
  // Start Serial (to use the Serial Monitor)
  Serial.begin(115200);
  
  // Create UI
  // Set Page title
  rest.title("Relay Control");
  // Create On/Off buttons linked to D1 pin (GPIO5) 
  rest.button(1);

  // Function to be exposed on REST API
  rest.function("led", ledControl);

  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // prepare GPIO2
  // pinMode(5, OUTPUT);
  pinMode(2, OUTPUT);
  // digitalWrite(5, 0);
  digitalWrite(2, 0);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  
  // Print the IP address
  Serial.println(WiFi.localIP());
  
}

void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);
}

// Function exposed on REST API (same behaviour as the button)
int ledControl(String command) {
  // Print command
  Serial.println(command);

  // Get state from command
  int state = command.toInt();
  
  // Relay allows current to pass through with low voltage at the input (and vice versa).
  if (state == 0) {
    // digitalWrite(5, HIGH);
    digitalWrite(2, true);
    Serial.println("HIGH");
  }
  else {
    // digitalWrite(5, LOW);
    digitalWrite(2, false);
    Serial.println("LOW");
  }
  return 1;
}