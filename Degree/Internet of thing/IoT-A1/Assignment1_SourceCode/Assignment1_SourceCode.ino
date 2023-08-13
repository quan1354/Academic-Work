#include <Wire.h>
#include "MAX30105.h"
#include <ESP8266WiFi.h>
#include "heartRate.h"

// Hearbeat variable
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
char classify[] = "";

// Hearbeat Sensor
MAX30105 particleSensor;

// WiFi variables
char ssid[] = "chuah’s iPhone";                                  
char password[] = "12345678";
WiFiServer server(80);

// functions
void calculateAverageBmP();
void calculateBmp();
String myAppTemplate();

void setup()
{
  Serial.begin(9600);
  Serial.println("Initializing...");

  // Initialize sensor with default I2C port, 400kHz speed
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)){
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  // Setup WiFi
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("Controller has connected to WLAN");

  // Setup Server
  server.begin();
  Serial.println("Server started at port 80");

  // Display Server Link to User
  Serial.print("URL=http://");
  Serial.print(WiFi.localIP());
  Serial.print("/");
}

void loop(){
  long irValue = particleSensor.getIR();
  WiFiClient client = server.available();
  //if(!client) return;

  if (checkForBeat(irValue) == true){
    // Calculate bpm
    calculateBmp();
    // Calculate avarage bpm
    if (beatsPerMinute < 255 && beatsPerMinute > 20){
      calculateAverageBmP();
    }
  }

  //Display Result
  if(irValue < 50000){
    Serial.print("No finger detected");
  }else{
    Serial.print("IR=");
    Serial.print(irValue);
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);
  }
  Serial.println();

  //wait until the client sends the data
  //while(!client.available()) delay(1);
  //String request = client.readStringUntil('\r');
  //client.flush();

  //check request
  // if(request.indexOf("/SENSOR=OFF") != -1){
  //   particleSensor.shutDown();
  // }
  // if(request.indexOf("/SENSOR=ON") != -1){
  //   particleSensor.setup();
  //   particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  //   particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
  // }

  //render result to webpage
  //client.println("HTTP/1.1 200 OK");
  //client.println("Content-Type: text/html");
  client.print(myAppTemplate(irValue, beatsPerMinute, beatAvg));
  client.flush();
}


void calculateBmp(){
  long delta = millis() - lastBeat;
  lastBeat = millis();
  beatsPerMinute = 60 / (delta / 1000.0);
}
void calculateAverageBmP(){
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
}

String myAppTemplate(float irValue, float beatsPerMinute, float beatAvg){
  // Status matching
  int rateLevel[4] = {40,101,110,131};
  String response[] = {"too low", "Normal", "Plase continue monitoring at home", "Have to take advice from Doctor", "Please call Polis 999, Have to take cure"};
  bool isFound = false;
  String status;

  for(int i=0; isFound == false; i++){
    if(beatsPerMinute < rateLevel[i]){
      isFound = true;
      status = response[i];
    }else if(beatsPerMinute >= rateLevel[3]){
      isFound = true;
      status = response[4];
    }
  }
  Serial.print(status);

  // HTML Template
  String ptr  = "<!DOCTYPE html><html lang=\"en\"><head>";
  ptr+="<meta charset=\"UTF-8\"><meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">";
  ptr+="<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  ptr+="<title>Document</title>";
  ptr+="<style>.container{width: 30%;display: block; margin: 20rem auto;}";
  ptr+=".title{text-align: center;font-size: 40px;border: red solid 3px;background-color: yellow;}";
  ptr+=".content{font-size: 35px;border:blue solid 3px;background-color: aqua}";
  ptr+=".content2{text-align:center}";
  ptr+="p{font-weight: bold;text-align: center;color: rgb(53, 15, 114);font-size: 35px;}span{color: red;}";
  ptr+="body{background-color: blanchedalmond;}</style>";
  ptr+="</head><body><div class=\"container\"><div class=\"title\">My Heart Beat Per Minute (BPM)</div><div class=\"content\">";

  if(irValue < 5000){
    ptr+="<p>No finger detected<p>";
  }else{
    ptr+="<p>IR:<span>";
    ptr+=irValue;
    ptr+="</span></p>";
    ptr+="<p>BMP:<span>";
    ptr+=beatsPerMinute;
    ptr+="</span></p>";
    ptr+="<p>Average BMP:<span>";
    ptr+=beatAvg;
    ptr+="</span></p>";
    ptr+="<p>Status: <span>";
    ptr+=status;
    ptr+="</span></p></div>";
  }
  //ptr+="<div class=\"content2\"><a href='/SENSOR=OFF'><button>TURN OFF</button></a><a href='/SENSOR=ON'><button>TURN ON</button></a></div>";
  ptr+="</div></body></html>";
  isFound = false;
  return ptr;
}


