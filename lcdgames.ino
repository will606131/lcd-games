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

byte joystick_icon[8] = {B00000, B01110, B01110, B01110, B00100, B00100, B11111, B11111};
byte settings_icon[8] = {B00000, B10101, B01110, B11011, B01110, B10101, B00000, B00000};
byte select_icon[8] = {B00000, B01110, B11011, B10001, B11011, B01110, B00000, B00000};
byte left_icon[8] = {B00000, B01110, B11011, B10011, B11011, B01110, B00000, B00000};
byte right_icon[8] = {B00000, B01110, B11011, B11001, B11011, B01110, B00000, B00000};

void jump_game_1();
void settings_menu();
void draw_custom_at(int column, int row, int byte_num);
void play_tone(int pin, int frequency, int duration);
void button_destick(int button_pin);
void menu_option_display(String message, int byte_num, void (*function)());

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(a_button_pin, INPUT);
  pinMode(b_button_pin, INPUT);
  pinMode(c_button_pin, INPUT);

  lcd.begin(16,2);
  lcd.clear();

  randomSeed(analogRead(A0));
}

void loop() {
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
      menu_option_display("    JUMP! ", 0, jump_game_1);
    }
    else if(menu_index == 1){
      menu_option_display("   Settings ", 1, settings_menu);

    }
  }

  draw_custom_at(0, 1, 3);
  draw_custom_at(7, 1, 2);
  draw_custom_at(15, 1, 4);
  
  delay(100);
}