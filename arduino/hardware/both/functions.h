volatile boolean sleeping = true;

volatile unsigned long millis_from_button_pressed = 0;
volatile int prev_button_status = HIGH;
const unsigned int millis_to_unsleep = 2000;

void unsleep_interrupt() {
  if (digitalRead(button_pin) == LOW && prev_button_status == HIGH)  {
    millis_from_button_pressed = millis();
    prev_button_status=LOW;
  }
  delay(millis_to_unsleep);
  if (digitalRead(button_pin) == LOW &&   millis() - millis_from_button_pressed >= millis_to_unsleep) {
    sleeping = false;
    prev_button_status=HIGH;
  }
  if (digitalRead(button_pin) == HIGH && prev_button_status == HIGH && sleeping){
    prev_button_status=HIGH;
  }
}
void power_down_while_button_pressed_2s() {
  byte adcsra_save = ADCSRA;
  for (int i = 0; i < 20; i++ ) {
    pinMode(i, OUTPUT);
  }
  pinMode(button_pin, INPUT);
  while (sleeping) {
    noInterrupts ();
    MCUSR = 0;
    wdt_disable();
    ADCSRA = 0;  // запрещаем работу АЦП
    power_all_disable ();   // выключаем все модули
    set_sleep_mode (SLEEP_MODE_PWR_DOWN);   // устанавливаем режим сна
    sleep_enable();
    attachInterrupt (digitalPinToInterrupt(button_pin), unsleep_interrupt, LOW);
    interrupts ();
    sleep_cpu ();
  }
  detachInterrupt (digitalPinToInterrupt(button_pin));     // останавливаем прерывание LOW
  ADCSRA = adcsra_save;  // останавливаем понижение питания
  power_all_enable ();   // включаем все модули
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


