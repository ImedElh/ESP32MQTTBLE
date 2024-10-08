/*------------------------------------------------------------------------------
  Copyright (c) 2022 Imed El Hadef
  Any reproduction without written permission is prohibited by law.
  ------------------------------------------------------------------------------
  Description :
  Implementation of the ESP32 WIFI connection module
  ------------------------------------------------------------------------------
*/
#include <Arduino.h>
#include "WiFi.h"
#include "mywifi.h"
#include "network.h"
#include "srvble.h"
#include "config.h"
#include "storage.h"



/*==============================================================================
  MACROS
  ==============================================================================
*/

/*==============================================================================
  TYPES
  ==============================================================================
*/

/*==============================================================================
  LOCAL FUNCTION PROTOTYPES
  ==============================================================================
*/

/*==============================================================================
  GLOBAL DATA
  ==============================================================================
*/
Config myConfig("XXX", "XXX", "XXX", 0x0, false,0x0);
Storage myStorage;
/*==============================================================================
  CONST DATA
  ==============================================================================
*/
const Config  defConfig;
/*==============================================================================
  INTERFACE FUNCTIONS
  ==============================================================================
*/


/*==============================================================================
   LOCAL FUNCTIONS
  ==============================================================================
*/
//------------------------------------------------------------------------------
// Description here
//------------------------------------------------------------------------------

void setup() {

  Serial.begin(115200);
  /*  delay(2000);
  MYWIFI_init();
  NETWORK_init();

  MYWIFI_start();
  NETWORK_start();
  */
  myStorage.begin("MYCONFIG");
  myStorage.getConfiguration(&myConfig);
  // Not configured yet?
  if(myConfig.getMagicKey() != MAGIC_VAL)
  {
   Serial.println("Config data already set");
   myStorage.configure(defConfig);
  }
  myStorage.end("MYCONFIG");

 

  SRVBLE_init();

}

void loop() {
  // put your main code here, to run repeatedly:
 /* MYWIFI_handle();
  NETWORK_handle();
*/
  SRVBLE_loop();
}