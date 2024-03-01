/*------------------------------------------------------------------------------
 * Copyright (c) Imed El Hadef
 * Any reproduction without written permission is prohibited by law.
 *------------------------------------------------------------------------------
 * Description :
 * App wifi 
 *------------------------------------------------------------------------------
 */
#ifndef NETWORK_H_
#define NETWORK_H_
#include <Arduino.h>
/*==============================================================================
 * MACROS
 *==============================================================================
 */

 /*==============================================================================
 * TYPES
 *==============================================================================
 */
 /*==============================================================================
 * FUNCTIONS
 *==============================================================================
 */

extern void NETWORK_init(void);
extern void NETWORK_start(void);
extern void NETWORK_handle(void);

#endif