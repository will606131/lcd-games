#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);

const int buzzer_pin = 2;

const int action_button_pin = 3;
const int select_l_button_pin = 4;
const int select_r_button_pin = 5;

int tick = 0;
int jump = 0;
int player_row = 0;

byte grass[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00100, B11011};
byte ground[8] = {B00000, B00000, B00000, B00000, B00000, B00000, B00000, B11111};

byte spike[8] = {B00000, B00000, B00100, B00100, B01110, B01110, B11111, B11111};
byte arrow[8] = {B00000, B00111, B01100, B11000, B11000, B01100, B00111, B00000};

byte player_states[2][8] = {
  {B00000,B00000,B00000,B00000,B00000,B01110,B01110,B01110},
  {B00000,B01110,B01110,B01110,B00000,B00000,B00000,B00000}
};

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(action_button_pin, INPUT);
  pinMode(select_l_button_pin, INPUT);
  pinMode(select_r_button_pin, INPUT);

  lcd.begin(16,2);
  lcd.clear();
  lcd.createChar(0, ground);
  lcd.createChar(1, grass);
  lcd.createChar(2, spike);
  lcd.createChar(3, arrow);
  
  lcd.createChar(6, player_states[0]);
  lcd.createChar(7, player_states[1]);
}

void loop() {
  tick += 1;
  
  //jump logic
  if (!jump){
    if (digitalRead(action_button_pin) == HIGH){
      jump = 10;
    }
  } 
  if (jump){
    jump--;
  }

  //drawing background
  for (int i = 0; i < 16; i++){
    lcd.setCursor(i, 1);
    if ((i + tick) % 4 == 0){lcd.write(byte(1));}
    else {lcd.write(byte(0));}
  }

  //drawing player
  player_row = (jump == 0) || (jump == 1) || (jump == 9);

  if (player_row){
    lcd.setCursor(1, 0);
    lcd.print(" ");
  }
  lcd.setCursor(1, int(player_row));
  lcd.write(byte(6 + int((jump==1) || (jump>=4 && jump<=7) || (jump==9))));

  //sfx
  if (jump >= 8){
    tone(buzzer_pin, 1000 + 200 * (9 - jump), 200);
  }

  delay(200);
}
