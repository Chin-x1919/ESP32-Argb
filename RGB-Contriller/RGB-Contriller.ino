#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 4        // Pin for LED data
#define LED_COUNT 16     // LED Count(Change to your)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Wifi AP Config
const char* ssid = "ESP32_RGB";
const char* password = "12345678";

WebServer server(80);

// Web function
void handleRoot() {
  String html = "<html><body><h1>ESP32 ARGB Control</h1>";
  html += "<a href='/red'><button style='font-size:20px;'>RED</button></a><br><br>";
  html += "<a href='/green'><button style='font-size:20px;'>GREEN</button></a><br><br>";
  html += "<a href='/blue'><button style='font-size:20px;'>BLUE</button></a><br><br>";
  html += "<a href='/rainbow'><button style='font-size:20px;'>RAINBOW</button></a><br><br>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

// change colour function
void setColor(uint8_t r, uint8_t g, uint8_t b) {
  for(int i=0; i<LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(r,g,b));
  }
  strip.show();
}

void handleRed() {
  setColor(255,0,0);
  server.sendHeader("Location","/");
  server.send(303);
}
void handleGreen() {
  setColor(0,255,0);
  server.sendHeader("Location","/");
  server.send(303);
}
void handleBlue() {
  setColor(0,0,255);
  server.sendHeader("Location","/");
  server.send(303);
}

void handleRainbow() {
  rainbowCycle(5);
  server.sendHeader("Location","/");
  server.send(303);
}

// Rainbow effect
void rainbowCycle(int wait) {
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  //Access Point Mode
  WiFi.softAP(ssid, password);
  Serial.begin(115200);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());  // it should be 192.168.4.1

  // config route
  server.on("/", handleRoot);
  server.on("/red", handleRed);
  server.on("/green", handleGreen);
  server.on("/blue", handleBlue);
  server.on("/rainbow", handleRainbow);

  server.begin();
}

void loop() {
  server.handleClient();
}
