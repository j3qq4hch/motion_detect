volatile boolean sleeping = true;
volatile boolean button_pressed = false;
volatile boolean  wdt_start = false;
volatile unsigned long millis_from_button_pressed = 0;
volatile int prev_button_status = HIGH;
const unsigned int millis_to_wakeup = 4000;
const unsigned int debounce_microseconds = 20000;
const byte s2 = 0b000111;
void(* resetFunc) (void) = 0;
byte device_ID_length = device_ID.length();


void sleep_if_button_5s_pressed() {
  if (digitalRead(button_pin) == LOW) {
    delay(5000);
    if (digitalRead(button_pin) == LOW) {
      pinMode(alarm_pin, OUTPUT);
      tone(alarm_pin, Sol);
      sleep_delay(mSLEEP_120MS);
      tone(alarm_pin, Mi);
      sleep_delay(mSLEEP_120MS);
      tone(alarm_pin, Do);
      sleep_delay(mSLEEP_120MS);
      noTone(alarm_pin);
      pinMode(alarm_pin, INPUT);
      resetFunc();
    }
  }




}
void button_2_isr() {
  wdt_start = true;
  sleep_disable();
  detachInterrupt (1);
}



void t() {
  pinMode(alarm_pin, OUTPUT);
  tone(alarm_pin, 400);
  delay(100);
  noTone(alarm_pin);
  delay(100);
}

void power_down_while_button_pressed_2s() {
  byte adcsra_save = ADCSRA;
  for (int i = 0; i < 20; i++ ) {
    pinMode(i, OUTPUT);
  }
  pinMode(red_led_pin, LOW);
  pinMode(button_pin, INPUT);
  wdt_disable();
  while (sleeping) {


    noInterrupts ();
    sleep_enable();

    attachInterrupt (1, button_2_isr, LOW);
    MCUSR = 0;
    noInterrupts ();
    // сбрасываем различные флаги
    byte adcsra_save = ADCSRA;
    ADCSRA = 0;  // запрещаем работу АЦП
    //Disable ADC
    power_all_disable ();   // выключаем все модули
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
    sleep_enable();
    interrupts ();
    sleep_cpu ();
    sleep_disable();
    power_all_enable();
    delay(millis_to_wakeup);
    if (digitalRead(button_pin) == LOW) {
      sleeping = false;
    }
  }
  t();
  detachInterrupt (digitalPinToInterrupt(button_pin));     // останавливаем прерывание LOW
  ADCSRA = adcsra_save;  // останавливаем понижение питания
  power_all_enable ();   // включаем все модули

}


void goto_sleep() {
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Re);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Do);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
  resetFunc();
}

String getId(String input) {
  String s = "";
  int i;
  for (i = 1; i < device_ID_length + 1; i++) {
    s += input.charAt(i);
  }
  return s;
}

boolean isValidNumber(String str) {
  for (byte i = 1; i < device_ID_length + 1; i++)
  {
    if (isDigit(str.charAt(i))) return true;
  }
  return false;
}

void check_leds() {
  blink_green(500);
  blink_red(500);
  
}
