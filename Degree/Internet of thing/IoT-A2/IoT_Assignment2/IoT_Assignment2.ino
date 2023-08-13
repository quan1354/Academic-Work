#include <Wire.h>
#include "MAX30105.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "heartRate.h"
#include <ArduinoJson.h>
#include <WiFiClientSecure.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <math.h>

// *********Hearbeat variable*****************
const byte RATE_SIZE = 9; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
char classify[] = "";
int beatAvg;
int beatMedian;
int beatHighest;
int beatStd;
String classifyBeat;

// *********Hearbeat Sensor*******************
MAX30105 particleSensor;

// *********WiFi variables********************
char ssid[] = "chuah’s iPhone";                                  
char password[] = "12345678";
WiFiServer server(80);
WiFiClient wifiClient;

// *********MQTT broker variable**************
#define mqtt_server "io.adafruit.com"
#define mqtt_port 1883
#define mqtt_user "quan1354"
#define mqtt_key  "aio_fNUh08mWYdjBZm8TpGaQyFOwqPnB"

// *********MQTT Publisher**************
Adafruit_MQTT_Client mqtt(&wifiClient,mqtt_server,mqtt_port,mqtt_user,mqtt_key);
Adafruit_MQTT_Publish beatPerMin_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatPerMin");
Adafruit_MQTT_Publish beatHighest_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatHighest");
Adafruit_MQTT_Publish beatAverage_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatAverage");
Adafruit_MQTT_Publish beatStandardDeviation_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatStandardDeviation");
Adafruit_MQTT_Publish beatClassify_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatClassify");
Adafruit_MQTT_Publish beatJson_publisher = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatJson");

// **MQTT Heart Rate Class Publisher**
Adafruit_MQTT_Publish low_Class = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/heartrateclass.low");
Adafruit_MQTT_Publish medium_Class = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/heartrateclass.medium");
Adafruit_MQTT_Publish high_Class = Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/heartrateclass.high");

// *********MQTT Subcriber**************
Adafruit_MQTT_Subscribe subscribeBpm = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatPerMin");
Adafruit_MQTT_Subscribe subscribeHighestBpm = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatHighest");
Adafruit_MQTT_Subscribe subscribeAvgBpm = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatAverage");
Adafruit_MQTT_Subscribe subscribeStdBpm = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatStandardDeviation");
Adafruit_MQTT_Subscribe subscribebeatClassify = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatClassify");
Adafruit_MQTT_Subscribe subscribeJson = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/beatJson");
Adafruit_MQTT_Subscribe subscribe_low_Class = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/heartrateclass.low");
Adafruit_MQTT_Subscribe subscribe_medium_Class = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/heartrateclass.medium");
Adafruit_MQTT_Subscribe subscribe_high_Class = Adafruit_MQTT_Subscribe(&mqtt, mqtt_user "/feeds/heartrateclass.high");
// *********Define Functions******************
void calculateAverageBpm();
void calculateBpm();
void calculateStandardDeviation();
void mqttConnection();
void createHeartRateData();
void readLastHeartRateData();
void updateStandardDeviation(int newHeartRate);
void deleteHighestRate();
void resetPublisher();
void promptCollectResult();

// *********Setup Function******************
void setup(){
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
  mqtt.subscribe(&subscribe_low_Class);
  mqtt.subscribe(&subscribe_medium_Class);
  mqtt.subscribe(&subscribe_high_Class);
  mqtt.subscribe(&subscribeBpm);
  mqtt.subscribe(&subscribeHighestBpm);
  mqtt.subscribe(&subscribeAvgBpm);
  mqtt.subscribe(&subscribeStdBpm);
  mqtt.subscribe(&subscribebeatClassify);
  mqtt.subscribe(&subscribeJson);
}

// *********Loop Function******************
void loop(){
  mqttConnection();
  long irValue = particleSensor.getIR();

  calculateBpm();

  rates[rateSpot] = (byte)beatsPerMinute; //Store this reading in the array
  if(rateSpot == 9){
    calculateAverageMedianBpm();
    calculateStandardDeviation();
    //classifyLevel();

    //promptCollectResult();
    if(beatAvg > beatHighest){
      beatHighest = beatsPerMinute;
      deleteHighestRate(); // Delete For Highest BPM
      beatHighest_publisher.publish(beatHighest); 
    }

    rateSpot = 0;
    Serial.print("Average:");
    Serial.println(beatAvg);
    Serial.print("Median:");
    Serial.println(beatMedian);
    updateStandardDeviation(beatStd); // Update For standard deviation

    // !!!! Remark: Free to uncommend, others function in loops, to have upload and read more data, but will lead BPM result significant low
    // upload stuture data with Median better, because after upload data, first BPM get very low 
    Adafruit_MQTT_Publish placer = (beatMedian >=0 && beatMedian <40) ? low_Class : (beatMedian >=40 && beatMedian <110) ? medium_Class : high_Class;

    if(!placer.publish(beatMedian)){
      Serial.println("Data upload Failure");
    }else{
      Serial.println("Data upload Successful");
    }
    //createHeartRateData();
    delay(1000);// ensure upload before, read data

    Adafruit_MQTT_Subscribe *read;
    while(read = mqtt.readSubscription(5000)){
      if(read == &subscribe_low_Class){
        Serial.print("Low Class Last Read: ");
        Serial.println((char*)subscribe_low_Class.lastread);
      }
      if(read == &subscribe_medium_Class){
        Serial.print("Medium Class Last Read: ");
        Serial.println((char*)subscribe_medium_Class.lastread);
      }
      if(read == &subscribe_high_Class){
        Serial.print("High Class Last Read: ");
        Serial.println((char*)subscribe_high_Class.lastread);
      }
    }

    //readLastHeartRateData();
    //resetPublisher();
  }else{
    Serial.print(Count:);
    Serial.println(rateSpot);
    Serial.println(rates[rateSpot]);
    rateSpot++;
  }
  // ==============================================
  if(!mqtt.ping()) mqtt.disconnect();
  delay(500);
}

// *********BMP Calculation******************
void calculateBpm(){
  long delta = millis() - lastBeat;
  lastBeat = millis();
  beatsPerMinute = 60 / (delta / 1000.0);
}
// ******Calculate Average/Mean********
void calculateAverageMedianBpm(){
  int position = RATE_SIZE / 2;
  // Serial.print("Position:");
  // Serial.println(position);
  beatMedian = rates[position] ; // median position is 4 in an array
  beatAvg = 0;
  for (byte x = 0 ; x < RATE_SIZE; x++)
    beatAvg += rates[x];
  beatAvg /= RATE_SIZE + 1; // 10 elements
}

// ******Calculate Standard Deviation********
void calculateStandardDeviation() {
    int sum = 0;
    int n = sizeof(rates) / sizeof(rates[0]);
    for (int i = 0; i < n; i++) {
        sum += pow((rates[i] - beatAvg), 2);
    }
    beatStd =  sqrt(sum / n);
}
// *********MQTT Connection****************
void mqttConnection(){
  if(mqtt.connected())
      return;

    Serial.println("Connecting to MQTT...");
    int tries = 3;
    while(mqtt.connect() != 0){
      Serial.println("Retry MQTT connection in 5 seconds...");
      mqtt.disconnect();
      delay(500);
      if(--tries == 0)
        while(true);
    }
    Serial.println("Connected to MQTT Server.");
}
// *********Classify User Heart Beat Class************
void classifyLevel(){
  if(beatAvg >=0 && beatAvg <40){
    classifyBeat = "Low";
  }else if(beatAvg >=40 && beatAvg <110){
    classifyBeat = "Medium";
  }else if(beatAvg >=110){
    classifyBeat = "High";
  }else{
    classifyBeat = "Average BPM have problem ...";
  }
}
// *********Create BPM data********************
void createHeartRateData() {
  //Create a new message
  char bpmMessage[16], avgBpmMessage[16], jsonMessage[128];
  DynamicJsonDocument doc(1024);

  sprintf(bpmMessage, "%d", (int) beatsPerMinute);
  sprintf(avgBpmMessage, "%d", beatAvg);

  //Publish the message
  if (!beatPerMin_publisher.publish(beatsPerMinute)) {
    Serial.println("Failed to upload BPM data.");
  } else {
    Serial.println("BPM published successfully.");
  }
  beatAverage_publisher.publish(avgBpmMessage);
  beatClassify_publisher.publish(classifyBeat.c_str());

  //Data upload by using JSON
  doc["BPM"] = beatsPerMinute; 
  doc["Average BPM"] = beatAvg; 
  doc["Higest BPM"] = beatHighest; 
  doc["Standard Deviation BPM"] = beatStd; 
  doc["Class BPM"] = classifyBeat.c_str(); 
  serializeJson(doc, jsonMessage);
  beatJson_publisher.publish(jsonMessage);
}
// *********Read data**********************
void readLastHeartRateData() {
  //Read the last message
  mqtt.processPackets(5000);
  StaticJsonDocument<1000> doc;
  DeserializationError err = deserializeJson(doc, (char*)subscribeJson.lastread);
  if (err){
    Serial.print("Read Json Error:");
    Serial.println(err.c_str());
  }
  Serial.println("===========Last Read Data===============");
  Serial.print("BPM:");
  Serial.println((char*)subscribeBpm.lastread);
  Serial.print("Higest BPM:");
  Serial.println((char*)subscribeHighestBpm.lastread);
  Serial.print("Average BPM:");
  Serial.println((char*)subscribeAvgBpm.lastread);
  Serial.print("Standard Deviation BPM:");
  Serial.println((char*)subscribeStdBpm.lastread);
  Serial.print("BPM Class:");
  Serial.println((char*)subscribebeatClassify.lastread);
  Serial.print("BPM Json:");
  serializeJson(doc, Serial);
  Serial.println();
  Serial.println("========================================");
}

// *********Update Standard Deviation********************
void updateStandardDeviation(int newHeartRate) {
  // Read the last message
  mqtt.processPackets(5000);
  char* lastMessage = (char*)subscribeStdBpm.lastread;

  // Update the last message with the new heart rate value
  char updatedMessage[16];
  sprintf(updatedMessage, "%d", newHeartRate);

  if (strcmp(lastMessage, updatedMessage) == 0) {
    Serial.println("Standard Deviation BPM is up-to-date.");
  } else {
    if (!beatStandardDeviation_publisher.publish(updatedMessage)) {
      Serial.println("Failed to update data.");
    } else {
      Serial.println("Standard Deviation BPM updated successfully.");
    }
  }
}

// **********Delete Highest BPM********************
void deleteHighestRate() {
  // Read the last message
  mqtt.processPackets(5000);
  char* lastMessage = (char*)subscribeHighestBpm.lastread;

  // Delete the last message
  if (strcmp(lastMessage, "") == 0) {
    Serial.println("No data to delete.");
  } else {
    if (!beatHighest_publisher.publish("",true)) {
      Serial.println("Failed to delete data.");
    } else {
      Serial.println("Data deleted successfully.");
    }
  }
}

// ***********Reset Publisher*****************
void resetPublisher() {
  // disconnect the MQTT client
  mqtt.disconnect();

  // create a new publisher object
  beatPerMin_publisher =  Adafruit_MQTT_Publish(&mqtt, mqtt_user "/feeds/beatPerMin");
}

void promptCollectResult(){
    Serial.print(", BPM=");
    Serial.print(beatsPerMinute);
    Serial.print(", Avg BPM=");
    Serial.print(beatAvg);
    Serial.print(", Highest BPM=");
    Serial.print(beatHighest);
    Serial.print(", STD Deviation=");
    Serial.println(beatStd);
}

