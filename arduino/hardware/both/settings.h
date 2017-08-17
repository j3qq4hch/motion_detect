



#define hc12_setted_eeprom_address 0
#define transmitter_ID_eeprom_address 2

#define bluetooth_name_prefix String("GRUZ_")

#define blink_duration      mSLEEP_500MS
#define blink_long_duration mSLEEP_2S
#define blink_short_duration mSLEEP_120MS

#define Do 261
#define Re 294
#define Mi 330
#define Fa 349
#define Sol 392
#define La 440
#define Si 494

#define looking_for_transmitter_cmd     'R'
#define polling_cmd     'P'
#define pir_cmd         'A'
#define id_cmd          'I'
#define no_polling_cmd  'N'
#define no_polling_stopped_cmd  'S'
#define hc12_SLEEP     "AT+SLEEP\r\n"
#define hc12_SET_DEFAULT   "AT+DEFAULT\r\n"
#define hc12_SET_FU    "AT+FU4\r\n"
#define hc12_SET_CH    "AT+C055\r\n"
#define hc12_SET_PDB   "AT+P8\r\n"
#define hc12_WAKEUP    "AT\r\n"

#define bluetooth_name_cmd "AT+NAME"
#define bluetooth_sleep_cmd "AT+SLEEP"
#define bluetooth_auto_sleep_cmd "AT+PWRM0"

#define AT_RESPONSE_DELAY mSLEEP_500MS    //Delay after sending AT cmd to HC-12
#define AT_EXIT_DELAY     mSLEEP_250MS     //Delay after  setting SET pin of HC-12 to LOW or HIGH
#define hc12_SEND_DELAY  mSLEEP_1S    //Delay after sending data (for FU4 - minimum 1s)


#define  battery_high_voltage   4.0
#define  battery_medium_voltage   3.7
#define  battery_low_voltage   3.5

unsigned long polling_timeout = 15UL * 60UL * 1000UL; //UL - окончания для unsigned long, иначе проблема
//unsigned long polling_timeout = 20UL * 1UL * 1000UL; //UL - окончания для unsigned long, иначе проблема
//const byte s8 = 0b100001;

