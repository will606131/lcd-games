#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);

const int buzzer_pin = 2;

const int a_button_pin = 3;
const int b_button_pin = 4;
const int c_button_pin = 5;

int tick = 0;

int jump = 0;
int player_row = 1;
int score = 0;

int obstacle_row = 1;
int obstacle_column = 15;
int obstacle_type = 1;

bool alive = true;

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
  pinMode(a_button_pin, INPUT);
  pinMode(b_button_pin, INPUT);
  pinMode(c_button_pin, INPUT);

  lcd.begin(16,2);
  lcd.clear();
  lcd.createChar(0, ground);
  lcd.createChar(1, grass);
  lcd.createChar(2, arrow);
  lcd.createChar(3, spike);
  
  lcd.createChar(6, player_states[0]);
  lcd.createChar(7, player_states[1]);
}

void loop() {
  tick += 1;
  if (!alive){
    lcd.clear();

    lcd.home();
    lcd.print("Game Over!");
    lcd.setCursor(0, 1);
    delay(500);
    lcd.print("Score: ");
    lcd.print(score);
    while (true){
      if (digitalRead(a_button_pin) == HIGH){
        alive = true;
        score = 0;
        break;
      }
    }
  }

  //player logic
  if (!jump){
    if (digitalRead(a_button_pin) == HIGH){
      jump = 10;
    }
  } 
  if (jump){
    jump--;
  }

  player_row = int((jump == 0) || (jump == 1) || (jump == 9));

  //obstacle logic
  if (obstacle_column <= 0){
    obstacle_row = analogRead(A0) % 2;
    obstacle_column = 15;
    obstacle_type = obstacle_row;
  }

  obstacle_column--;

  //game logic
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
  lcd.write(byte(6 + int((jump==1) || (jump>=4 && jump<=7) || (jump==9))));

  //drawing obstacle
  lcd.setCursor(obstacle_column, obstacle_row);
  lcd.write(byte(2 + obstacle_type));

  //sfx
  if (jump >= 8){
    tone(buzzer_pin, 1000 + 200 * (9 - jump), 200);
  }  
  if (!alive){
    tone(buzzer_pin, 1000, 80);
    delay(80);
    tone(buzzer_pin, 800, 80);
    delay(80);
    tone(buzzer_pin, 600, 80);
    delay(80);
    tone(buzzer_pin, 400, 120);
    delay(120);
    tone(buzzer_pin, 300, 150);
    delay(150);
  }

  delay(200 - score);
}
