#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiUdp.h>
#include <string.h>
#include <ESP8266Ping.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32 

#define OLED_RESET  LED_BUILTIN 
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

WiFiUDP udp;

const char *ssid = "";
const char *password = "";
String ip_addr = "";

char packetBuffer[256];
char *ack = "ACK";

String ipToString(IPAddress ip){
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void initDisplay(Adafruit_SSD1306 display, int i2cAddress){
  if(!display.begin(SSD1306_SWITCHCAPVCC, i2cAddress)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
}

void printDisplay(Adafruit_SSD1306 display, String str) {
  //display.clearDisplay();
  display.clearDisplay();
  display.setTextSize(1);      
  display.setTextColor(SSD1306_WHITE); 
  display.setCursor(0, 0);     
  display.cp437(true);         

  display.print(str);

  display.display();

}

void addressScan(){
  for(int i = 1;i<127;i++){
    Wire.beginTransmission(i);
    int addr = Wire.endTransmission();
    if(addr == 0){
      Serial.print("Address found: ");
      Serial.println(i, HEX);
    }

    if(addr == 4){
      Serial.print("Unknown address found: ");
      Serial.println(i, HEX);
    }
    
  }
}




void setup() {

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print("\n\n");


  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print("\nConnecting...");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  ip_addr = ipToString(WiFi.localIP());

  if(!display1.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display fail!");
  }

       

  Serial.println(udp.begin(4210));
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), 4210);


  udp.beginPacket(IPAddress(192,168,1,254), 3000);
  udp.write(ack);
  udp.endPacket();
  

 // addressScan();
}

void loop() {
  // put your main code here, to run repeatedly:

  int rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  //printDisplay(display1, String(rssi));

  delay(1000);
  
}
