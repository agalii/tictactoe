/*first tictactoe draft...
 
 created 2014
 by Daniel, Christian & Lisa

This code is published under GPL v2
*/


#include <DmxSimple.h>

// constants won't change. Used here to 
// set pin numbers:
const int RedledPin     = 65;      // the number of the red LED pin
const int GreenledPin   = 66;      // the number of the green LED pin
const int SensorPins[9] = {A13, A14, A2, A3, A4, A5, A10, A11, A12};
const int FieldAddr[9]  = {2, 5, 8, 29, 14, 17, 20, 23, 26};
const int StroboAddr    = 69;

const int Threshold   = 160;    // trigger value to switch LED on (from analog signal)
const int TimeOut     = 1000;  // number of loops needed for positive input signal 
// game states
const int Reset       = 0;
const int RedTurn     = 1;       
const int GreenTurn   = 2;
const int RedWon      = 3;
const int GreenWon    = 4;
const int Idle        = 5;
// field states
const int FieldFree   = 0;
const int FieldRed    = 1;
const int FieldGreen  = 2;

const int MaxBrightness  = 250;


// Variables will change:
int sensorValue;     // variable to store the value coming from the sensor, possible Range 0 - 1023
int gameState = Reset;
int count[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int fieldState[9];
int idleCount;
int idleTimeOut;

void setup() 
{
  DmxSimple.usePin(3);
  DmxSimple.maxChannel(64);

  pinMode(RedledPin, OUTPUT);      
  pinMode(GreenledPin, OUTPUT);  
  
  for (int i = 0; i < 9; i++) 
    pinMode(SensorPins[i], INPUT);    

  DmxSimple.write(1, 255); // switch master on 
  DmxSimple.write(StroboAddr, 0);
  DmxSimple.write(StroboAddr + 1, 200);
  DmxSimple.write(StroboAddr + 2, 0);
 
  idleCount = 0;
  gameState = Idle;
}

int checkTriplet (int a, int b, int c) 
{
  return fieldState[a] != FieldFree && 
         fieldState[a] == fieldState[b] && 
         fieldState[a] == fieldState[c];
}

// display whos turn it is, can be adapted to the actual setting...
int displayState (int state) 
{
  switch (state) {
    case RedTurn:
      DmxSimple.write(RedledPin, MaxBrightness);
      DmxSimple.write(GreenledPin, 0);
      break;

    case GreenTurn:
      DmxSimple.write(GreenledPin, MaxBrightness);
      DmxSimple.write(RedledPin, 0);
      break;
  }
}

int celebrate (int winnerFieldState) {
  DmxSimple.write(StroboAddr, 200);
  for (int z = 0; z < 15; z++) {
     DmxSimple.write(RedledPin, MaxBrightness);
     DmxSimple.write(GreenledPin, 0);
     for (int i = 0; i < 9; i++) {
        delay(50);
        switchLamp(i, FieldFree, 0);
     }

     delay(100);
   
     for (int i = 0; i < 9; i++) 
        switchLamp(i, winnerFieldState, MaxBrightness);
   
     DmxSimple.write(RedledPin, 0);
     DmxSimple.write(GreenledPin, MaxBrightness);
     delay(300);
  }
   
  switchAllOff();
}


// switch on red or green light, has to be adapted to the actual setting...
int switchLamp (int index, int newFieldState, int brightness)
{
    switch (newFieldState) {
       case FieldFree:
          DmxSimple.write(FieldAddr[index], 0);
          DmxSimple.write(FieldAddr[index] + 1, 0);
          break;
          
       case FieldRed:
          DmxSimple.write(FieldAddr[index], brightness);
          DmxSimple.write(FieldAddr[index] + 1, 0);
          break;
          
       case FieldGreen:
          DmxSimple.write(FieldAddr[index], 0);
          DmxSimple.write(FieldAddr[index] + 1, brightness);
          break;
    }
}

int checkFields(int newFieldState) 
{
    int fieldValue = checkSensor(newFieldState);
    if (fieldValue >= 0 && fieldState[fieldValue] == FieldFree) {
      fieldState[fieldValue] = newFieldState;
      switchLamp(fieldValue, newFieldState, MaxBrightness);
      idleTimeOut = 0;
      return 1;
    }
    
    return 0;
}

int checkWinner() 
{
  return checkTriplet(0, 1, 2) ||
         checkTriplet(3, 4, 5) ||
         checkTriplet(6, 7, 8) ||
         checkTriplet(0, 3, 6) ||
         checkTriplet(1, 4, 7) ||
         checkTriplet(2, 5, 8) ||  
         checkTriplet(0, 4, 8) ||
         checkTriplet(2, 4, 6);
}

int checkGameOver() 
{
  for (int i = 0; i < 9; i++) {
    if (fieldState[i] == FieldFree)
      return 0;
  }
  return 1;
}

int checkSensor(int newFieldState)
{
  for (int i = 0; i < 9; i++) {
    sensorValue = analogRead(SensorPins[i]); 
    if (sensorValue > Threshold) 
      count[i]++;
    else 
      count[i] = 0;

    if (fieldState[i] == FieldFree && newFieldState != FieldFree)
      switchLamp(i, newFieldState, (count[i] * MaxBrightness) / (2 *TimeOut));

    if (count[i] > TimeOut) {
      count[i] = 0;
      return i;
    } 
  }
  return -1; 
}

void switchAllOff()
{
  for (int i = 0; i < 9; i++)
    switchLamp(i, FieldFree, 0);

  DmxSimple.write(RedledPin, 0);
  DmxSimple.write(GreenledPin, 0);
  DmxSimple.write(StroboAddr, 0);
}

void loop()
{
//  celebrate(FieldRed);
//  return;

  if (idleTimeOut++ > 30000) {
    switchAllOff();
    gameState = Idle;
    idleTimeOut = 0;
  }

  switch (gameState) {
    case Reset:
      for (int i = 0; i < 9; i++)
        fieldState[i] = FieldFree;

      switchAllOff();
      idleTimeOut = 0;
      gameState = random(2) ? RedTurn:GreenTurn;
      break;

    case Idle:
      if (checkSensor(FieldFree) == 4) {
        switchAllOff();
        delay(1000);
        gameState = Reset;
        break; 
      }
      
      if (idleCount++ % 500 == 0) {
        int lamp = random(8);
        
        if (lamp == 4)
          lamp++;
          
        switchLamp(lamp, random(3), MaxBrightness);
      }
      
      break;
    
    case RedTurn:
     displayState(RedTurn);
     if (checkFields(FieldRed)) {
        if (checkWinner())
          gameState = RedWon;
        else if (checkGameOver()) {
          switchAllOff();
          gameState = Idle;
        } else
          gameState = GreenTurn;
      }
      break;

    case GreenTurn:
      displayState(GreenTurn);
      if (checkFields(FieldGreen)) {
        if (checkWinner())
           gameState = GreenWon;
        else if (checkGameOver()) {
          switchAllOff();
          gameState = Reset;
        } else
          gameState = RedTurn;
      }
      break;

    case RedWon:
      celebrate(FieldRed);
      gameState = Idle;
      break;
      
    case GreenWon:
      celebrate(FieldGreen);
      gameState = Idle;
      break;
  }  
} 


  





