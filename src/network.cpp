/*------------------------------------------------------------------------------
  Copyright (c) 2022 Imed El Hadef
  Any reproduction without written permission is prohibited by law.
  ------------------------------------------------------------------------------
  Description :
  Implementation of the ESP32 WIFI network module
  ------------------------------------------------------------------------------
*/
#include "WiFi.h"
#include "network.h"
#include "mywifi.h"
//#include <StateMachine.h>
#include <PubSubClient.h>
#include <Arduino.h>
/*==============================================================================
  MACROS
  ==============================================================================
*/
#define MQTT_SERVER     "broker.hivemq.com"
#define MQTT_PORT       1883

#define MAX_TX_DATA    50 // 50 bytes
/*==============================================================================
  TYPES
  ==============================================================================
*/


/*==============================================================================
  LOCAL FUNCTION PROTOTYPES
  ==============================================================================
*/
static void wifiStatusCb(WIFI_ConnectState_t wifiConnectState);
static void callback(char* topic, byte* payload, unsigned int length);
static void setupMQTT();
static void reconnect(); 
static void networkTx(void);
/*==============================================================================
  GLOBAL DATA
  ==============================================================================
*/
static long    _last_time;
// MQTT data
WiFiClient   _wifiClient;
PubSubClient _mqttClient(_wifiClient);
static char    _txData[MAX_TX_DATA];

static bool _isNetworkActive = false;
/*==============================================================================
  CONST DATA
  ==============================================================================
*/

/*==============================================================================
  INTERFACE FUNCTIONS
  ==============================================================================
*/
//------------------------------------------------------------------------------
void NETWORK_init(void)
{
  Serial.println("Init Network Module");
}
//------------------------------------------------------------------------------
void NETWORK_start(void)
{
  MYWIFI_registerWifiConnectState(wifiStatusCb);
}
//------------------------------------------------------------------------------

void NETWORK_handle(void)
{
  //MYWIFI_registerWifiStatus(wifiStatusCb);
  if (!_mqttClient.connected()) {
    reconnect();
  }
   _mqttClient.loop();
   networkTx();
}

/*==============================================================================
   LOCAL FUNCTIONS
  ==============================================================================
*/
//------------------------------------------------------------------------------
// Description here

static void wifiStatusCb(WIFI_ConnectState_t wifiConnectState)
{
  if(wifiConnectState == WIFI_MODEM_CONNECTED)
  {
    setupMQTT();
    reconnect();
    _isNetworkActive = true;
  }
  if(WIFI_MODEM_NOT_CONNECTED)
  {
    _isNetworkActive = false;
  }
  
}

//------------------------------------------------------------------------------
// Description here
static void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback - ");
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}
//------------------------------------------------------------------------------
// Description here
// MQTT routines
static void setupMQTT() {
  _mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
  // set the callback function
  _mqttClient.setCallback(callback);
}

//------------------------------------------------------------------------------
// Description here
static void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!_mqttClient.connected()) {
    Serial.println("Reconnecting to MQTT Broker..");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);

    if (_mqttClient.connect(clientId.c_str())) {
      Serial.println("Connected.");
      // subscribe to topic
      _mqttClient.subscribe("/swa/commands");
    }

  }
}

//------------------------------------------------------------------------------
// Description here
static void networkTx(void)
{
  long now = millis();
  if (now - _last_time > 60000) {
    // Send data
    float temp = 24.564;
    float hum = 16.45;
    float pres = 20.8 / 100;
    // Publishing data throgh MQTT
    sprintf(_txData, "%f", temp);
    Serial.println(_txData);
    _mqttClient.publish("/swa/temperature", _txData);
    sprintf(_txData, "%f", hum);
    Serial.println(hum);
    _mqttClient.publish("/swa/humidity", _txData);
    sprintf(_txData, "%f", pres);
    Serial.println(pres);
    _mqttClient.publish("/swa/pressure", _txData);
    _last_time = now;
  }
}