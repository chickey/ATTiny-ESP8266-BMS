#include <Wire.h>
#include <ESP8266WiFi.h>

// Configure SSID and password of Wifi Network
const char* ssid = "chickey-house";
const char* password = "adamjoeollie";

// Web Server on port 80
WiFiServer server(80);

//Declare pins for ATtiny inputs

const int Cell1 = D2;
const int Cell2 = D3;
const int Cell3 = D4;
const int Cell4 = D5;
const int Cell5 = D6;
const int Cell6 = D7;
const int Cell7 = D8;

//Declare booleans for auto-detect Cell count
boolean CellON1 = false;
boolean CellON2 = false;
boolean CellON3 = false;
boolean CellON4 = false;
boolean CellON5 = false;
boolean CellON6 = false;
boolean CellON7 = false;

//Common variable declarations for converting pwm to mV
int CellV;
unsigned long z;
unsigned long y;
unsigned long x;

//Declare boolean for LVC warning and cutoff
boolean LVWarn = false;
boolean LVCutoff = false;

//Declare LVC output to motor control MCU
const int throttleBack = 8;
const int throttleCut = 9;

//Manual Variables
int LVW = 3600;  //Warning mV
int LVC = 3400;  //Cutoff mV

void setup() {

Serial.begin(9600);
Serial.println("Booting");

  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());

pinMode(Cell1, INPUT);
pinMode(Cell2, INPUT);
pinMode(Cell3, INPUT);
pinMode(Cell4, INPUT);
pinMode(Cell5, INPUT);
pinMode(Cell6, INPUT);
pinMode(Cell7, INPUT);

//pinMode(throttleBack, OUTPUT);  //Analog GPIO output to throttle management
//pinMode(throttleCut, OUTPUT);

delay(800);  //Ensure all ATtiny85s are initiated

//Check for active Cells
int CellCheck1 = pulseIn(Cell1, HIGH, 100000);
int CellCheck2 = pulseIn(Cell2, HIGH, 100000);
int CellCheck3 = pulseIn(Cell3, HIGH, 100000);
int CellCheck4 = pulseIn(Cell4, HIGH, 100000);
int CellCheck5 = pulseIn(Cell5, HIGH, 100000);
int CellCheck6 = pulseIn(Cell6, HIGH, 100000);
int CellCheck7 = pulseIn(Cell7, HIGH, 100000);

Serial.printf("Detecting active Cells: ");

int batteryCellCount = 0;  //variable to add up active Cells

//Toggle active Cells ON and print out
if(CellCheck1 > 1){
    CellON1 = true;
    Serial.print("1");
    batteryCellCount++;
}
if(CellCheck2 > 1){
    CellON2 = true;
    Serial.println("2");
    batteryCellCount++;
}
if(CellCheck3 > 1){
    CellON3 = true;
    //Serial.print("3");
    batteryCellCount++;
}
if(CellCheck4 > 1){
    CellON4 = true;
    //Serial.print("4");
    batteryCellCount++;
}
if(CellCheck5 > 1){
    CellON5 = true;
    //Serial.print("5");
    batteryCellCount++;
}
if(CellCheck6 > 1){
    CellON6 = true;
    //Serial.print("6");
    batteryCellCount++;
}
if(CellCheck7 > 1){
    CellON7 = true;
    //Serial.print("7");
    batteryCellCount++;
}
delay(100);
Serial.println();
Serial.print(batteryCellCount);
Serial.println(" Cells detected");
delay(2000);

}

void loop() 
{
  if(CellON1 == true){
      y = pulseIn(Cell1, HIGH, 100000);
      z = pulseIn(Cell1, LOW, 100000);
//      Serial.print("z pulse = ");
//      Serial.println(z);
//      Serial.print("y pulse = ");
//      Serial.println(y);
      CellV = pwmConvert(z, y);
      throttleTend(CellV);
//      Serial.print("1: ");
//      Serial.println(CellV);
  }
  if(CellON2 == true){
      z = pulseIn(Cell2, HIGH, 100000);
      y = pulseIn(Cell2, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print(" 2: ");
      Serial.print(CellV);
  }
  if(CellON3 == true){
      z = pulseIn(Cell3, HIGH, 100000);
      y = pulseIn(Cell3, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print("3: ");
      Serial.print(CellV);
  }
  if(CellON4 == true){
      z = pulseIn(Cell4, HIGH, 100000);
      y = pulseIn(Cell4, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print("4: ");
      Serial.print(CellV);
  }
  if(CellON5 == true){
      z = pulseIn(Cell5, HIGH, 100000);
      y = pulseIn(Cell5, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print("5: ");
      Serial.print(CellV);
  }
  if(CellON6 == true){
      z = pulseIn(Cell6, HIGH, 100000);
      y = pulseIn(Cell6, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print("6: ");
      Serial.print(CellV);
  }
  if(CellON7 == true){
      z = pulseIn(Cell7, HIGH, 100000);
      y = pulseIn(Cell7, LOW, 100000);

      CellV = pwmConvert(z, y);
      throttleTend(CellV);
      Serial.print("7: ");
      Serial.print(CellV);
  }

  //Serial.println(""); //End row print
// Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        
        if (c == '\n' && blank_line) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            // your actual web page that displays temperature
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head></head><body><h1>Battery Monitor</h1><h3>Battery 1: ");
            client.println(CellV);
            client.println("mV</h3></body></html>");  
            break;
        }
        if (c == '\n') {
          // when starts reading a new line
          blank_line = true;
        }
        else if (c != '\r') {
          // when finds a character on the current line
          blank_line = false;
        }
      }
    }  
    // closing the client connection
    delay(1);
    client.stop();
    Serial.println("Client disconnected.");
  }
  
  //Early Warning
  if(LVWarn == true) {
    Serial.println(" WARNING LOW VOLTAGE");
    LVWarn = false;
    digitalWrite(throttleBack, HIGH);
  }
  else
  {
     digitalWrite(throttleBack, LOW);
  }

  //Low Voltage Cutoff
  if(LVCutoff == true){
    Serial.println(" Cutoff Limit");
    LVCutoff = false;
    digitalWrite(throttleCut, HIGH);
  }
  else
  {
    digitalWrite(throttleCut, LOW);
  }

 delay(1000); //common delay
  
}

void throttleTend(int x)
{
  if(x < LVC){
    LVCutoff = true;
    //digitalWrite(throttleCut, HIGH);
  }
  if(x < LVW && x > LVC){
    LVWarn = true;
    //digitalWrite(throttleBack, HIGH);
  }
}

int pwmConvert(int z, int y)
{
  int x = z + y;
  int v = (int) ( 5000UL * z / x );
  return v;
}

