#define pir_pin                         2

#define alarm_pin                       5
#define power_plugged_pin               A0
#define red_led_pin                     A4
#define button_pin                      3

#define hm10_rx_pin                     8
#define hm10_tx_pin                     9

#define hm10_key_pin                    6
#define hm10_conn_pin                   4

#define hc12_set_pin                    A1
#define hc12_enable_pin                 A3

#define DC5_enable_pin                  A2

#define power_bank_activation_pin       10


#define bat_voltage_measure_enable_pin  7
#define bat_voltage_measure_pin         A6

const byte s8 = 0b100001;
const byte s4 = 0b100000;


float battery_voltage() {
  pinMode(bat_voltage_measure_enable_pin,OUTPUT);
  digitalWrite(bat_voltage_measure_enable_pin, HIGH);
  float voltage = analogRead(bat_voltage_measure_pin) / 1024.0 * 5 * 1.014;
  digitalWrite(bat_voltage_measure_enable_pin, LOW);
  return voltage*2;
}
void turn_5v_on() {
  pinMode(DC5_enable_pin,OUTPUT);
  digitalWrite(DC5_enable_pin,HIGH);
}

void turn_5v_off() {
  pinMode(DC5_enable_pin,OUTPUT);
  digitalWrite(DC5_enable_pin,LOW);
}
void prepare_led_pin () {
  pinMode(red_led_pin, OUTPUT);
}
void release_led_pin() {
  pinMode(red_led_pin, INPUT);

}
void blink_red(int duration) {
  prepare_led_pin();
  delay(duration);
  release_led_pin();
}


void led_on(int pin) {
  prepare_led_pin();
  digitalWrite(pin, HIGH);
}

void led_off(int pin) {
  digitalWrite(pin, LOW);
  release_led_pin();
}


