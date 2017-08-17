
#define pir_pin                         2
#define button_pin                      3
#define hm10_conn_pin                   4
#define alarm_pin                       5
#define hm10_key_pin                    6
#define bat_voltage_measure_enable_pin  7

#define hm10_rx_pin                     8
#define hm10_tx_pin                     9
#define power_bank_activation_pin       10


#define power_plugged_pin               A0

#define hc12_set_pin                    A1
#define DC5_enable_pin                  A2
#define hc12_enable_pin                 A3

#define red_led_pin                     A4
#define green_led_pin                   13
#define bat_voltage_measure_pin         A6


const byte s8 = 0b100001;
const byte s1 = 0b000110;
const byte s4 = 0b100000;

// Показываем статус аккумуляторы: если ОК - длинное включение светодиода, если средне - среднее, малый заряд - короткое моргание
float battery_voltage() {
  pinMode(bat_voltage_measure_enable_pin, OUTPUT);
  digitalWrite(bat_voltage_measure_enable_pin, HIGH);
  float voltage = analogRead(bat_voltage_measure_pin) / 1024.0 * 5 * 1.014;
  digitalWrite(bat_voltage_measure_enable_pin, LOW);
  return voltage * 2;
}
void turn_5v_on() {
  pinMode(DC5_enable_pin, OUTPUT);
  digitalWrite(DC5_enable_pin, HIGH);
}

void turn_5v_off() {
  pinMode(DC5_enable_pin, OUTPUT);
  digitalWrite(DC5_enable_pin, LOW);
}
void prepare_led_pins () {
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
}
void release_led_pin() {
  pinMode(red_led_pin, HIGH);
  pinMode(green_led_pin, HIGH);
}

void led_on(int pin) {
  prepare_led_pins();
  digitalWrite(pin, LOW);
}

void led_off(int pin) {
  digitalWrite(pin, HIGH);
  release_led_pin();
}

void blink_red(int duration) {
  prepare_led_pins();
  led_on(red_led_pin);
  delay(duration);
  release_led_pin();
}

void blink_green(int duration) {
  prepare_led_pins();
  led_on(green_led_pin);
  delay(duration);
  release_led_pin();
}

void blink_yellow(int duration) {
  prepare_led_pins();
  led_on(red_led_pin);
  led_on(green_led_pin);
  delay(duration);
  release_led_pin();
}



void show_battery_status() {
  float b = battery_voltage();
  if (b > battery_high_voltage) {
    blink_green(blink_long_duration);
  } else if (b > battery_medium_voltage) {
    blink_yellow(blink_duration);
  }
  else {
    blink_red(blink_short_duration);
  }
}


void sensor_started() {
  pinMode(alarm_pin, OUTPUT);
  tone(alarm_pin, Do);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Mi);
  sleep_delay(mSLEEP_120MS);
  tone(alarm_pin, Sol);
  sleep_delay(mSLEEP_120MS);
  noTone(alarm_pin);
  pinMode(alarm_pin, INPUT);

}

void activate_power_bank(){
  pinMode(power_bank_activation_pin,OUTPUT);
  digitalWrite(power_bank_activation_pin,HIGH);
  delay(1);
  digitalWrite(power_bank_activation_pin,LOW);
  pinMode(power_bank_activation_pin,INPUT);
  digitalWrite(power_bank_activation_pin,LOW);
  
  }


