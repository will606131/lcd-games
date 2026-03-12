#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);
const int buzzer_pin = 2;
const int a_button_pin = 3;
const int b_button_pin = 4;
const int c_button_pin = 5;

int menu_index = 0;
int interface = 0;
int sound_setting = 1;
int settings_index = 0;

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(a_button_pin, INPUT);
  pinMode(b_button_pin, INPUT);
  pinMode(c_button_pin, INPUT);

  lcd.begin(16,2);
  lcd.clear();

  randomSeed(analogRead(A0));
}

void play_tone(int pin, int frequency, int duration){
  if (sound_setting){
    tone(pin, frequency, duration);
  }
  return;
}

void button_destick(int pin) {
  play_tone(buzzer_pin, 2000, 20);
  delay(20);
  play_tone(buzzer_pin, 1200, 30);
  while(digitalRead(pin) == HIGH);
}

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

void game1(){
  int tick = 0;

  int jump = 0;
  int player_row = 1;
  int score = 0;

  int obstacle_row = 1;
  int obstacle_column = 16;
  int obstacle_type = 1;

  bool alive = true;

  byte grass[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00100, B11011};
  byte ground[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};

  byte tree[8] = {B10010, B10100, B01100, B00101, B10110, B01100, B00100, B00100};
  byte dart[8] = {B00000, B00000, B00011, B11100, B00011, B00000, B00000, B00000};

  byte player_states[2][8] = {
    {B00000,B00000,B00000,B00000,B00000,B01110,B01110,B01110},
    {B00000,B01110,B01110,B01110,B00000,B00000,B00000,B00000}
  };
  byte dead_player_states[2][8] = {
    {B00000, B00100, B01001, B10110, B01111, B10110, B01001, B00000},
    {B00000, B00000, B10001, B00010, B01001, B10000, B01001, B00000}
  };

  lcd.createChar(0, ground);
  lcd.createChar(1, grass);
  lcd.createChar(2, dart);
  lcd.createChar(3, tree);
  lcd.createChar(4, dead_player_states[0]);
  lcd.createChar(5, dead_player_states[1]);
  lcd.createChar(6, player_states[0]);
  lcd.createChar(7, player_states[1]);
  lcd.clear();

  while (alive){
    //player logic
    if (!jump){
      if (digitalRead(a_button_pin) == HIGH){
        jump = 9;
      }
    } 
    if (jump){
      jump--;
    }

    player_row = int((jump == 0) || (jump == 1) || (jump == 8));

    //obstacle logic
    if (obstacle_column <= 0){
      obstacle_row = random(0,100) < 75;
      obstacle_column = 16;
      obstacle_type = obstacle_row;
    }

    obstacle_column--;

    //score/death logic
    if (obstacle_column == 1){
      if (player_row == obstacle_row){
        alive = false;
      }else{
        score++;
      }
    }

    //clearing sky
    lcd.setCursor(0, 0);
    lcd.print("                ");

    //drawing background
    for (int i = 0; i < 16; i++){
      lcd.setCursor(i, 1);
      if ((i + tick) % 4 == 0){lcd.write(byte(1));}
      else {lcd.write(byte(0));}
    }

    //drawing player
    lcd.setCursor(1, player_row);
    lcd.write(byte(6 + int((jump==1) || (jump>=4 && jump<=6) || (jump==8))));

    //drawing obstacle
    lcd.setCursor(obstacle_column, obstacle_row);
    lcd.write(byte(2 + obstacle_type));

    //sfx
    if (jump >= 7){
      play_tone(buzzer_pin, 1000 + 200 * (8 - jump), 200);
    }  

    tick += 1;
    delay(max(60, 200 - score * 2));
  }

  //game over screen
  lcd.setCursor(1, player_row);
  lcd.write(byte(4));
  play_tone(buzzer_pin, 1000, 80);
  delay(80);
  play_tone(buzzer_pin, 800, 80);
  delay(80);
  play_tone(buzzer_pin, 600, 80);
  delay(80);
  play_tone(buzzer_pin, 400, 120);
  delay(120);
  play_tone(buzzer_pin, 300, 150);
  delay(150);
  lcd.setCursor(1, player_row);
  lcd.write(byte(5));
  delay(400);

  lcd.clear();
  lcd.home();
  lcd.print("Game Over!");
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);

  while (true){
    if (digitalRead(a_button_pin) == HIGH){ 
      button_destick(a_button_pin);
      lcd.clear();
      break;
    }
  }
}

void loop() {
  byte joystick_icon[8] = {B00000, B01110, B01110, B01110, B00100, B00100, B11111, B11111};
  byte settings_icon[8] = {B00000, B10101, B01110, B11011, B01110, B10101, B00000, B00000};
  byte select_icon[8] = {B00000, B01110, B11011, B10001, B11011, B01110, B00000, B00000};
  byte left_icon[8] = {B00000, B01110, B11011, B10011, B11011, B01110, B00000, B00000};
  byte right_icon[8] = {B00000, B01110, B11011, B11001, B11011, B01110, B00000, B00000};

  lcd.createChar(0, joystick_icon);
  lcd.createChar(1, settings_icon);
  lcd.createChar(2, select_icon);
  lcd.createChar(3, left_icon);
  lcd.createChar(4, right_icon);

  if (digitalRead(a_button_pin) == HIGH){
    button_destick(a_button_pin);
    menu_index--;
  }
  if (digitalRead(c_button_pin) == HIGH){
    button_destick(c_button_pin);
    menu_index++;
  }
  menu_index = constrain(menu_index, 0, 1);

  if (interface == 0){
    if (menu_index == 0){
      lcd.home();
      lcd.print("    JUMP! ");
      lcd.write(byte(0));
      lcd.print("     ");
      if (digitalRead(b_button_pin) == HIGH){
        button_destick(b_button_pin);
        game1();
        lcd.clear();
      }

    }else if(menu_index == 1){
      lcd.home();
      lcd.print("   Settings ");
      lcd.write(byte(1));
      lcd.print("   ");
      if (digitalRead(b_button_pin) == HIGH){
        button_destick(b_button_pin);
        settings_menu();
        lcd.clear();
      }
    }

  lcd.setCursor(0, 1);
  lcd.write(byte(3));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(15, 1);
  lcd.write(byte(4));

  delay(100);
  }
}
