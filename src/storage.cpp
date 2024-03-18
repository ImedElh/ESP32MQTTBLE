/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "storage.h"
#include "config.h"


Preferences preferences;

Storage::Storage():_started(false){

}

Storage::~Storage(){

}

bool Storage::begin(const char * name) 
{
  //Create a new storage space on the flash memory with the credentials namespace.
  // false= able to read and write
  preferences.begin(name, false);

  // close the preferences 
  preferences.end();

  return true;
}

bool Storage::configure(Config &configData)
{
  // Create many keys by using the putType() methods.
  preferences.putString(MAGIC_K, configData.magic_key); 
  preferences.putString(WIFI_SSID_K, configData.wifi_ssid); 
  preferences.putString(WIFI_PWD_K,  configData.wifi_pwd);
  preferences.putUInt(BLE_PWD_K,configData.ble_pwd);
  preferences.putBool(BLE_CONFIG_STATUS_K,configData.ble_config_status);
  preferences.putUInt(DELIMITER_K,configData.delimiter);

  Serial.println("Needed Credentials Saved using Preferences");
  // close the preferences 
  preferences.end();

  return true;
}

#ifdef SKIP
bool Storage::getConfiguration(Config::Data *pData)
{

  pData->wifi_ssid = preferences.getString("ssid", ""); 
  pData->wifi_pwd = preferences.getString("password", "");

  // Create many keys by using the putType() methods.
  preferences.putString(MAGIC_K, pData->magic_key); 
  preferences.putString(WIFI_SSID_K, pData->wifi_ssid); 
  preferences.putString(WIFI_PWD_K,  pData->wifi_pwd);
  preferences.putUInt(BLE_PWD_K,pData->ble_pwd);
  preferences.putBool(BLE_CONFIG_STATUS_K,pData->ble_config_status);
  preferences.putUInt(DELIMITER_K,pData->delimiter);

  Serial.println("Needed Credentials Saved using Preferences");
  // close the preferences 
  preferences.end();

  return true;
}
#endif
#ifdef GET_CRED
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <Preferences.h>
#include "WiFi.h"

Preferences preferences;

String ssid;
String password;

void setup() {
  Serial.begin(115200);
  Serial.println();
  
  preferences.begin("credentials", false);
 
  ssid = preferences.getString("ssid", ""); 
  password = preferences.getString("password", "");

  if (ssid == "" || password == ""){
    Serial.println("No values saved for ssid or password");
  }
  else {
    // Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
    Serial.println(WiFi.localIP());  
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}
#endif