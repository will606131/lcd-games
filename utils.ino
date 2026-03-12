void draw_custom_at(int column, int row, int byte_num){
  lcd.setCursor(column, row);
  lcd.write(byte(byte_num));
}

void play_tone(int pin, int frequency, int duration){
  if (sound_setting){
    tone(pin, frequency, duration);
  }
}

void button_destick(int pin) {
  play_tone(buzzer_pin, 2000, 20);
  delay(20);
  play_tone(buzzer_pin, 1200, 30);
  while(digitalRead(pin) == HIGH);
}

void menu_option_display(String message, int byte_num, void (*function)()) {
  int total_spaces = 16 - message.length() - 1;
  if (total_spaces < 0) total_spaces = 0;

  lcd.home();
  lcd.print(message);
  lcd.write(byte(byte_num));
  for (int i = 0; i < total_spaces; i++) {
    lcd.print(" ");
  }
  
  if (digitalRead(b_button_pin) == HIGH) {
    button_destick(b_button_pin);
    function();
    lcd.clear();
  }
}