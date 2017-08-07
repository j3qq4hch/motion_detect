
const String firmware_ver='0.1';
const byte s8 = 0b100001;
const byte s4 = 0b100000;
const byte s2 = 0b000111;
const byte s1 = 0b000110;

const byte s0200 = 0b000100;
#ifdef DEBUG
int pass_first_alarms = 1; // (2min=120sec-5 ~startup_time / 8s)
#else
int pass_first_alarms = 13; // (2min=120sec-5 ~startup_time / 8s)
#endif

byte s = s8;

volatile boolean pir_alarm = false;
volatile int sleeps_count = 0;
boolean polling = false;
const int sleep_interval = 10 * 60 ; //sec
const int sleep_cycles = (int) sleep_interval / 8;
unsigned long time;

void pir_interrupt () {
  pir_alarm = true;
  wdt_disable();
}



// прерывание сторожевого таймера
ISR (WDT_vect)
{
  wdt_disable();  // отключаем сторожевой таймер
}

void sleep_and_react (const byte interval)

{

  if (digitalRead(power_plugged_pin) == HIGH) {
    usb_plugged = true;
  } else {
    usb_plugged = false;
  }


  if (!usb_plugged) {
    if (pir_alarm && digitalRead(pir_pin) == HIGH )
    {

      if (!polling) {
        detachInterrupt (0);
        hc_12_wakeup();
        Serial.print(pir_cmd + device_ID);
        delay(HC_12_SEND_DELAY);
        hc_12_sleep();
        blink_red(blink_fast_duration);
        pir_alarm = false;
      }
    }
    else {
      blink_green(blink_fast_duration);
      if (polling) {
        polling = false;
        s = s8;
      }
      sleeps_count += 1;
      if (sleeps_count > sleep_cycles) {
        polling = true;
        sleeps_count = 0;
        hc_12_wakeup();
        Serial.print(polling_cmd + device_ID + 'b' + battery_voltage());
        delay(HC_12_SEND_DELAY);
        hc_12_sleep();
        s = s8;
      }
    }

    noInterrupts ();
    MCUSR = 0;                          // сбрасываем различные флаги
    WDTCSR |= 0b00011000;               // устанавливаем WDCE, WDE
    WDTCSR =  0b01000000 | interval;    // устанавливаем WDIE, и соответсвующую задержку
    wdt_reset();

    byte adcsra_save = ADCSRA;
    ADCSRA = 0;  // запрещаем работу АЦП
    //Disable ADC
    power_all_disable ();   // выключаем все модули
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
    sleep_enable();
    if (pass_first_alarms == 0 && !polling) {
      attachInterrupt (0, pir_interrupt, RISING);   // позволяем заземлить pin 2 для выхода из сна
    }


    interrupts ();
    sleep_cpu ();

    // переходим в сон и ожидаем прерывание
    detachInterrupt (0);     // останавливаем прерывание LOW
    ADCSRA = adcsra_save;  // останавливаем понижение питания
    power_all_enable ();   // включаем все модули
    if (pass_first_alarms > 0) {
      pass_first_alarms--;
      led_on(red_pin);
    }
    else {
      led_off(red_pin);
    }
    if (usb_plugged) {
      s = s1;
    }
    else {
      s = s8;
    }
  }
  else
  {
    led_on(green_pin);
    usb_plugged = digitalRead(power_pin) == HIGH;
  }
}
float battery_voltage() {
  return analogRead(battery_pin) / 1024.0 * 5 * 1.014;
}

void prepareLedPins () {
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(led_common_pin, OUTPUT);
  digitalWrite(led_common_pin, led_common);
}
void releaseLedPins() {
  pinMode(red_pin, INPUT);
  pinMode(green_pin, INPUT);
  pinMode(led_common_pin, INPUT);

}
void blink_red(int duration) {
  prepareLedPins();
  digitalWrite(red_pin, led_pin);
  delay(duration);
  digitalWrite(red_pin, !led_pin);
  releaseLedPins();

}

void blink_green(int duration) {
  prepareLedPins();
  digitalWrite(green_pin, led_pin);
  delay(duration);
  digitalWrite(green_pin, !led_pin);
  releaseLedPins();
}

void blink_yellow(int duration) {
  prepareLedPins();
  digitalWrite(red_pin, led_pin);
  digitalWrite(green_pin, led_pin);
  delay(duration);
  digitalWrite(red_pin, !led_pin);
  digitalWrite(green_pin, !led_pin);
  releaseLedPins();
}

void led_on(int pin) {
  prepareLedPins();
  digitalWrite(pin, led_pin);
  releaseLedPins();
}
void led_off(int pin) {
  prepareLedPins();
  digitalWrite(pin, !led_pin);
  releaseLedPins();
}
void battery_status(int duration) {
  float b = battery_voltage();
  if (b > 3.9) {
    blink_green(duration);
  } else if (b > 3.6) {
    blink_yellow(duration);
  }
  else {
    blink_red(duration);
  }
}





void setup () {


  pinMode(pir_pin, INPUT);
  pinMode(power_pin, INPUT);
  pinMode(button_pin, INPUT);

  digitalWrite(pir_pin, LOW);

  digitalWrite(led_common_pin, !led_common);

  blink_red(blink_duration);
  blink_green(blink_duration);
  blink_yellow(blink_duration);

  hc_12_init();
  Serial.print(id_cmd + device_ID + 'b' + battery_voltage());
  delay(HC_12_SEND_DELAY);
  hc_12_sleep();
  battery_status(blink_duration);
}


void loop()
{
  sleep_and_react (s);


}




boolean usb_plugged = false;
const byte s8 = 0b100001;
const byte s4 = 0b100000;
const byte s2 = 0b000111;
const byte s1 = 0b000110;

const byte s0200 = 0b000100;
#ifdef DEBUG
int pass_first_alarms = 1; // (2min=120sec-5 ~startup_time / 8s)
#else
int pass_first_alarms = 13; // (2min=120sec-5 ~startup_time / 8s)
#endif

byte s = s8;

volatile boolean pir_alarm = false;
volatile int sleeps_count = 0;
boolean polling = false;
const int sleep_interval = 10 * 60 ; //sec
const int sleep_cycles = (int) sleep_interval / 8;
unsigned long time;

void pir_interrupt () {
  pir_alarm = true;
  wdt_disable();
}



// прерывание сторожевого таймера
ISR (WDT_vect)
{
  wdt_disable();  // отключаем сторожевой таймер
}
void hc_12_wakeup() {
  pinMode(hc_12_set_pin, OUTPUT);
  digitalWrite(hc_12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  digitalWrite(hc_12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}
void hc_12_sleep() {
  pinMode(hc_12_set_pin, OUTPUT);
  digitalWrite(hc_12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  Serial.print(F(HC_12_SLEEP));
  delay(AT_RESPONSE_DELAY);
  digitalWrite(hc_12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}

void hc_12_init() {
  pinMode(hc_12_set_pin, OUTPUT);
  #ifdef reflash
  byte hc_12_setted = EEPROM.read(hc_12_setted_eeprom_address);
  if (hc_12_setted != 1) {
  #endif
    Serial.begin(9600);
    delay(HC_12_SEND_DELAY);
    digitalWrite(hc_12_set_pin, LOW);
    delay(AT_EXIT_DELAY);
    Serial.print(F(HC_12_SET_DEFAULT));
    delay(AT_RESPONSE_DELAY);
    Serial.print(F(HC_12_SET_CH));
    delay(AT_RESPONSE_DELAY);
    Serial.print(F(HC_12_SET_FU));
    delay(AT_RESPONSE_DELAY);
    Serial.print(F(HC_12_SET_PDB));
    delay(AT_RESPONSE_DELAY);
    digitalWrite(hc_12_set_pin, HIGH);
    delay(AT_EXIT_DELAY);
    EEPROM.write(hc_12_setted_eeprom_address, 1);
  #ifdef reflash
     }
  #endif
  digitalWrite(hc_12_set_pin, HIGH);
  Serial.begin(1200);
  
  delay(AT_EXIT_DELAY);
  Serial.print(id_cmd + device_ID + 'b' + battery_voltage()+firmaware_ver);
  delay(HC_12_SEND_DELAY);
}

/*
  unsigned long m1;
  unsigned long m2;
  unsigned long m3;
  unsigned long m4;
  unsigned long m5;
  unsigned long m6;
*/

void sleep_and_react (const byte interval)


{

  if (digitalRead(power_pin) == HIGH) {
    usb_plugged = true;
  } else {
    usb_plugged = false;
  }


  if (!usb_plugged) {
    if (pir_alarm && digitalRead(pir_pin) == HIGH )
    {

      if (!polling) {
        detachInterrupt (0);
        hc_12_wakeup();
        Serial.print(pir_cmd + device_ID);
        delay(HC_12_SEND_DELAY);
        hc_12_sleep();
        blink_red(blink_fast_duration);
        pir_alarm = false;
      }
    }
    else {
      blink_green(blink_fast_duration);
      if (polling) {
        polling = false;
        s = s8;
      }
      sleeps_count += 1;
      if (sleeps_count > sleep_cycles) {
        polling = true;
        sleeps_count = 0;
        hc_12_wakeup();
        Serial.print(polling_cmd + device_ID + 'b' + battery_voltage());
        delay(HC_12_SEND_DELAY);
        hc_12_sleep();
        s = s8;
      }
    }
    
#ifdef DEBUG
    Serial.println("preparing to sleep");
    delay(100);
#endif

    noInterrupts ();
    MCUSR = 0;                          // сбрасываем различные флаги
    WDTCSR |= 0b00011000;               // устанавливаем WDCE, WDE
    WDTCSR =  0b01000000 | interval;    // устанавливаем WDIE, и соответсвующую задержку
    wdt_reset();

    byte adcsra_save = ADCSRA;
    ADCSRA = 0;  // запрещаем работу АЦП
    //Disable ADC
    power_all_disable ();   // выключаем все модули
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
    sleep_enable();
    if (pass_first_alarms == 0 && !polling) {
      attachInterrupt (0, pir_interrupt, RISING);   // позволяем заземлить pin 2 для выхода из сна
    }


    interrupts ();
    sleep_cpu ();

    // переходим в сон и ожидаем прерывание
    detachInterrupt (0);     // останавливаем прерывание LOW
    ADCSRA = adcsra_save;  // останавливаем понижение питания
    power_all_enable ();   // включаем все модули
    if (pass_first_alarms > 0) {
      pass_first_alarms--;
      led_on(red_pin);
    }
    else {
      led_off(red_pin);
    }
#ifdef DEBUG
    delay(200);
    Serial.begin(1200);
    Serial.println(pass_first_alarms);
#endif
    if (usb_plugged) {
      s = s1;
    }
    else {
      s = s8;
    }
  }
  else
  {
    led_on(green_pin);
    usb_plugged = digitalRead(power_pin) == HIGH;
  }
}
float battery_voltage() {
  return analogRead(battery_pin) / 1024.0 * 5 * 1.014;
}

void prepareLedPins () {
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(led_common_pin, OUTPUT);
  digitalWrite(led_common_pin, led_common);
}
void releaseLedPins() {
  pinMode(red_pin, INPUT);
  pinMode(green_pin, INPUT);
  pinMode(led_common_pin, INPUT);

}
void blink_red(int duration) {
  prepareLedPins();
  digitalWrite(red_pin, led_pin);
  delay(duration);
  digitalWrite(red_pin, !led_pin);
  releaseLedPins();

}

void blink_green(int duration) {
  prepareLedPins();
  digitalWrite(green_pin, led_pin);
  delay(duration);
  digitalWrite(green_pin, !led_pin);
  releaseLedPins();
}

void blink_yellow(int duration) {
  prepareLedPins();
  digitalWrite(red_pin, led_pin);
  digitalWrite(green_pin, led_pin);
  delay(duration);
  digitalWrite(red_pin, !led_pin);
  digitalWrite(green_pin, !led_pin);
  releaseLedPins();
}

void led_on(int pin) {
  prepareLedPins();
  digitalWrite(pin, led_pin);
  releaseLedPins();
}
void led_off(int pin) {
  prepareLedPins();
  digitalWrite(pin, !led_pin);
  releaseLedPins();
}
void battery_status(int duration) {
  float b = battery_voltage();
  if (b > 3.9) {
    blink_green(duration);
  } else if (b > 3.6) {
    blink_yellow(duration);
  }
  else {
    blink_red(duration);
  }
}





void setup () {


  pinMode(pir_pin, INPUT);
  pinMode(power_pin, INPUT);
  pinMode(button_pin, INPUT);

  digitalWrite(pir_pin, LOW);

  digitalWrite(led_common_pin, !led_common);

  blink_red(blink_duration);
  blink_green(blink_duration);
  blink_yellow(blink_duration);

  hc_12_init();
  hc_12_sleep();
  battery_status(blink_duration);
}


void loop()
{
  sleep_and_react (s);


}




