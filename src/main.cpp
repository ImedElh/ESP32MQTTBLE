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

/*==============================================================================
  CONST DATA
  ==============================================================================
*/

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
  delay(2000);
  MYWIFI_init();
  NETWORK_init();

  MYWIFI_start();
  NETWORK_start();

}

void loop() {
  // put your main code here, to run repeatedly:
  MYWIFI_handle();
  NETWORK_handle();

}