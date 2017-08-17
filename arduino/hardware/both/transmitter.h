
const String firmware_ver = "0.1";

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

void sleep_and_react (const byte interval) {
  if (pir_alarm && digitalRead(pir_pin) == HIGH )
  {
    if (!polling) {
      detachInterrupt (0);
      hc12_wakeup();
      Serial.print(pir_cmd + device_ID);
      delay(hc12_SEND_DELAY);
      hc12_sleep();
      blink_red(blink_short_duration);
      pir_alarm = false;
    }
  }
  else {
    blink_red(blink_short_duration);
    if (polling) {
      polling = false;
      s = s8;
    }
    sleeps_count += 1;
    if (sleeps_count > sleep_cycles) {
      polling = true;
      sleeps_count = 0;
      hc12_wakeup();
      Serial.print(polling_cmd + device_ID + 'b' + battery_voltage());
      delay(hc12_SEND_DELAY);
      hc12_sleep();
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
//    attachInterrupt (1, button_interrupt, FALLING);   // позволяем заземлить pin 2 для выхода из сна
  }
  interrupts ();
  sleep_cpu ();
  // переходим в сон и ожидаем прерывание
  detachInterrupt (0);     // останавливаем прерывание LOW
  ADCSRA = adcsra_save;  // останавливаем понижение питания
  power_all_enable ();   // включаем все модули
  if (pass_first_alarms > 0) {
    pass_first_alarms--;
    led_on(red_led_pin);
  }
  else {
    led_off(red_led_pin);
  }

}

void setup () {
  power_down_while_button_pressed_2s();
  led_on(red_led_pin);

  pinMode(pir_pin, INPUT);
  pinMode(power_plugged_pin, INPUT);
  pinMode(button_pin, INPUT);

  digitalWrite(pir_pin, LOW);

  hc12_init();
  Serial.print(id_cmd + device_ID + 'b' + battery_voltage() + firmware_ver);
  delay(hc12_SEND_DELAY);
  hc12_sleep();
  check_leds();
  show_battery_status();
  turn_5v_on();
}
void loop()
{
  sleep_and_react (s);
  sleep_if_button_5s_pressed();
}





