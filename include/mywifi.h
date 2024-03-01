#ifndef MYWIFI_H_
#define MYWIFI_H_
/*------------------------------------------------------------------------------
 * Copyright (c) Imed El Hadef
 * Any reproduction without written permission is prohibited by law.
 *------------------------------------------------------------------------------
 * Description :
 * App wifi 
 *------------------------------------------------------------------------------
 */
#include <Arduino.h>
/*==============================================================================
 * MACROS
 *==============================================================================
 */

 /*==============================================================================
 * TYPES
 *==============================================================================
 */
 typedef enum
 {
  WIFI_MODEM_NOT_CONNECTED,
  WIFI_MODEM_CONNECTED
 } WIFI_ConnectState_t;
 
typedef void (*MYWIFI_PfWifiConnectState_t)(WIFI_ConnectState_t wifiConnectState);
 /*==============================================================================
 * FUNCTIONS
 *==============================================================================
 */
//extern void  MYWIFI_getWifiStatus(void);
extern void  MYWIFI_init(void);
extern void  MYWIFI_start(void);
extern void  MYWIFI_registerWifiConnectState(MYWIFI_PfWifiConnectState_t wifiConnectStateCb);
extern void  MYWIFI_handle(void);

#endif