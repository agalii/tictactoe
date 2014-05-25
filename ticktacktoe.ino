/*first tictactoe draft...
 
 created 2014
 by Daniel, Christian & Lisa

This code is published under GPL v2
*/

// constants won't change. Used here to 
// set pin numbers:
const int RedledPin     = 2;      // the number of the red LED pin
const int GreenledPin   = 3;      // the number of the green LED pin
const int SensorPins[9] = {A0, A1, A2, A3, A4, A5, A6, A7, A7};

const int Threshold   = 200;    // trigger value to switch LED on (from analog signal)
const int TimeOut     = 15000;  // number of loops needed for positive input signal 
// game states
const int Reset       = 0;
const int RedTurn     = 1;       
const int GreenTurn   = 2;
const int RedWon      = 3;
const int GreenWon    = 4;
// field states
const int FieldFree   = 0;
const int FieldRed    = 1;
const int FieldGreen  = 2;


// Variables will change:
int sensorValue;     // variable to store the value coming from the sensor, possible Range 0 - 1023
int redledState;         // ledState used to set the red LED
int greenledState;       // ledState used to set the green LED
int gameState = Reset;
int count[9]  = {0, 0, 0, 0, 0, 0, 0, 0, 0};
int fieldState[9];


void setup() 
{
  pinMode(RedledPin, OUTPUT);      
  pinMode(GreenledPin, OUTPUT);  
  
  for (int i = 0; i < 9; i++) 
    pinMode(SensorPins[i], INPUT);      
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
      redledState   = HIGH;
      greenledState = LOW;
      break;

    case GreenTurn:
      greenledState = HIGH;
      redledState   = LOW;
      break;
  }
}

// switch on red or green light, has to be adapted to the actual setting...
int switchLamp (int index, int newFieldState) 
{
  
}

int checkFields(int newFieldState) 
{
    int fieldValue = checkSensor();
    if (fieldValue >= 0 && fieldState[fieldValue] == FieldFree) {
      fieldState[fieldValue] = newFieldState;
      switchLamp(fieldValue, newFieldState);
    }
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

int checkSensor()
{
  for (int i = 0; i < 9; i++) {
    sensorValue = analogRead(SensorPins[i]); 
    if (sensorValue > Threshold) 
      count[i]++;
    else 
      count[i] = 0;

    if (count[i] > TimeOut) {
      count[i] = 0;
      return i;
    } 
  }
  return -1; 
  
}

  


void loop()
{
  switch (gameState) {
    case Reset:
      for (int i = 0; i < 9; i++)
        fieldState[i] = FieldFree;
      gameState = RedTurn;
      break;

    case RedTurn:
      displayState(RedTurn);
      checkFields(FieldRed);   
    
      if (checkWinner())
        gameState = RedWon;
      else
        gameState = GreenTurn;
      break;

    case GreenTurn:
      displayState(GreenTurn);
      checkFields(FieldGreen);   

      if (checkWinner())
        gameState = GreenWon;
      else
        gameState = RedTurn;
      break;

    case RedWon:
    case GreenWon:
      gameState = Reset;
      break;
  }  

  digitalWrite(GreenledPin, greenledState);
  digitalWrite(RedledPin, redledState);
} 


  





