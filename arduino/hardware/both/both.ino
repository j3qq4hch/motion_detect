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

// Для прошивки 
#define reciever true

String device_ID = "040055";
boolean power_plugged = false;


#ifdef reciever
  #include "reciever.h"
  #warning !!!!!!!!   Прошивка для приемника c ID device_ID

#else
  #include "transmitter.h"
#endif
