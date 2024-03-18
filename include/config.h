/*------------------------------------------------------------------------------
 * Copyright (c) Imed El Hadef
 * Any reproduction without written permission is prohibited by law.
 *------------------------------------------------------------------------------
 * Description :
 * App wifi 
 *------------------------------------------------------------------------------
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include "keys.h"
#include "storage.h"



class Config
{
private:
    String magic_key;
   // uint32_t CRC;
   String wifi_ssid;
   String wifi_pwd;
   uint32_t ble_pwd;
   bool     ble_config_status;
   uint16_t delimiter; // IMED: It's not a key it's a just a delimiter for the config area and will be used in case of config growup + config struct should be aligned

public:
    Config(String mkey = MAGIC_VAL, String ssid = SSID_VAL, String wPwd = WIFI_PWD_VAL, uint32_t bPwd  = BLE_PWD_VAL, bool bStatus = false,uint16_t del = BLE_PWD_VAL);
    // Member Function of another class (storage) as Friend Function
    friend  bool Storage::configure(Config &configData);
    ~Config();
    String getMagicKey(void);
    String getSsid(void);
    String getWifiPwd(void);
    uint32_t getBlePwd(void);
    bool     getBleConfigStatus(void);
    uint16_t getDelimiter(void);
    
};



#endif