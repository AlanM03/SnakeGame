#include <NonBlockingGame.h>
#include <iostream>
#include <time.h>
#include <vector>

using namespace bridges::game;
using namespace std;


class Block {
public:
    Block* next = nullptr;
    int x;
    int y;
    Block() {
        x = 0;
        y = 0;
    }
    Block(int x, int y) {
        this->x = x;
        this->y = y;
    }
};
class SnakeGame: public NonBlockingGame {
public:
    long frameTime;
    long nextFrameTime;
    Block apple;
    Block* snake;
    Block* temp;
    Block prevBlock1;
    Block prevBlock2;
    int gold = 0;//will be used to determine if apple is gold
 
    enum direction {UP, DOWN, LEFT, RIGHT, NONE};
    direction movement = NONE;    
    int counter = 0;
    int tensPlace = 0;
    int scoreboardPlace = 14;
    int frame = 0;
    int bodyCount = 0;
    SnakeGame(int assID, std::string username, std::string apikey)
        : NonBlockingGame(assID, username, apikey, 30, 30) {
    }
    virtual void initialize() override {
        srand(time(NULL));
       
      
      snake = new Block(15, 15);
      temp = snake;
      for(int i = 1; i <= 3; i++){
        
        temp->next = new Block(15, 15 - i);
        temp = temp->next; 
        bodyCount++;
        }
      
        plantApple();
        paint();
      }

void handleInput() {//handles keystrokes and prevents inputs
       
      if(keyUp()){
        if(movement!=DOWN)
           movement = UP;
        }
      else if(keyDown()){
        if(movement != UP)
           movement = DOWN;
        }
      else if(keyRight()){
        if(movement != LEFT)
           movement = RIGHT;
        }
      else if(keyLeft()){
        if(movement != RIGHT)
           movement = LEFT; 
        }
    }
    
void updatePosition() { //incorporates proper movement
      temp = snake;
       switch(movement){
        case UP:
          prevBlock1.x = temp->x;
          prevBlock1.y = temp->y;
          temp->x= (temp->x-1);
          break;

        case DOWN:
         
          prevBlock1.x = temp->x;
          prevBlock1.y = temp->y;
          temp->x= (temp->x+1);            
          break;

        case LEFT:
          
          prevBlock1.x = temp->x;
          prevBlock1.y = temp->y;
          temp->y= (temp->y-1);            
          break;

        case RIGHT:
          
          prevBlock1.x = temp->x;
          prevBlock1.y = temp->y;
          temp->y= (temp->y+1);
          break;
      

      case NONE:
        break;
    }

          if(movement != NONE){
            for(int i = 1; i <= bodyCount; i++){
              temp = temp->next;
              prevBlock2.x = temp->x;
              prevBlock2.y = temp->y;
              temp->x= (prevBlock1.x);
              temp->y= (prevBlock1.y);
              prevBlock1.x = prevBlock2.x;
              prevBlock1.y = prevBlock2.y;
              }
            }
    }
   
void plantApple() {//creates logic to make both red and yellow apples
      srand(time(NULL));
      bool onBody = false;
      
        
      apple.x = 1 + (rand() % 27);
      apple.y = 1 + (rand() % 27);
      gold = 1 + (rand() % 5);//20% chance for apple to be yellow

      while(true){
        temp = snake;
        for(int i = 0; i < bodyCount;i++){
        while(apple.x == temp->x && apple.y == temp->y){
          apple.x = 1 + (rand() % 27);
          apple.y = 1 + (rand() % 27);
          gold = 1 + (rand() % 5);
              }
          temp = temp->next;
            }
        break;
        }
      }
        
    
    
void detectApple() {//checks to see if the snake head connects with a apple
      temp = snake;
      if(temp->x == apple.x && temp->y == apple.y && gold != 1){//adds one body to snake if apple is red
        for(int i = 0;i < bodyCount;i++){
          temp = temp->next;
        }
        temp->next = new Block(prevBlock1.x, prevBlock1.y);
        bodyCount++;
        counter++;
          plantApple();
        }

      else if(temp->x == apple.x && temp->y == apple.y && gold == 1){//adds two to length if the apple is golden
        for(int i = 0;i < bodyCount;i++){
          temp = temp->next;
        }      
        temp->next = new Block(prevBlock1.x, prevBlock1.y);
        temp = temp->next;
        temp->next = new Block(prevBlock2.x, prevBlock2.y);
        bodyCount+=2; 
        counter+=2;
        plantApple();
      }  
    }

void deleteSnake(){ //removes snake and temp from pointing to heap
      for(int i = 0; i < bodyCount;i++){
            temp = temp->next;
            bodyCount--;
            delete snake;
            snake = temp;
        }
      snake = nullptr;     
}

    
void detectDeath() {//detects if the snake eats itself or hits the grey wall
      temp = snake;

      if(temp->x > 28 || temp->x < 1 || temp->y > 28 || temp->y < 1){
        end();        
        quit();
        temp = snake;
        deleteSnake();
        return;
        
        }
      temp = temp->next;
      for(int i = 0;i < bodyCount;i++){
          if(snake->x == temp->x && snake->y == temp->y){
            end();                                     
            quit();
            temp = snake;
            deleteSnake();
            return;
        }
          temp = temp->next;
        }
      
    }

void end(){//creates end screen
  for(int i = 0; i < 30; i++) {
    for(int j = 0; j < 30; j++){
      drawSymbol(i, j,NamedSymbol::square , NamedColor::black);
          if(i == 0 || i == 29 || j == 0 || j == 29){
            setBGColor(i, j, NamedColor::grey);
            drawSymbol(i, j,NamedSymbol::square , NamedColor::grey);
          }
          }
      }
  for(int i = 0; i < 18;i++){
    setBGColor(23-i, 23-i, NamedColor::red);
    drawSymbol(23-i, 23-i,NamedSymbol::square , NamedColor::red);
  }
  for(int i = 0; i < 18;i++){
    setBGColor(23-i, 7+i, NamedColor::red);
    drawSymbol(23-i, 7+i,NamedSymbol::square , NamedColor::red);
  }
  
}

 
void paint() {//paints this on the screen every frame
 
//------------------sets boards black and edges grey-------------
      for(int i = 0; i < 30; i++) {
        for(int j = 0; j < 30; j++){
            drawSymbol(i, j,NamedSymbol::square , NamedColor::black);
          if(i == 0 || i == 29 || j == 0 || j == 29){
            setBGColor(i, j, NamedColor::grey);
            drawSymbol(i, j,NamedSymbol::square , NamedColor::grey);
          }
          }
      }

//-----------------Draws the snake every frame----------------
      temp = snake;
      for(int i = 0; i < bodyCount; i++) {
        drawSymbol(temp->x, temp->y, NamedSymbol::square ,NamedColor::green);
        drawSymbol(snake->x, snake->y, NamedSymbol::square ,NamedColor::lightgreen);      
        temp = temp->next;     
      }

//-----------------Draws the apples every frame-------------------
      if(gold != 1)
            drawSymbol(apple.x, apple.y,NamedSymbol::square , NamedColor::red);
        else if(gold == 1)
            drawSymbol(apple.x, apple.y,NamedSymbol::square , NamedColor::yellow);

//-----------------Draws the scoreboard---------------------

  drawSymbol(0, 13, NamedSymbol::apple, NamedColor::red);
  for(int i = 0; i < counter;i++){

    switch(counter){//handles the scoreboard
      case 0:
        drawSymbol(0, scoreboardPlace, NamedSymbol::zero, NamedColor::red);
        break;

      case 1:
        drawSymbol(0, scoreboardPlace, NamedSymbol::one, NamedColor::red);
        break;

      case 2:
        drawSymbol(0, scoreboardPlace, NamedSymbol::two, NamedColor::red);
        break;

      case 3:
        drawSymbol(0, scoreboardPlace, NamedSymbol::three, NamedColor::red);
        break;

      case 4:
        drawSymbol(0, scoreboardPlace, NamedSymbol::four, NamedColor::red);
        break;

      case 5:
        drawSymbol(0, scoreboardPlace, NamedSymbol::five, NamedColor::red);
        break;

      case 6:
        drawSymbol(0, scoreboardPlace, NamedSymbol::six, NamedColor::red);
        break;

      case 7:
        drawSymbol(0, scoreboardPlace, NamedSymbol::seven, NamedColor::red);
        break;

      case 8:
        drawSymbol(0, scoreboardPlace, NamedSymbol::eight, NamedColor::red);
        break;

      case 9:
        drawSymbol(0, scoreboardPlace, NamedSymbol::nine, NamedColor::red);
        break;
          }
        }

    if(counter >= 10){//incriments spacing and tensPlace 
      counter-=10;
      tensPlace++;
      if(tensPlace == 1 || tensPlace == 10)
        scoreboardPlace++;
    }

    if(counter == 0 && tensPlace == 0){
      drawSymbol(0, scoreboardPlace, NamedSymbol::zero, NamedColor::red);
    }
    if(counter == 0 && tensPlace >= 1){//these if statements take care of the 10's place
        drawSymbol(0, scoreboardPlace, NamedSymbol::zero, NamedColor::red);
    }
        if(tensPlace == 1){
              drawSymbol(0, 14, NamedSymbol::one, NamedColor::red);
        }
        if(tensPlace == 2){
              drawSymbol(0, 14, NamedSymbol::two, NamedColor::red);
        }
        if(tensPlace == 3){
              drawSymbol(0, 14, NamedSymbol::three, NamedColor::red);
        }
        if(tensPlace == 4){
              drawSymbol(0, 14, NamedSymbol::four, NamedColor::red);
        }
        if(tensPlace == 5){
              drawSymbol(0, 14, NamedSymbol::five, NamedColor::red);
        }
         if(tensPlace == 6){
              drawSymbol(0, 14, NamedSymbol::six, NamedColor::red);
        }
         if(tensPlace == 7){
              drawSymbol(0, 14, NamedSymbol::seven, NamedColor::red);
        }
         if(tensPlace == 8){
              drawSymbol(0, 14, NamedSymbol::eight, NamedColor::red);
        }
         if(tensPlace == 9){
              drawSymbol(0, 14, NamedSymbol::nine, NamedColor::red);
        }
         if(tensPlace == 10 && counter == 0){//recognizes if score is 100
            drawSymbol(0, 14, NamedSymbol::one, NamedColor::red);
            drawSymbol(0, 15, NamedSymbol::zero, NamedColor::red);
            drawSymbol(0, 16, NamedSymbol::zero, NamedColor::red);       
        }
        if(tensPlace == 10 && counter > 0){//if greater then 100 display an arrow next to the 100
            drawSymbol(0, 14, NamedSymbol::bow_and_arrow, NamedColor::red);
            drawSymbol(0, 15, NamedSymbol::one, NamedColor::red);
            drawSymbol(0, 16, NamedSymbol::zero, NamedColor::red);
            drawSymbol(0, 17, NamedSymbol::zero, NamedColor::red);           
        }     
    }
 
virtual void gameLoop() override {
        paint();
        
        if (frame % 5 == 0) {
            handleInput();
            updatePosition();
            detectApple();
            detectDeath();
        }
        frame++;
    }
};
