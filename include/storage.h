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

#include <Preferences.h>
#include "Arduino.h"


class Config;   // forward definition needed

class Storage {
    protected:
      Preferences preference;
      bool _started;
      String name;
    public:
        Storage();
        ~Storage();
        bool begin(const char * name);
        bool configure(const Config &configData);
        bool getConfiguration(Config *pconfigData);
        bool end(const char * name);
        bool clear();
        bool remove(const char * key);
        size_t putValue(const char* key, String value);
        size_t getValue(const char* key, String value = String());
        bool isKey(const char* key);
        size_t freeEntries();
};
#endif