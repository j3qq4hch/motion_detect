
void hc12_init() {
#ifdef reflash
  byte hc12_setted = EEPROM.read(hc12_setted_eeprom_address);
  if (hc12_setted != 1) {
#endif
    pinMode(hc12_enable_pin, OUTPUT);
    digitalWrite(hc12_enable_pin,HIGH);
    Serial.begin(9600);
    sleep_delay(hc12_SEND_DELAY);
    digitalWrite(hc12_set_pin, LOW);
    sleep_delay(AT_EXIT_DELAY);
    Serial.print(F(hc12_SET_DEFAULT));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_CH));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_FU));
    sleep_delay(AT_RESPONSE_DELAY);
    Serial.print(F(hc12_SET_PDB));
    sleep_delay(AT_RESPONSE_DELAY);
    digitalWrite(hc12_set_pin, HIGH);
    sleep_delay(AT_EXIT_DELAY);
    EEPROM.write(hc12_setted_eeprom_address, 1);
#ifdef reflash
  }
#endif
  digitalWrite(hc12_set_pin, HIGH);
  Serial.begin(1200);
}
void hc12_sleep() {
  pinMode(hc12_set_pin, OUTPUT);
  digitalWrite(hc12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  Serial.print(F(hc12_SLEEP));
  delay(AT_RESPONSE_DELAY);
  digitalWrite(hc12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}
void hc12_wakeup() {
  pinMode(hc12_set_pin, OUTPUT);
  digitalWrite(hc12_set_pin, LOW);
  delay(AT_EXIT_DELAY);
  digitalWrite(hc12_set_pin, HIGH);
  delay(AT_EXIT_DELAY);
}
