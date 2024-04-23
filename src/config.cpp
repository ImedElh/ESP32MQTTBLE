/*------------------------------------------------------------------------------
  Copyright (c) 2024 Imed El Hadef
  Any reproduction without written permission is prohibited by law.
  ------------------------------------------------------------------------------
  Description :
  Implementation of the ESP32 Configuration module
  ------------------------------------------------------------------------------
*/
#include "config.h"
/*==============================================================================
  MACROS
  ==============================================================================
*/


Config::Config(String mkey, String ssid , String wPwd, uint32_t bPwd, bool bStatus ,uint16_t del ):
 magic_key{mkey},wifi_ssid{ssid}, wifi_pwd{wPwd}, ble_pwd{bPwd}, ble_config_status{bStatus},delimiter{del}
 {
   // cout<<"Default Constructor called for " + name <<endl; 
 }

Config:: ~Config()
{}

String Config::getMagicKey(void)
{
    return magic_key;
}

