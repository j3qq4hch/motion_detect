SoftwareSerial bluetooth(hm10_tx_pin, hm10_rx_pin);
const String r_firmware_ver        = "f0.2";
byte life_counter = 0;
String sensor_init_string = "";
unsigned int last_polling = 0;
String transmitter_ID;
unsigned long int powerbank_last_activation_time = 0;
unsigned long int powerbank_activation_interval = 5L * 3600L * 1000L; //5 hours

void idle_1s() {
 
  byte adcsra_save = ADCSRA;

  
   cli(); 
    MCUSR = 0;   
  WDTCSR |= 0b00011000;               // устанавливаем WDCE, WDE
  WDTCSR =  0b01000000 | 0b000110;   //1s
  wdt_reset(); 

  
  ADCSRA = 0;  // запрещаем работу АЦП
   
  
  set_sleep_mode(SLEEP_MODE_IDLE); // если спать - то на полную
  sei(); //interrupts ();
  sleep_mode();
  power_all_enable();
  wdt_reset();
  sleep_disable();

  ADCSRA = adcsra_save;  
  
}

void no_polling() {
  bluetooth.print(no_polling_cmd + transmitter_ID);
  tone(alarm_pin, 2000);
  sleep_delay(mSLEEP_250MS);
  noTone(alarm_pin);
  last_polling = millis();
}

void try_to_add_new_transmitter() {
  boolean flag = true;
  while (digitalRead(button_pin) == LOW && flag) {
    tone(alarm_pin, 300);
    sleep_delay(mSLEEP_120MS);
    noTone(alarm_pin);
    sleep_delay(mSLEEP_250MS);
    if (Serial.available()) {
      String s = Serial.readString();
      sleep_delay(mSLEEP_120MS);
      if (s.charAt(0) == id_cmd && isValidNumber(s)) {
        transmitter_ID = getId(s);
        write_StringEE(transmitter_ID_eeprom_address, transmitter_ID);
        flag = false;
        sensor_started();
      }
    }

  }
}

volatile boolean life = false;


void polling_ok() {
  tone(alarm_pin, Do);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Re);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
}

void alarm() {
  int i = 0;
  int k = 0;
  int j = 0;
  boolean flag = true;
  pinMode(alarm_pin,OUTPUT);
  prepare_led_pins();
  //  led_on(red_pin);
  for (k = 0; k < 3; k++) {
    j = 0;
    if (flag) {
      tone(alarm_pin, 300, 200);
      while (j < 300) {
        if (digitalRead(button_pin) == LOW) {
          flag = false;
          break;
          j = 301;
        }
        sleep_delay(mSLEEP_15MS);
        j++;
      }
    }
  }

  while (flag) {
    led_on(red_led_pin);
    for (i = 300; i < 500; i++) {
      if (digitalRead(button_pin) == LOW) {
        flag = false;
        break;
      }
      tone(alarm_pin, i);
      sleep_delay(mSLEEP_15MS);
    }

    led_off(red_led_pin);
    for (i = 500; i > 300; i--) {
      if (digitalRead(button_pin) == LOW) {
        flag = false;
        break;
      }
      tone(alarm_pin, i);
      sleep_delay(mSLEEP_15MS);
    }
  }
  noTone(alarm_pin);
  pinMode(alarm_pin,INPUT);

}

void prepare_after_wake_up() {
  pinMode(hc12_set_pin, OUTPUT);
  pinMode(alarm_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  digitalWrite(alarm_pin, LOW);
  hc12_init();
  String s = read_StringEE(transmitter_ID_eeprom_address, 5);
  if (!isValidNumber(s)) {
    transmitter_ID = device_ID;
    write_StringEE(transmitter_ID_eeprom_address, transmitter_ID);
  }
  else {
    transmitter_ID = s;
  }
  if (digitalRead(button_pin) == LOW) {
    try_to_add_new_transmitter();
  }
  bluetooth.begin(9600);
  sleep_delay(mSLEEP_250MS);
  bluetooth.println(bluetooth_name_cmd + bluetooth_name_prefix + device_ID);
  sleep_delay(mSLEEP_250MS);
  bluetooth.println(bluetooth_auto_sleep_cmd);
  Serial.print(looking_for_transmitter_cmd + transmitter_ID);
}

void prepare_after_unsleep() {
  bluetooth.begin(9600);
  blink_red(blink_duration);
  show_battery_status();
  last_polling = millis();
}

void setup() {
  power_down_while_button_pressed_2s();
  prepare_after_wake_up();
  prepare_after_unsleep();
  powerbank_last_activation_time = millis();
}
void loop() {
  if (millis() - powerbank_last_activation_time > powerbank_activation_interval) {
    activate_power_bank();
    powerbank_last_activation_time = millis();
  }
  if ((millis() - last_polling) > polling_timeout) {
    no_polling();
  }
  if (life) {
    life = false;
    blink_red(blink_short_duration);
  }
  if (bluetooth.available()) {
    String bls = bluetooth.readString();
    if (bls == (pir_cmd + transmitter_ID)) {
      alarm();
    }
    if (bls == "voltage") {
      bluetooth.print("power" + (String) + digitalRead(power_plugged_pin) + 'v' + (String) battery_voltage() + sensor_init_string + r_firmware_ver);
    }
    if (bls == "ping") {
      Serial.print("ping" + transmitter_ID);
    }
  }
  if (Serial.available()) {
    String s = Serial.readString();
    if (s.indexOf(pir_cmd + transmitter_ID) > -1) {
      last_polling = millis();
      bluetooth.print(s);
      alarm();
    }
    if (s.indexOf(id_cmd + transmitter_ID) > -1) {
      last_polling = millis();
      bluetooth.print(s);
      sensor_init_string = s;
      sensor_started();
    }
    if (s.indexOf(polling_cmd + transmitter_ID) > -1) {
      last_polling = millis();
      bluetooth.print(s);
      polling_ok();
    }
  }
  sleep_if_button_5s_pressed();
  life_counter++;
  if (life_counter == 8) {
    life = true;
    life_counter = 0;
  }
 //  idle_1s();
}




