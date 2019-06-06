// ### Bibliotecas ###
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//### Dados Wi-Fi ###

#define WLAN_SSID       "Fablab-IoT"
#define WLAN_PASS       "s0paiot01"
WiFiClient client;

//### Adafruit IO Stuff

#define SERVER      "io.adafruit.com"
#define SERVERPORT  1883
#define USERNAME    "PnMarques"
#define KEY         "0f4fa2db14184ea1a7093f201470e6c9"

// ### MQTT Stuff

Adafruit_MQTT_Client mqtt(&client, SERVER, SERVERPORT, USERNAME, KEY);
Adafruit_MQTT_Publish Temp_1 = Adafruit_MQTT_Publish(&mqtt, USERNAME "/feeds/sensoresfablab.temp1");
Adafruit_MQTT_Publish Press_1 = Adafruit_MQTT_Publish(&mqtt, USERNAME "/feeds/sensoresfablab.press1");
Adafruit_MQTT_Publish Humi_1 = Adafruit_MQTT_Publish(&mqtt, USERNAME "/feeds/sensoresfablab.humi1");


#define SEALEVELPRESSURE_HPA (1018.00)


int contador = 0;
int Temperatura;
int Humidade;
int Pressao;

Adafruit_BME280 bme;

void setup() {
  Serial.begin(9600);


  // ### WiFi Connection
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  Serial.print("Connecting WiFi");
  delay(500);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }
  Serial.println(" ");
  Serial.print("WiFi Connected to: ");
  Serial.println(WLAN_SSID);
  Serial.println(WiFi.localIP());

Serial.print("Wake up  reason");
//Serial.println(ESP.getResetReason());

  // ###   Ve sensor  ###
  if (!bme.begin(0x77)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  //####################
  //### connect MQQT ###
  //####################


  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  Serial.print("Conectando ao servidor MQTT... ");
  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      while (1);
    }
  }

  Serial.println("MQTT Connected!");


} // EO Setup

void loop() {

  float temp1;
  float humi1;
  float press1;

  Serial.print("contador 1:");
  Serial.println(contador);

  temp1 = bme.readTemperature();

  Serial.print("Temperature = ");
  Serial.print(temp1);
  Serial.println("*C");

  press1 = bme.readPressure() / 100.0F;
  Serial.print("Pressure = ");
  Serial.print(press1);
  Serial.println("hPa");

  humi1 = bme.readHumidity();
  Serial.print("Humidity = ");
  Serial.print(humi1);
  Serial.println("%");

  Serial.println();




  //### Publica Adafruit.IO ###

  
  if (! Temp_1.publish(temp1) ) {
    Serial.println("Erro no envio da temperatura!");
  } else {
    Serial.println("Temperatura Enviada!");
    Serial.println("");
    delay(500);
  }

    if (! Humi_1.publish(humi1) ) {
    Serial.println("Erro no envio da humidade!");
  } else {
    Serial.println("Humidade Enviada!");
    Serial.println("");
    delay(500);
  }
  
  if (! Press_1.publish(press1) ) {
    Serial.println("Erro no envio da pressão!");
  } else {
    Serial.println("Pressão Enviada!");
    Serial.println("");
    delay(500);
  }



  Serial.println("Going into deep sleep ");
  //ESP.deepSleep(30e6,WAKE_RF_DEFAULT); // 
  ESP.deepSleepInstant(60e7); //

  delay(15000);


} //EO Loop
