#define BLYNK_TEMPLATE_ID "TMPL6_0ObQRWb"
#define BLYNK_TEMPLATE_NAME "Smart Device"
#define BLYNK_AUTH_TOKEN "EmqiB-e23ooWtY2cISNWqhWX3BnPdRRi"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>

const int AOUTpin= 34; 
const int sensorPin = 35;
const float alpha = 0.95; // Low Pass Filter alpha (0 - 1 )
const float aRef = 4.6; // analog reference
float filteredVal = 512.0; // midway starting point
float sensorVal;
float voltage;
float psiVal;
int value;
#define DHTPIN 4 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // DHT 11


DHT dht(DHTPIN, DHTTYPE);
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Sabbir"; // your WiFi name
char pass[] = "19122918"; // your WiFi password


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);

  while (!Serial);
  
  dht.begin();
 
  delay(1000);

}

void loop() {
 
 sensorVal = (float)analogRead(sensorPin); // Read pressure sensor val (A0)
  filteredVal = (alpha * filteredVal) + ((1.0 - alpha) * sensorVal); // Low Pass Filter
  voltage = (filteredVal / 1024.0) * aRef; // calculate voltage
  psiVal = (voltage - 0.4784) / 0.0401; // x=(y-b)/m

  Serial.print("raw_adc "); Serial.print(sensorVal, 0);
  Serial.print(", filtered_adc "); Serial.println(filteredVal, 0);
  Serial.print(psiVal);

  delay(200);

  value= analogRead(AOUTpin);
  Serial.print("Butane value: ");
  Serial.println(value);
  // DHT sensor handling
  float temperature = dht.readTemperature(); // Read temperature
  int humidity = dht.readHumidity();       // Read humidity

  Serial.print("T: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.println(humidity);

  
  Blynk.virtualWrite(V0, psiVal);
  Blynk.virtualWrite(V1, value);
  Blynk.virtualWrite(V2, temperature);
  Blynk.virtualWrite(V3, humidity);


  Blynk.run();
  delay(1000); // Short delay for readability and stability
}
