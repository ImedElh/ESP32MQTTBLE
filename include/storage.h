/*------------------------------------------------------------------------------
 * Copyright (c) Imed El Hadef
 * Any reproduction without written permission is prohibited by law.
 *------------------------------------------------------------------------------
 * Description :
 * App wifi 
 *------------------------------------------------------------------------------
 */
#ifndef STORAGE_H_
#define STORAGE_H_

#include "Arduino.h"
#include "config.h"

class Storage {
    protected:
      //  uint32_t _handle;
      //  bool _started;
      //  bool _readOnly;
      bool _started;
    public:
        Storage();
        ~Storage();

        bool begin(const char * name);
        bool configure(const Config::Data *pData);
        void end();
        bool clear();
        bool remove(const char * key);
        size_t putValue(const char* key, String value);
        size_t getValue(const char* key, String value = String());
        bool isKey(const char* key);
        size_t freeEntries();
};
#endif