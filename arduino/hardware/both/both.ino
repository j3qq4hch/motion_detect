String device_ID = "040056";

#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <EEPROM.h>
#include "delays.h"
#include "settings.h"
#include "hardware.h"
#include "eeprom.h"
#include "hc12.h"
#include "functions.h"




boolean power_plugged = false;


/// Для прошивки датчика - поставьте // в начале строки
//#define reciever true


//Раскоментируйте  следующую строку, если нужно перепроишть hc-12, неуспешно прошитую ранее микроконтроллером (влияет на hc12_init)
//#define reflash true

//  ID прошиваемого устройства




  #warning  Проверьте ID прошиваемого устройства, убедитесь что поставили актульный номер
#ifdef reciever
  #include "reciever.h"
  #warning !!!!!!!!   Внимание!!! Это прошивка для ___Приемника.  Закомментируйте строку выше  ____#define reciever_____ true для прошивки Датчика
#else
  #include "transmitter.h"
  #warning !!!!!!!!   Внимание!!! Это прошивка для ___Датчика.    Раскомемнтируйте строку выше  ____#define reciever_____ true для прошивки Приемника
#endif
