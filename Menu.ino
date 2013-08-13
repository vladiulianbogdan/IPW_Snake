#define MENU_RIGHT_COORD 10
#define TEXT_HEIGHT 10
#define NEW_GAME 1
#define HIGHSCORE 2
#define TOP_MENU 15

char select_circle_x = MENU_RIGHT_COORD - 5;
char select_circle_y = 4;

char option_selected = NEW_GAME;

void show_menu()
{
  screen.clearDisplay();
  screen.setCursor(10,0);
  screen.setTextSize(2);
  screen.println("SNAKE");
  screen.setTextSize(1);
  screen.setCursor(MENU_RIGHT_COORD,TOP_MENU);
  screen.drawCircle( select_circle_x, select_circle_y + TOP_MENU ,1,BLACK);
  screen.println("New game");
  screen.setCursor(MENU_RIGHT_COORD,TEXT_HEIGHT + TOP_MENU);
  screen.println("Highscore");
  screen.display();
  
  while(1)
  {
    char bRight = digitalRead(buttonRight);
    char bDown = digitalRead(buttonDown);
    char bUp = digitalRead(buttonUp);
    
    if( bRight == 0 ) 
    {
      if( option_selected == NEW_GAME )
      {
        select_new_game();
        break;
      }
      else if( option_selected == HIGHSCORE )
      {
        select_highscore();
        break;
      }
    }
    else if( bDown == 0 )
    {
      option_menu_select_down();
      delay(500);
    }
    else if( bUp == 0 )
    {
      option_menu_select_up();
      delay(500);
    }
  }
}

void option_menu_select_up()
{
  if( select_circle_y >= 4 )
  {
    screen.drawCircle(select_circle_x, select_circle_y + TOP_MENU, 1, WHITE);
    screen.display();
    select_circle_y -= TEXT_HEIGHT;
    screen.drawCircle( select_circle_x, select_circle_y + TOP_MENU, 1,BLACK);
    screen.display();
    option_selected--;
  }
}

void option_menu_select_down()
{
    if( select_circle_y <= 14 )
    {
      screen.drawCircle(select_circle_x, select_circle_y + TOP_MENU, 1, WHITE);
      screen.display();
      select_circle_y += TEXT_HEIGHT;
      screen.drawCircle( select_circle_x, select_circle_y + TOP_MENU, 1,BLACK);
      screen.display();
      option_selected++;
    }
}

void select_new_game()
{
  screen.clearDisplay();
  init_snake();
  draw_board();
  refresh_high_score();
  start_game();
}

void select_highscore()
{
  screen.clearDisplay();
  screen.println("Highscore is:");
  screen.print(EEPROM.read(0));
  screen.display();
  
  while(1)
  {
    char bRight = digitalRead(buttonRight);
    char bDown = digitalRead(buttonDown);
    char bUp = digitalRead(buttonUp);
    char bLeft = digitalRead(buttonUp);
    
    if( bRight == 0 || bDown == 0 || bUp == 0 || bLeft == 0 )
    {
      show_menu();
      break;
    }
  }
}
