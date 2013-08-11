#define MENU_RIGHT_COORD 10
#define TEXT_HEIGHT 10

char select_circle_x = MENU_RIGHT_COORD - 5;
char select_circle_y = 4;

void show_menu()
{
  screen.clearDisplay();
  screen.setCursor(MENU_RIGHT_COORD,0);
  screen.drawCircle( select_circle_x, select_circle_y ,1,BLACK);
  screen.println("New game");
  screen.setCursor(MENU_RIGHT_COORD,TEXT_HEIGHT);
  screen.println("Highscore");
  screen.display();
  
  while(1)
  {
    int bRight = digitalRead(buttonRight);
    int bDown = digitalRead(buttonDown);
    int bUp = digitalRead(buttonUp);
    
    if( bRight == 0 ) 
    {
      screen.clearDisplay();
      select_new_game();
      break;
    }
    else if( bDown == 0 )
    {
      option_menu_select_down();
    }
    else if( bUp == 0 )
    {
      option_menu_select_up();
    }
  }
}

void option_menu_select_up()
{
    screen.drawCircle(select_circle_x, select_circle_y, 1, WHITE);
    screen.display();
    select_circle_y -= TEXT_HEIGHT;
    Serial.println(select_circle_y,DEC);
    screen.drawCircle( select_circle_x, select_circle_y, 1,BLACK);
    screen.display();
}

void option_menu_select_down()
{
    screen.drawCircle(select_circle_x, select_circle_y, 1, WHITE);
    screen.display();
    select_circle_y += TEXT_HEIGHT;
    Serial.println(select_circle_y,DEC);
    screen.drawCircle( select_circle_x, select_circle_y, 1,BLACK);
    screen.display();
}

void select_new_game()
{
  init_snake();
  draw_board();
  refresh_high_score();
  while(1)
  {
    start_game();
  }
}

void select_highscore()
{

}
