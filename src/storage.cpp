/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-save-data-permanently-preferences/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/


#include "storage.h"
#include "config.h"




Storage::Storage():_started(false),name(""){

}

Storage::~Storage(){

}

bool Storage::begin(const char * name) 
{
  //Create a new storage space on the flash memory with the credentials namespace.
  // false= able to read and write
  preference.begin(name, false);
 // this->name = name;
  this->name = name;

  // close the preferences 
  //preference.end();

  return true;
}

bool Storage::configure(const Config &configData)
{
  // Create many keys by using the putType() methods.
  preference.putString(MAGIC_K, configData.magic_key); 
  preference.putString(WIFI_SSID_K, configData.wifi_ssid); 
  preference.putString(WIFI_PWD_K,  configData.wifi_pwd);
  preference.putUInt(BLE_PWD_K,configData.ble_pwd);
  preference.putBool(BLE_CONFIG_STATUS_K,configData.ble_config_status);
  preference.putUInt(DELIMITER_K,configData.delimiter);

  Serial.println("Needed Credentials Saved using Preferences");


  return true;
}


bool Storage::getConfiguration(Config *pconfigData)
{
  // Create all configured data by using preferences.getXXX() methods.
  pconfigData->magic_key = preference.getString(MAGIC_K, ""); 
  pconfigData->wifi_ssid = preference.getString(WIFI_SSID_K, ""); 
  pconfigData->wifi_pwd = preference.getString(WIFI_PWD_K, "");
  pconfigData->ble_pwd =  preference.getUInt(BLE_PWD_K, 0);
  pconfigData->ble_config_status =  preference.getBool(BLE_CONFIG_STATUS_K, 0);
  pconfigData->delimiter =  preference.getUInt(DELIMITER_K, 0);

  Serial.println("Get all saved credentials");
  // close the preferences 
 // preference.end();

  return true;
}

bool Storage::end(const char *name)
{
  if (strcmp(this->name.c_str(), name) == 0 )
  {
    // close the preferences 
    preference.end();
  }
  return true;
}

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