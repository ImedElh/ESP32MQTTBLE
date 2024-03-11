#include <Arduino.h>
#include "keys.h"

//Default Configurations
#define CFG_CONFIG_OK                    0x1234
#define CFG_DELIMITER_OK                 0xABCD      //IMED note: that should not be a valid number of any used items in the configuratio list

#define MIN(a,b) ((a) < (b) ? (a) : (b))

class Config
{
private:
  static inline bool dirty{ false };
	
public:
  static inline int LastValueSize{ 1 };

  template <typename T, unsigned int N = 1>
struct __attribute__ ((__packed__)) key
{
  public:
    T V[N];  
  private:
    //bool empty;
    T Get ()
    {
        return V[0];
    }
    void Set (String v)
    {
        if (strncmp (V, v,v.length()))
        {
            strncpy (V, v, MIN(N,v.length()));
            dirty = true;
        }
    }
    void Set (const char *v)
    {
        if (strcmp (V, v))
        {
            strncpy (V, v, N);
            dirty = true;
        }
    }
    void Set (T v)
    {
        if (v != V[0])
        {
            V[0] = v;
            dirty = true;
        }
    }
	
  public:

    bool isDefault()
    {
        auto offset = (uint8_t *)&V - (uint8_t *)&data;
        auto size = sizeof (V);
        for (int i = 0; i < size; i++)
        {
            #ifdef SKIP
            auto b = ((uint8_t *)&DefaultValues)[i + offset];
            auto c = ((uint8_t *)&V)[i];
	        if (c != b)
	        {
		        return false;
	        }
            #endif
        }
        return true;
    }
    T * GetNewValueOnly ()
    {
        //static_assert (sizeof (T) > 1, "Cannot use GetNewValueOnly with string keys. Use  GetNewString instead");
        LastValueSize = sizeof (T);
        return isDefault () ? NULL : V;

    }	
    T * GetValue()
    {
	    LastValueSize = sizeof(T);
	    return V;

    }
};

struct __attribute__ ((__packed__)) Data
{
    String magic_key;
   // uint32_t CRC;
   // key<uint16_t> motion_interval_minutes;
   String wifi_ssid;
   String wifi_pwd;
   uint32_t ble_pwd;
   bool     ble_config_status;
   #ifdef CONFIG_NETWORK
    key<char, 51> tcp_url_main;
    key<char, 51> tcp_url_backup;
    key<uint16_t> tcp_port_backup;
    key<char, 51> tenerife_ftp_url;
    key<uint16_t> tenerife_ftp_port;
    key<char, 16> tenerife_ftp_user;
    key<char, 16> tenerife_ftp_pw;
    key<char, 51> malta_ftp_url;
    key<uint16_t> malta_ftp_port;
    key<char, 16> malta_ftp_user;
    key<char, 16> malta_ftp_pw;
    key<char, 51> udp_url_main;
    key<char, 51> udp_url_backup;
    #endif
	uint16_t delimiter; // IMED: It's not a key it's a just a delimiter for the config area and will be used in case of config growup + config struct should be aligned
 };
 static inline Data data;  //= (Data *)START_ADDRESS_CONFIGURATION;
};

 const Config::Data DefaultValues {
	 .magic_key = MAGIC_VAL,
     .wifi_ssid = SSID_VAL,
     .wifi_pwd =  WIFI_PWD_VAL,
     .ble_pwd = BLE_PWD_VAL,
     .ble_config_status = BLE_CONFIG_STATUS_VAL,
     .delimiter = CFG_DELIMITER_OK,
 };