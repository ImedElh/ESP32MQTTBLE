/*------------------------------------------------------------------------------
  Copyright (c) 2022 Imed El Hadef
  Any reproduction without written permission is prohibited by law.
  ------------------------------------------------------------------------------
  Description :
  Implementation of the ESP32 WIFI connection module
  ------------------------------------------------------------------------------
*/
#include "WiFi.h"
#include <StateMachine.h>
#include "mywifi.h"
/*==============================================================================
  MACROS
  ==============================================================================
*/
#define WIFI_PASSWORD   "723C92442CA9"
#define WIFI_TIMEOUT_MS 20000 // 20 secs 


#define MAX_TAB_SIZE(a) (sizeof(a)/sizeof(a[0]))
#define WORST_RSSI       -90
#define UNDEFINED_RSSI   0xFF
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))


/*==============================================================================
  TYPES
  ==============================================================================
*/
typedef struct
{
  String  ssid; // SSID name
  int8_t  rssi; // RSSI value
  bool   isEncrypted;
} WifiInfo_t;

typedef enum
{
  MODEM_NOT_ACTIVE,
  MODEM_ACTIVE
} ModemState_t;
/*==============================================================================
  LOCAL FUNCTION PROTOTYPES
  ==============================================================================
*/
// State machine prototypes
static void stateIdle();
static bool transitionIdleScan();
static void stateWifiScan();
static bool transitionScanIdle();
static bool transitionScanConnect();
static void stateWifiConnect();
static bool transitionConnectIdle();
static bool transitionConnectBind();
static void stateNetworkConnect();
static bool transitionBindIdle();
/*==============================================================================
  GLOBAL DATA
  ==============================================================================
*/
static MYWIFI_PfWifiConnectState_t  _pWifiConnectStateCb;
static uint32_t                     _chipId;
static uint8_t                     _numberofScannedSSID;
static ModemState_t                _wifiModemState;
static WIFI_ConnectState_t         _wifiConnectState;
WifiInfo_t* _pBestScannedNetwork = NULL;

// State _machine
StateMachine _machine = StateMachine();
// Add states to the _machine
State* StateIdle;
State* StateScan;
State* StateConnect;
State *StateBind;

static char    _ssidBuf[20];

WifiInfo_t _wifiInfo[] =
{
  {"ImhobFamilie", 0 , false},
  {"ImhobFamilie_Ext", 0 , false}
};


/*==============================================================================
  CONST DATA
  ==============================================================================
*/

/*==============================================================================
  INTERFACE FUNCTIONS
  ==============================================================================
*/
//------------------------------------------------------------------------------
void MYWIFI_init(void)
{
  // Init data
  _wifiModemState = MODEM_NOT_ACTIVE;
  _pWifiConnectStateCb = NULL;
  _chipId = 0;
  _numberofScannedSSID = 0;
  // Add states to the _machine
  StateIdle =    _machine.addState(&stateIdle);
  StateScan =    _machine.addState(&stateWifiScan);
  StateConnect = _machine.addState(&stateWifiConnect);
  StateBind =    _machine.addState(&stateNetworkConnect);
}

//------------------------------------------------------------------------------
void MYWIFI_start(void)
{
  for (int i = 0; i < 17; i = i + 8) {
    _chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  if (_chipId != 0) {
    _wifiModemState = MODEM_ACTIVE;
  }
  StateIdle->addTransition(&transitionIdleScan, StateScan);
  StateScan->addTransition(&transitionScanIdle, StateIdle);
  StateScan->addTransition(&transitionScanConnect, StateConnect);
  StateConnect->addTransition(&transitionConnectIdle, StateIdle);
  StateConnect->addTransition(&transitionConnectBind, StateBind);
  StateBind->addTransition(&transitionBindIdle, StateIdle);
}

//------------------------------------------------------------------------------
void  MYWIFI_registerWifiConnectState(MYWIFI_PfWifiConnectState_t wifiConnectStateCb)
{
  if (wifiConnectStateCb != NULL)
  {
    _pWifiConnectStateCb = wifiConnectStateCb;
  }
}

//------------------------------------------------------------------------------
void  MYWIFI_handle(void)
{
  _machine.run();
}



/*==============================================================================
   LOCAL FUNCTIONS
  ==============================================================================
*/
//------------------------------------------------------------------------------
// Description here
static WifiInfo_t*  scanNetwork(void)
{
  uint8_t i, j;
  uint8_t index;

  Serial.println("Start scanning");

  // WiFi.scanNetworks will return the number of networks found
  _numberofScannedSSID = WiFi.scanNetworks();
  Serial.println("Scan done!");
  if (_numberofScannedSSID == 0)
  {
    Serial.println("no networks found");
    //TODO: Go to deep sleep
    // WiFi.deppSleep();
    return NULL;
  }
  // Network found?
  else
  {
    Serial.println(_numberofScannedSSID + " Networks were found.");
    for (i = 0 ; i < _numberofScannedSSID ; i++)
    {
      String ssid = WiFi.SSID(i);
      for (j = 0; j < MAX_TAB_SIZE(_wifiInfo) ; j++)
      {
        if (ssid == _wifiInfo[j].ssid)
        {
          Serial.println(ssid);
          _wifiInfo[j].rssi = WiFi.RSSI(i);
          _wifiInfo[j].isEncrypted = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? false : true;
          break;
        }
      }
    }
  }
  // Check scan results
  for (j = 0; j < MAX_TAB_SIZE(_wifiInfo); j++)
  {
    int8_t minRssiStart = -90;
    int8_t minRssiEnd;
    minRssiEnd = MAX(_wifiInfo[j].rssi , minRssiStart);
    if ((minRssiEnd != minRssiStart) && (_wifiInfo[j].isEncrypted))
    {
      minRssiStart = minRssiEnd;
      index = j;
    }
  }

  return &_wifiInfo[index];
}

//------------------------------------------------------------------------------
// Description here
static void connectToWifi(WifiInfo_t* pBestNetwork)
{
  unsigned long startAttemp; //TODO: To be checked
  // put your setup code here, to run once:
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  // Wait ...
  delay(100);

  //WiFi.begin(myString,WIFI_PASSWORD);
  pBestNetwork->ssid.toCharArray(_ssidBuf, sizeof(_ssidBuf));
  WiFi.begin((const char*)_ssidBuf, WIFI_PASSWORD);

  if ((WiFi.status() != WL_CONNECTED) && (millis() - startAttemp < WIFI_TIMEOUT_MS)) {
    Serial.print(".");
    delay(100);
  }

  if ((WiFi.status() != WL_CONNECTED) && (millis() - startAttemp < WIFI_TIMEOUT_MS)) {
    Serial.println("Failed To Connect ...");
    _wifiConnectState = WIFI_MODEM_NOT_CONNECTED;
    _pWifiConnectStateCb(_wifiConnectState);
  }
  else
  {
    Serial.println("Connected!");
    _wifiConnectState = WIFI_MODEM_CONNECTED;
    _pWifiConnectStateCb(_wifiConnectState);
  }

}

//------------------------------------------------------------------------------
static void stateIdle() {
  Serial.println("State idle");
}

//------------------------------------------------------------------------------
static bool transitionIdleScan() {
  if (_wifiModemState == MODEM_ACTIVE) {
    return true;
  }
  else
  {
    return false;
  }
}
//------------------------------------------------------------------------------
static void stateWifiScan() {
  Serial.println("stateWifiScan");
  // Scan for networks
  _pBestScannedNetwork = scanNetwork();
}
//------------------------------------------------------------------------------
static bool transitionScanIdle() {
  if (_pBestScannedNetwork == NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//------------------------------------------------------------------------------
static bool transitionScanConnect() {
  if (_pBestScannedNetwork != NULL) {
    return true;
  }
  else {
    return false;
  }
}
//------------------------------------------------------------------------------
static void stateWifiConnect() {
  static bool _isFirstEntry = true;
  Serial.println("stateWifiConnect");
  if (_isFirstEntry) {
    _isFirstEntry = false;
  //  startAttemp = millis();
  }
  // Start connecting
  connectToWifi(_pBestScannedNetwork);
}
//------------------------------------------------------------------------------
static bool transitionConnectIdle() {
  if (_wifiConnectState == WIFI_MODEM_NOT_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//------------------------------------------------------------------------------
static bool transitionConnectBind() {
  if (_wifiConnectState == WIFI_MODEM_CONNECTED)
  {
    return true;
  }
  else
  {
    return false;
  }
}
//------------------------------------------------------------------------------
static void stateNetworkConnect() {
  Serial.println("stateNetworkConnect");
}
//------------------------------------------------------------------------------
static bool transitionBindIdle() {
  return true;
}