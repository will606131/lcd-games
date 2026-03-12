void jump_game_1(){
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
      draw_custom_at(i, 1, ((i + tick) & 3) == 0 ? byte(1) : byte(0));
    }

    //drawing player
    draw_custom_at(1, player_row, 6 + int((jump==1) || (jump>=4 && jump<=6) || (jump==8)));

    //drawing obstacle
    draw_custom_at(obstacle_column, obstacle_row, 2 + obstacle_type);

    //sfx
    if (jump >= 7){
      play_tone(buzzer_pin, 1000 + 200 * (8 - jump), 200);
    }  

    tick += 1;
    delay(max(60, 200 - score * 2));
  }

  //game over screen
  draw_custom_at(1, player_row, 4);
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
  draw_custom_at(1, player_row, 5);
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