#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <time.h>
#include <stdlib.h>
#include <EEPROM.h>

Adafruit_PCD8544 screen = Adafruit_PCD8544(7,6,5,3);

const char screen_height = screen.height();
const char screen_width = screen.width();

#define MAX_SNAKE_LENGTH 300

#define UP 1
#define DOWN 3
#define RIGHT 2
#define LEFT 4

#define FOOD_RADIUS 1

#define BOARD_TOP 7

char buttonDown = 8;
char buttonRight = 9;
char buttonUp = 10;
char buttonLeft = 11;

char snake[MAX_SNAKE_LENGTH][2];
char CURRENT_SNAKE_LENGTH;

//snake direction
//       N 1
//  E4         V 2
//       S 3
//
char snakeDirection = 2;

char foodIsOnScreen = 0;

char current_food_x;
char current_food_y;

boolean snake_is_dead = 0;

int score = 0;

void setup()
{ 
  //initialize buttons
  pinMode(buttonDown,INPUT);
  pinMode(buttonRight,INPUT);
  pinMode(buttonLeft,INPUT);
  pinMode(buttonRight,INPUT);
  
  Serial.begin(9600);
  screen.display();
  screen.command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);
  screen.clearDisplay();
  screen.begin();
  screen.setContrast(50);
  

  show_menu();
}

void draw_board()
{
   screen.drawRect(0, BOARD_TOP, screen.width(), screen.height() - BOARD_TOP, BLACK);
   screen.display();
}

void refresh_high_score()
{
  for(int i=0;i<84;i++)
    for(int j=0;j<BOARD_TOP;j++) screen.drawPixel(i,j,WHITE);
  
  screen.setTextSize(1);
  screen.setTextColor(BLACK);
  screen.setCursor(0,0);
  screen.print(score);
  screen.setCursor(screen_width/2,0);
  screen.print("HS:");
  screen.print(EEPROM.read(0));
  screen.display();
  
}

void init_snake()
{
  for(int i=0;i<MAX_SNAKE_LENGTH;i++)
  {
    snake[i][0] = -1;
    snake[i][1] = -1;
  }
  
  for(int i=0;i<10;i++)   
  { 
      snake[i][0] = i + 10;
      snake[i][1] = screen_height / 2;
  }
  
  for(int i=0;i<MAX_SNAKE_LENGTH;i++)
  {
    if( snake[i][0] != -1 && snake[i][1] != -1 )
        screen.drawPixel( snake[i][0], snake[i][1], BLACK);
  }
  
  screen.display();
}

void loop()
{ 
  
}

void start_game()
{
  while(!snake_is_dead)
  {
    draw_board();
  
    int down = digitalRead(buttonDown);
    int right = digitalRead(buttonRight);
    int left = digitalRead(buttonLeft);
    int up = digitalRead(buttonUp);
  
    if( down == 0 ) if( snakeDirection != UP ) snakeDirection = DOWN;
    if( right == 0 ) if( snakeDirection != LEFT ) snakeDirection = RIGHT;
    if( left == 0 ) if( snakeDirection != RIGHT ) snakeDirection = LEFT;
    if( up == 0 ) if( snakeDirection != DOWN ) snakeDirection = UP;
    Serial.println("MOVE");
  
    move_snake();
    check_snake_food();
    show_food();
  
    if( check_if_snake_is_dead() )
    {
       screen.clearDisplay();
       screen.display();
       
       screen.println("Game over!");
       screen.display();
       
       snake_is_dead = 1;
    }
  }
}

void move_snake()
{
  char tail_x = snake[0][0];
  char tail_y = snake[0][1];
  
  screen.drawPixel(tail_x, tail_y, WHITE );
  
  char contor = -1;
  
  for(int i=0;i<MAX_SNAKE_LENGTH;i++)
  {   

    if( snake[i][0] != -1 && snake[i][1] != -1)
    {
      contor++;
      snake[i][0] = snake[i+1][0];
      snake[i][1] = snake[i+1][1];
    }
    else break;
    
  }
  
  CURRENT_SNAKE_LENGTH = contor;
  
  switch(snakeDirection)
  {
    case UP:
      snake[contor][0] = snake[contor - 1][0];    
      if( (snake[contor - 1][1] - 1) <= BOARD_TOP ) snake[contor][1] = screen_height - 2;
        else snake[contor][1] = snake[contor - 1 ][1] - 1;
      break;

    case RIGHT:
      if( (snake[contor - 1][0] + 1) >= screen_width - 1 ) snake[contor][0] = 2;
      else snake[contor][0] = snake[contor - 1][0] + 1;
      snake[contor][1] = snake[contor - 1][1];
      break;
      
    case DOWN:
      snake[contor][0] = snake[contor - 1][0];
      if( (snake[contor - 1][1] + 1) >= screen_height - 1 ) snake[contor][1] = BOARD_TOP + 1;
        else snake[contor][1] = snake[contor - 1][1] + 1;
      break;

    case LEFT:
      if( (snake[contor - 1][0] - 1) <= 1 ) snake[contor][0] = screen_width - 2;
         else snake[contor][0] = snake[contor - 1][0] - 1;
      snake[contor][1] = snake[contor - 1][1];
      break;
  }
  
  for(int i = 0;i<MAX_SNAKE_LENGTH;i++)
  {
    if( snake[i][0] != -1 && snake[i][1] != -1 ) 
    screen.drawPixel( snake[i][0], snake[i][1], BLACK);
  }
  
  screen.display();
  
  delay(20);
}

void show_food()
{
  char food_x = 0;
  char food_y = 0;
  
  if( foodIsOnScreen == 0 )
  {
    char ok = 0;
    
    while(!ok)
    {
      ok = 1;
      
      food_x = rand() % 38 + BOARD_TOP + 2;
      food_y = rand() % 80 + 2;
  
      Serial.print("Food x = ");
      Serial.println(food_x, DEC);
      Serial.print("Food y = ");
      Serial.println(food_y, DEC);
  
      screen.fillCircle(food_y, food_x, FOOD_RADIUS, BLACK);
      screen.display();
    
      for(int i=0;i<CURRENT_SNAKE_LENGTH;i++)
      {
        if( snake[i][0] == food_x && snake[i][1] == food_y ) ok = 0; 
      }
      
      if( ok == 1 ) break;
    }
    
    foodIsOnScreen = 1;
    current_food_x = food_y;
    current_food_y = food_x;
  }
}

void check_snake_food()
{
  int snake_head_x = snake[CURRENT_SNAKE_LENGTH][0];
  int snake_head_y = snake[CURRENT_SNAKE_LENGTH][1];
  
  if( ( snake_head_x == current_food_x  && snake_head_y == current_food_y ) || 
      ( snake_head_x - 1 == current_food_x  && snake_head_y == current_food_y ) ||
      ( snake_head_x == current_food_x && snake_head_y - 1 == current_food_y ) ||
      ( snake_head_x - 1 == current_food_x && snake_head_y - 1 == current_food_y ) ||
      ( snake_head_x + 1 == current_food_x && snake_head_y == current_food_y ) ||
      ( snake_head_x == current_food_x && snake_head_y + 1 == current_food_y ) ||
      ( snake_head_x + 1 == current_food_x && snake_head_y + 1 == current_food_y ) )
   {
      screen.fillCircle(current_food_x, current_food_y, FOOD_RADIUS, WHITE);
      screen.display();
     
      for(int i=0;i<2;i++) 
      {
        switch(snakeDirection)
        {
        case UP:
          snake[CURRENT_SNAKE_LENGTH + 1][0] = snake[CURRENT_SNAKE_LENGTH][0];    
          snake[CURRENT_SNAKE_LENGTH + 1][1] = snake[CURRENT_SNAKE_LENGTH][1] - 1;
          break;

        case RIGHT:
          snake[CURRENT_SNAKE_LENGTH + 1][0] = snake[CURRENT_SNAKE_LENGTH][0] + 1;
          snake[CURRENT_SNAKE_LENGTH + 1][1] = snake[CURRENT_SNAKE_LENGTH][1];
          break;
      
        case DOWN:
          snake[CURRENT_SNAKE_LENGTH + 1][0] = snake[CURRENT_SNAKE_LENGTH][0];
          snake[CURRENT_SNAKE_LENGTH + 1][1] = snake[CURRENT_SNAKE_LENGTH][1] + 1;
          break;

        case LEFT:
          snake[CURRENT_SNAKE_LENGTH + 1][0] = snake[CURRENT_SNAKE_LENGTH][0] - 1;
          snake[CURRENT_SNAKE_LENGTH + 1][1] = snake[CURRENT_SNAKE_LENGTH][1];
          break;
        }
        CURRENT_SNAKE_LENGTH++;   
        foodIsOnScreen = 0;
     }
     
     score += 5;
     refresh_high_score();
     Serial.println("IA MANCAREA!");  
 }
   
   
}

boolean check_if_snake_is_dead()
{
  int snake_head_x = snake[CURRENT_SNAKE_LENGTH][0];
  int snake_head_y = snake[CURRENT_SNAKE_LENGTH][1];
  
  for(int i=0;i<CURRENT_SNAKE_LENGTH;i++)
  {
    if( snake[i][0] == snake_head_x && snake[i][1] == snake_head_y ) 
    {    
      int last_score = EEPROM.read(0);
      if( score > last_score ) EEPROM.write(0,score);
      return true; 
    }
  }
  
  return false;
}
