/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

//#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
#include <MemoryUsage.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int cursorPos = 0;
int oldCursorPos = 0;
bool cursorOff = true;
unsigned int cursorTime = 0;
unsigned int moveTime = 0;
unsigned int drawTime = 0;
int showTime = 5;

int cursorWaitTime = 500;
int moveWaitTime = 200;
int drawWaitTime = 100;

int cursorPosMenu = 16;
bool inMenu = false;

int cursorPosSubMenu = 0;
bool inSubMenu = false;

unsigned char currMap[32]={0};

void setup() {
  Serial.begin(9600);
    Serial.println(F("Ending state of the memory:"));
    Serial.println();
    
    MEMORY_PRINT_START
    MEMORY_PRINT_HEAPSTART
    MEMORY_PRINT_HEAPEND
    MEMORY_PRINT_STACKSTART
    MEMORY_PRINT_END
    MEMORY_PRINT_HEAPSIZE
    
    Serial.println();
    Serial.println();
    
    FREERAM_PRINT;

  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.display();
}

void loop() {  
  moveCursor();
  inputDraw();
  alternateCursor();

  if (digitalRead(8) == LOW){
    enterMenu();
    
      //currMap[1] = 0b00000011;
      //currMap[5] = 0b11111111;
      //drawMap(currMap);
      //Serial.println(checkMap(24));
      Serial.println("\nbutton 7 pressed!");
      for (int i = 0; i < 32; i++){
        Serial.print(currMap[i]);
        if (i%2==1){
          Serial.println();
        }
        else{
          Serial.print(',');
        }
      }
      delay(200);
  }

  display.display();
}

void moveCursor(){
  //move cursor to new pixel if button pressed
  int oldCursorPos = cursorPos;
  if ((millis()% 30000) - moveTime > moveWaitTime){
    if (digitalRead(2) == LOW && (cursorPos % 16 != 15)){
        Serial.println("\nbutton 1 pressed!");
        cursorPos += 1;
        Serial.println(cursorPos);
        moveTime = millis() % 30000;
    }  
    if (digitalRead(3) == LOW && (cursorPos < 240)){
        Serial.println("\nbutton 2 pressed!");
        cursorPos += 16;
        Serial.println(cursorPos);
        moveTime = millis() % 30000;
  
    }
    if (digitalRead(4) == LOW && (cursorPos > 15)){
        Serial.println("\nbutton 3 pressed!");
        cursorPos -= 16;
        Serial.println(cursorPos);
        moveTime = millis() % 30000;
    }
    if (digitalRead(5) == LOW && (cursorPos % 16 != 0)){
        Serial.println("\nbutton 4 pressed!");
        cursorPos -= 1;
        Serial.println(cursorPos);
        moveTime = millis()% 30000;
    }
  }
  
  //make sure cursor state doesn't persist on previous pixel
  if (oldCursorPos != cursorPos){
      bool oldPosVal = checkMap(oldCursorPos);
      bool newPosVal = checkMap(cursorPos);
      
      Serial.print("old cursor pos: ");
      Serial.println(oldCursorPos);
      Serial.print("new cursor position: ");
      Serial.println(cursorPos);
      Serial.print("cursor is off: ");
      Serial.println(cursorOff);
      Serial.print("old position value: ");
      Serial.println(oldPosVal);
      Serial.println("-------------");
      
      if (oldPosVal && cursorOff){
        Serial.println("hit white");
        display.drawRect(oldCursorPos % 16 * 4 + 33, (oldCursorPos / 16) * 4 + 1, 2, 2, WHITE);
      }
      else if (!(oldPosVal || cursorOff)){
        Serial.println("hit black");
        display.drawRect(oldCursorPos % 16 * 4 + 33, (oldCursorPos / 16) * 4 + 1, 2, 2, BLACK);
      }
      //reset cursor to on position
      cursorOff = !newPosVal;
      cursorTime = (millis() % 30000) + cursorWaitTime;
  }
  
}

void inputDraw(){
  if ((millis()% 30000) - drawTime > drawWaitTime){
    if (digitalRead(6) == LOW){
        Serial.print("\nbutton 5 pressed!");
        display.drawRect(cursorPos % 16 * 4 + 32, (cursorPos / 16) * 4, 4, 4, WHITE);
        setMap(cursorPos, 1);
        drawTime = millis()% 30000;
    }
    if (digitalRead(7) == LOW){
        Serial.print("\nbutton 6 pressed!");
        display.drawRect(cursorPos % 16 * 4 + 32, (cursorPos / 16) * 4, 4, 4, BLACK);
        setMap(cursorPos, 0);
        drawTime = millis()% 30000;
    }
  }
}

void alternateCursor(){
  if ((millis()% 30000) - cursorTime > cursorWaitTime){
    cursorTime = millis()% 30000;
    if (cursorOff){
      display.drawRect(cursorPos % 16 * 4 + 33, (cursorPos / 16) * 4 + 1, 2, 2, WHITE);
    }
    if (!cursorOff){
      display.drawRect(cursorPos % 16 * 4 + 33, (cursorPos / 16) * 4 + 1, 2, 2, BLACK);
    }
    cursorOff = !cursorOff;
  }
}

bool checkMap(int pos){
  unsigned char currByte = currMap[pos / 8];
  int ind = (pos % 16) % 8;
  return (currByte >> ind) & 1U;
}

void setMap(int pos, int val){
  unsigned char currByte = currMap[pos / 8];
  int ind = (pos % 16) % 8;
  currMap[pos / 8] = (currByte & ~(1UL << ind)) | (val << ind);
}

void drawMap(unsigned char mapx[32]){
  display.clearDisplay();
  for (int i = 0; i < 32; i++){
    for (int j = 0; j < 8; j++){
      if ((mapx[i] >> j) & 1U){
        int posx = (i*8+j);
        display.drawRect(posx % 16 * 4 + 32, (posx / 16) * 4, 4, 4, WHITE);
        display.drawRect(posx % 16 * 4 + 33, (posx / 16) * 4 + 1, 2, 2, WHITE);
      }
    }
  }
}

void moveMenu(){
  if ((millis() % 30000)- moveTime > moveWaitTime){
      if (digitalRead(3) == LOW && (cursorPosMenu < 208)){
          Serial.println("\nbutton 2 pressed!");
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, BLACK);
          cursorPosMenu += 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);
          
          moveTime = millis()% 30000;
    
      }
      if (digitalRead(4) == LOW && (cursorPosMenu > 16)){
          Serial.println("\nbutton 3 pressed!");
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, BLACK);
          cursorPosMenu -= 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);
          moveTime = millis()% 30000;
      }
  }
}

int handleMenu(){
    if ((millis() % 30000)- drawTime > drawWaitTime){
      if (digitalRead(6) == LOW){
          Serial.print("\nbutton 5 pressed!");
          drawTime = millis()% 30000;

          inMenu = false;
          return (cursorPosMenu - 16) / 48;
      }
      else if (digitalRead(7) == LOW){
          inMenu = false;
          return -1;
      }
    }
}

void enterSaveMenu(){
  int numMaps = 0;
  int fileNum = -1;
  int mapNum = -1;
  
  inMenu = true;
  cursorPosMenu = 64;
  displaySubMenu();
  display.display();
  while(digitalRead(6) == LOW){};

  while(inMenu){
    display.display();
    moveSubMenu();
    fileNum = handleMenu();
  }
  switch(fileNum) {
   case 1:
      //numMaps = EEPROM.read(0);
      //displayMapMenu(numMaps);
      //moveMapMenu(numMaps);
      saveMap(currMap, 0);
      break;
   case 2:
      //numMaps = EEPROM.read(32);
      saveMap(currMap, 32);
      break;
   case 3:
      //numMaps = EEPROM.read(64);
      saveMap(currMap, 64);
      break;
  }
  display.clearDisplay();
  drawMap(currMap);
  
  /*
  cursorPosMenu = 16;
  display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, BLACK);

  if (numMaps == 0){
    mapNum = 0;
  }
  else{
    displayMapMenu(numMaps);
    moveMapMenu(numMaps);
    mapNum = handleMapMenu();
    Serial.println("Map number!");
    Serial.println(mapNum);
  }
  */
  
}

void enterLoadMenu(){
  int res = -1;
  inMenu = true;
  cursorPosMenu = 64;
  displaySubMenu();
  display.display();
  while(digitalRead(6) == LOW){};

  while(inMenu){
    display.display();
    moveSubMenu();
    res = handleMenu();
  }
  switch(res) {
   case 1:
      loadMap(0);
      break;
   case 2:
      loadMap(32);
      break;
   case 3:
      loadMap(64);
      break;
  }
  display.clearDisplay();
  drawMap(currMap);
}

void enterAnimMenu(){
  while(digitalRead(6) == LOW){};
  showTime = 5;

    while(showTime > 0){
    for (int i = 0; i < 3; i++){

      for (int j = 0; j < 32; j++){
        currMap[j] = EEPROM.read((i*32) + j);
      }
      drawMap(currMap);
      display.display();
      delay(100);         
    }
    showTime--;  
        
  }


  /*
  while(showTime > 0){
    for (int j = 0; j < 32; j++){
      currMap[j] = EEPROM.read(j);
    }
    drawMap(currMap);
    display.display();
    delay(500);
    for (int j = 0; j < 32; j++){
      currMap[j] = EEPROM.read(32+j);
    }
    drawMap(currMap);
    display.display();
    delay(500);
    for (int j = 0; j < 32; j++){
      currMap[j] = EEPROM.read(64+j);
    }
    drawMap(currMap);
    display.display();
    delay(500);

    showTime--;
  }
  */

    MEMORY_PRINT_START
    MEMORY_PRINT_HEAPSTART
    MEMORY_PRINT_HEAPEND
    MEMORY_PRINT_STACKSTART
    MEMORY_PRINT_END
    MEMORY_PRINT_HEAPSIZE
    
    Serial.println();
    Serial.println();
    
    FREERAM_PRINT;
  /*
  while(!exitAnim){
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 32; j++){
        currMap[j] = EEPROM.read((i*32) + j);
      }
        drawMap(currMap);
        display.display();
        showTime = millis() + 500;
        while(showTime > millis()){
          Serial.println("STUCK");
          if (digitalRead(4) == LOW){
            exitAnim = true;
          }
        }
        Serial.println("HITHITHIT");
    }
  }
*/
  
  /*
  for (int i = 0; i < 96; i++){
    if (i%32 == 0){
      Serial.println("---------");
      Serial.println(i/32);
      Serial.println("---------");
    }
    Serial.println(EEPROM.read(i));
  }

  display.clearDisplay();
  drawMap(currMap);
  */

  
  /*
  delay(500);
  int res = -1;
  inMenu = true;
  cursorPosMenu = 64;
  displaySubMenu();

  while(inMenu){
    display.display();
    moveSubMenu();
    res = handleMenu() - 1;
  }
  switch(res) {
   case 1:
      //todo
   case 2:
      //todo
   case 3:
      //todo
   default:
      display.clearDisplay();
      drawMap(currMap);
  }
  */
}

void enterDeleteMenu(){
  int res = -1;
  inMenu = true;
  cursorPosMenu = 64;
  displaySubMenu();
  display.display();
  while(digitalRead(6) == LOW){};

  while(inMenu){
    display.display();
    moveSubMenu();
    res = handleMenu();
  }
  switch(res) {
   case 1:
      deleteMap(0);
      break;
   case 2:
      deleteMap(32);
      break;
   case 3:
      deleteMap(64);
      break;
  }
  display.clearDisplay();
  drawMap(currMap);
}



void moveSubMenu(){
  if ((millis() % 30000) - moveTime > moveWaitTime){
      if (digitalRead(3) == LOW && (cursorPosMenu < 160)){
          Serial.println("\nbutton 2 pressed!");
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, BLACK);
          cursorPosMenu += 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);
          
          moveTime = millis()% 30000;
    
      }
      if (digitalRead(4) == LOW && (cursorPosMenu > 64)){
          Serial.println("\nbutton 3 pressed!");
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, BLACK);
          cursorPosMenu -= 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);
          moveTime = millis()% 30000;
      }
  }
}

void saveMap(unsigned char mapx[32], int mem){
  for (int i = mem, j = 0; i < (mem + 32); i++, j++){
    EEPROM.write(i, mapx[j]);
  }
}

void loadMap(int mem){
  for (int i = mem, j = 0; i < (mem + 32); i++, j++){
    currMap[j] = EEPROM.read(i);
  }
}

void deleteMap(int mem){
  for (int i = mem; i < (mem + 32); i++){
    EEPROM.write(i, 0);
  }
}

void enterMenu(){
    int menuRes = -1;
    inMenu = true;
    cursorPosMenu = 16;
    
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE); // Draw white text
    display.cp437(true);         // Use full 256 char 'Code Page 437' font

    display.setCursor(44, 5);
    display.write("SAVE");
    display.setCursor(44, 17);
    display.write("LOAD");
    display.setCursor(44, 29);
    display.write("ANIMATE");
    display.setCursor(44, 41);
    display.write("DELETE");
    display.setCursor(44, 53);
    display.write("CLEAR");

    display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);

    display.display();
    while(digitalRead(6) == LOW){};

    while(inMenu){
      display.display();
      moveMenu();
      menuRes = handleMenu();
    }
    switch(menuRes) {
       case 0:
          enterSaveMenu();
          break;
       case 1:
          enterLoadMenu();
          break;
       case 2:
          enterAnimMenu();
          break; //optional
       case 3:
          enterDeleteMenu();
          break;
       default:
          memset(currMap, 0, 32);
          cursorPos = 0;
          oldCursorPos = 0;
          display.clearDisplay();
          drawMap(currMap);
    
    }
}

void displaySubMenu(){
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.setCursor(44, 5);
  display.write("SELECT:");
  display.setCursor(44, 17);
  display.write("FILE 1");
  display.setCursor(44, 29);
  display.write("FILE 2");
  display.setCursor(44, 41);
  display.write("FILE 3");

  display.fillRect(cursorPosMenu % 16 * 4 + 32, (cursorPosMenu / 16) * 4, 8, 8, WHITE);
}


void displayMapMenu(int numMaps){
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  
  int xVal = 42;
  int yVal = 5;
  
  for (int i = 1; i <= numMaps; i++){
    
    display.setCursor(xVal, yVal);
    if (i < 10){
      display.write(char(i + 48));
    }
    else{
      display.write("1");
      display.setCursor(xVal + 6, yVal);
      display.write(char((i%10) + 48));
      display.setCursor(xVal, yVal);
    }

    if (yVal == 41){
      yVal = 5;
      xVal += 20 - (i/8 * 1);
    }
    else{
      yVal += 12;
    }
  }
  display.display();
}


void moveMapMenu(int numMaps){
  cursorPosMenu = 0;
  display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, BLACK);
   
  if (millis() %30000 - moveTime > moveWaitTime && (cursorPosMenu % 16 != 10)){
      if (digitalRead(2) == LOW){
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, BLACK);
          cursorPosMenu += 5;
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, WHITE);
          moveTime = millis();
      }  
      if (digitalRead(3) == LOW && (cursorPos < 144)){
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, BLACK);
          cursorPosMenu += 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, WHITE);
          moveTime = millis();
    
      }
      if (digitalRead(4) == LOW && (cursorPos > 16)){
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, BLACK);
          cursorPosMenu -= 48;
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, WHITE);
          moveTime = millis();
      }
      if (digitalRead(5) == LOW && (cursorPosMenu % 16 != 0)){
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, BLACK);
          cursorPosMenu -= 5;
          display.fillRect(cursorPosMenu % 16 * 4 + 34, ((cursorPosMenu / 16) * 4) + 2, 4, 4, WHITE);
          moveTime = millis();
      }
  }
}

int handleMapMenu(){}
