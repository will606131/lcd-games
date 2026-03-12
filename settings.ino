void settings_menu() {
  lcd.clear();
  while (true){
    lcd.setCursor(0, 0);
    if (settings_index == 0) {
      lcd.print("> Sound: ");
      if (sound_setting == 1) {
        lcd.print("On ");
      } else {
        lcd.print("Off");
      }
    } else {
      lcd.print("  Sound: ");
      if (sound_setting == 1) {
        lcd.print("On ");
      } else {
        lcd.print("Off");
      }
    }

    lcd.setCursor(0, 1);
    if (settings_index == 1) {
      lcd.print("> Return");
    } else {
      lcd.print("  Return");
    }

    if (digitalRead(a_button_pin) == HIGH) {
      button_destick(a_button_pin);
      settings_index = (settings_index + 1) % 2;
    }

    if (digitalRead(b_button_pin) == HIGH) {
      button_destick(b_button_pin);
      if (settings_index == 0) {
        sound_setting = !sound_setting;
        if (sound_setting) {
          lcd.setCursor(8, 0);
          lcd.print(" On ");
        } else {
          lcd.setCursor(8, 0);
          lcd.print(" Off");
        }
      } else if (settings_index == 1) {
        interface = 0;
        return;
      }
    }

    if (digitalRead(c_button_pin) == HIGH) {
      button_destick(c_button_pin);
      settings_index = (settings_index == 1) ? 0 : settings_index + 1;
    }
  }
}