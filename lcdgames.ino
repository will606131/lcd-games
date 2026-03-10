#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);
const int buzzer_pin = 2;
const int a_button_pin = 3;
const int b_button_pin = 4;
const int c_button_pin = 5;

int menu_index = 0;
int interface = 0; //0 -> menu; 1 -> game selection; 2 -> settings; 

void setup() {
  pinMode(buzzer_pin, OUTPUT);
  pinMode(a_button_pin, INPUT);
  pinMode(b_button_pin, INPUT);
  pinMode(c_button_pin, INPUT);

  lcd.begin(16,2);
  lcd.clear();
}


void scroller(){
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

  byte spike[8] = {B00000, B00000, B00100, B00100, B01110, B01110, B11111, B11111};
  byte arrow[8] = {B00000, B00111, B01100, B11000, B11000, B01100, B00111, B00000};

  byte player_states[2][8] = {
    {B00000,B00000,B00000,B00000,B00000,B01110,B01110,B01110},
    {B00000,B01110,B01110,B01110,B00000,B00000,B00000,B00000}
  };

  lcd.createChar(0, ground);
  lcd.createChar(1, grass);
  lcd.createChar(2, arrow);
  lcd.createChar(3, spike);
  
  lcd.createChar(6, player_states[0]);
  lcd.createChar(7, player_states[1]);
  lcd.clear();

  while (alive){
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
    lcd.write(byte(6 + int((jump==1) || (jump>=4 && jump<=7) || (jump==9))));

    //drawing obstacle
    lcd.setCursor(obstacle_column, obstacle_row);
    lcd.write(byte(2 + obstacle_type));

    //sfx
    if (jump >= 8){
      tone(buzzer_pin, 1000 + 200 * (9 - jump), 200);
    }  

    tick += 1;
    delay(200 - score * 2);
  }

  //game over screen
  lcd.clear();
  lcd.home();
  lcd.print("Game Over!");
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
  lcd.setCursor(0, 1);
  lcd.print("Score: ");
  lcd.print(score);

  //wait for buttons to return to menu or restart
  while (true){
    if (digitalRead(a_button_pin) == HIGH){
      return scroller(); // restart game
    }
    if (digitalRead(b_button_pin) == HIGH){
      lcd.clear();
      return; // go back to menu
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
    menu_index--;
    while(digitalRead(a_button_pin) == HIGH);
  }
  if (digitalRead(c_button_pin) == HIGH){
    menu_index++;
     while(digitalRead(c_button_pin) == HIGH);
  }

  if (interface == 0){
    if (menu_index == 0){
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("Games ");
      lcd.write(byte(0));
      if (digitalRead(b_button_pin) == HIGH){
        interface = 1;
        while(digitalRead(b_button_pin) == HIGH){
          continue;
        };
      }

    }else if(menu_index == 1){
      lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Settings ");
      lcd.write(byte(1));
      if (digitalRead(b_button_pin) == HIGH){
        interface = 2;
        while(digitalRead(b_button_pin) == HIGH){
          continue;
        };
      }

    }

  }else if (interface == 1){
    scroller();
  }
  else if (interface == 2){
  }

  lcd.setCursor(0, 1);
  lcd.write(byte(3));
  lcd.setCursor(7, 1);
  lcd.write(byte(2));
  lcd.setCursor(15, 1);
  lcd.write(byte(4));

  delay(100);
}
